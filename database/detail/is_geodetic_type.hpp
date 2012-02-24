// Andrew Naplavkov

#ifndef BRIG_DATABASE_DETAIL_IS_GEODETIC_TYPE_HPP
#define BRIG_DATABASE_DETAIL_IS_GEODETIC_TYPE_HPP

#include <brig/database/column_detail.hpp>
#include <brig/database/detail/is_geometry_type.hpp>
#include <brig/database/global.hpp>

namespace brig { namespace database { namespace detail {

inline bool is_geodetic_type(DBMS sys, const column_detail& col)
{
  if (is_geometry_type(sys, col))
    switch (sys)
    {
    case VoidSystem:
    case MySQL:
    case SQLite: break;
    case DB2: return 2000000000 <= col.srid && col.srid <= 2000001000;
    case MS_SQL:
    case Postgres: return "geography" == col.case_folded_type.name;
    case Oracle: return "geographic2d" == col.case_folded_type_detail || "geographic3d" == col.case_folded_type_detail;
    }
  return false;
}

} } } // brig::database::detail

#endif // BRIG_DATABASE_DETAIL_IS_GEODETIC_TYPE_HPP
