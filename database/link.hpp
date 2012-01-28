// Andrew Naplavkov

#ifndef BRIG_DATABASE_LINK_HPP
#define BRIG_DATABASE_LINK_HPP

#include <boost/utility.hpp>
#include <brig/database/column_detail.hpp>
#include <brig/database/dbms.hpp>
#include <brig/database/detail/is_geometry_type.hpp>
#include <brig/database/detail/sql_identifier.hpp>
#include <brig/database/rowset.hpp>
#include <brig/database/variant.hpp>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace brig { namespace database {

struct link : public rowset
{
  // dialect
  virtual DBMS system() = 0;
  virtual void sql_parameter(int order, const column_detail& param_col, std::ostringstream& stream);
  virtual void sql_column(const column_detail& col, std::ostringstream& stream);

  // command
  virtual void exec(const std::string& sql, const std::vector<variant>& params, const std::vector<column_detail>& param_cols) = 0;
  virtual int64_t affected() = 0;
  virtual void columns(std::vector<std::string>& cols) = 0;

  // transaction
  virtual void start() = 0;
  virtual void commit() = 0;
  virtual void rollback() = 0;
}; // link

inline void link::sql_parameter(int, const column_detail& param_col, std::ostringstream& stream)
{
  const DBMS sys(system());
  if (detail::is_geometry_type(sys, param_col))
    switch (sys)
    {
    case UnknownSystem:
      break;

    case DB2:
      stream << param_col.type_schema << '.' << param_col.type_name << "(CAST(? AS BLOB (100M)), " << param_col.srid << ')';
      return;

    case MS_SQL:
      stream << param_col.type_name << "::STGeomFromWKB(?, " << param_col.srid << ')';
      return;

    case MySQL:
    case SQLite:
      stream << "GeomFromWKB(?, " << param_col.srid << ')';
      return;

    case Oracle:
      if (param_col.type_name != "SDO_GEOMETRY") stream << param_col.type_schema << '.' << param_col.type_name << '(';
      stream << "MDSYS.SDO_GEOMETRY(TO_BLOB(?), " << param_col.srid << ')';
      if (param_col.type_name != "SDO_GEOMETRY") stream << ')';
      return;

    case Postgres:
      if (param_col.type_name == "GEOGRAPHY")
      {
        if (param_col.srid != 4326) throw std::runtime_error("it only supports wgs 84 long lat (srid:4326)");
        stream << "ST_GeogFromWKB(?)";
      }
      else
        stream << "ST_GeomFromWKB(?, " << param_col.srid << ')';
      return;
    }
  stream << '?';
}

inline void link::sql_column(const column_detail& col, std::ostringstream& stream)
{
  const DBMS sys(system());
  if (detail::is_geometry_type(sys, col))
    switch (sys)
    {
    case UnknownSystem:
      break;

    case DB2:
      stream << "DB2GSE.ST_AsBinary(";
      detail::sql_identifier(sys, col.name, stream);
      stream << ')';
      return;

    case MS_SQL:
      detail::sql_identifier(sys, col.name, stream);
      stream << ".STAsBinary() ";
      detail::sql_identifier(sys, col.name, stream);
      return;

    case MySQL:
    case SQLite:
      stream << "AsBinary(";
      detail::sql_identifier(sys, col.name, stream);
      stream << ')';
      return;

    case Oracle:
      stream << col.type_schema << '.' << col.type_name << ".GET_WKB(";
      detail::sql_identifier(sys, col.name, stream);
      stream << ')';
      return;

    case Postgres:
      stream << "ST_AsBinary(";
      detail::sql_identifier(sys, col.name, stream);
      stream << ')';
      return;
    }
  detail::sql_identifier(sys, col.name, stream);
} // link::

} } // brig::database

#endif // BRIG_DATABASE_LINK_HPP