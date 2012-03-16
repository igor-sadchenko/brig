// Andrew Naplavkov

#ifndef BRIG_DATABASE_COMMAND_HPP
#define BRIG_DATABASE_COMMAND_HPP

#include <brig/database/column_definition.hpp>
#include <brig/database/detail/sql_identifier.hpp>
#include <brig/database/global.hpp>
#include <brig/database/rowset.hpp>
#include <brig/database/variant.hpp>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace brig { namespace database {

struct command : public rowset {
  virtual void exec
    ( const std::string& sql
    , const std::vector<variant>& params = std::vector<variant>()
    , const std::vector<column_definition>& param_cols = std::vector<column_definition>()
    ) = 0;
  virtual size_t affected() = 0;

  // dialect
  virtual DBMS system() = 0;
  virtual std::string sql_parameter(size_t order, const column_definition& param_col);
  virtual std::string sql_column(const column_definition& col);

  // transaction
  virtual void set_autocommit(bool autocommit) = 0;
  virtual void commit() = 0;
}; // command

inline std::string command::sql_parameter(size_t, const column_definition& param_col)
{
  using namespace detail;
  const DBMS sys(system());
  std::ostringstream stream; stream.imbue(std::locale::classic());
  if (Geometry == param_col.type)
    switch (sys)
    {
    case DB2:
      stream << sql_identifier(sys, param_col.dbms_type) << "(CAST(? AS BLOB (100M)), " << param_col.srid << ")";
      return stream.str();

    case MS_SQL:
      stream << sql_identifier(sys, param_col.dbms_type) << "::STGeomFromWKB(?, " << param_col.srid << ")";
      return stream.str();

    case MySQL:
    case SQLite:
      stream << "GeomFromWKB(?, " << param_col.srid << ")";
      return stream.str();

    case Oracle:
      {
      const bool conv("sdo_geometry" != param_col.lower_case_type.name);
      if (conv) stream << sql_identifier(sys, param_col.dbms_type) << "(";
      stream << "MDSYS.SDO_GEOMETRY(TO_BLOB(?), " << param_col.srid << ")";
      if (conv) stream << ")";
      }
      return stream.str();

    case Postgres:
      if ("geography" == param_col.lower_case_type.name)
      {
        if (param_col.srid != 4326) throw std::runtime_error("SRID error");
        stream << "ST_GeogFromWKB(?)";
      }
      else
        stream << "ST_GeomFromWKB(?, " << param_col.srid << ")";
      return stream.str();
    }
  return "?";
}

inline std::string command::sql_column(const column_definition& col)
{
  using namespace detail;
  const DBMS sys(system());
  const std::string id(sql_identifier(sys, col.name));

  if (!col.sql_expression.empty()) return col.sql_expression + " " + id;

  if (Postgres == sys)
  {
    if ("user-defined" != col.lower_case_type.schema) return id;
    else if ("raster" == col.lower_case_type.name) return "ST_AsBinary(ST_Envelope(" + id + ")) " + id;
    else if ("geography" == col.lower_case_type.name
          || "geometry" == col.lower_case_type.name) return "ST_AsBinary(" + id + ") " + id;
    else return id;
  }

  if (Geometry == col.type)
    switch (sys)
    {
    case DB2: return "DB2GSE.ST_AsBinary(" + id + ") " + id;
    case MS_SQL: return id + ".STAsBinary() " + id;
    case MySQL:
    case SQLite: return "AsBinary(" + id + ") " + id;
    case Oracle: return sql_identifier(sys, col.dbms_type) + ".GET_WKB(" + id + ") " + id;
    }

  return id;
} // command::

} } // brig::database

#endif // BRIG_DATABASE_COMMAND_HPP
