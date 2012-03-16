// Andrew Naplavkov

#ifndef BRIG_DATABASE_DETAIL_SQL_TABLE_HPP
#define BRIG_DATABASE_DETAIL_SQL_TABLE_HPP

#include <algorithm>
#include <boost/geometry/algorithms/covered_by.hpp>
#include <brig/boost/geometry.hpp>
#include <brig/database/column_definition.hpp>
#include <brig/database/detail/get_columns.hpp>
#include <brig/database/detail/is_geodetic_type.hpp>
#include <brig/database/detail/normalize_hemisphere.hpp>
#include <brig/database/detail/sql_box_filter.hpp>
#include <brig/database/detail/sql_identifier.hpp>
#include <brig/database/detail/sql_limit.hpp>
#include <brig/database/detail/sql_select_list.hpp>
#include <brig/database/global.hpp>
#include <brig/database/index_definition.hpp>
#include <brig/database/table_definition.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace brig { namespace database { namespace detail {

template <typename Dialect>
std::string sql_table(std::shared_ptr<Dialect> dct, const table_definition& tbl)
{
  const DBMS sys(dct->system());
  std::vector<column_definition> cols(tbl.columns);
  std::vector<column_definition> select_cols = tbl.select_columns.empty()? cols: get_columns(cols, tbl.select_columns);
  std::string sql_infix, sql_condition, sql_suffix;
  sql_limit(sys, tbl.rows, sql_infix, sql_condition, sql_suffix);

  //
  auto geom_col = std::find_if(cols.begin(), cols.end(), [&](const column_definition& col){ return col.name == tbl.box_filter_column; });
  if ( tbl.box_filter_column.empty() ||
       geom_col != cols.end() && geom_col->mbr.type() == typeid(brig::boost::box) && ::boost::geometry::covered_by(::boost::get<brig::boost::box>(geom_col->mbr), tbl.box_filter)
     )
  {
    std::string res;
    if (!sql_condition.empty()) res += "SELECT * FROM (";
    res += "SELECT " + sql_infix + " " + sql_select_list(dct, select_cols) + " FROM " + sql_identifier(sys, tbl.id);
    if (!tbl.sql_filter.empty()) res += " WHERE " + tbl.sql_filter;
    if (!sql_condition.empty()) res += ") WHERE " + sql_condition;
    res += " " + sql_suffix;
    return res;
  }

  //
  if (geom_col == cols.end()) throw std::runtime_error("SQL error");
  std::vector<brig::boost::box> boxes(1, tbl.box_filter);
  normalize_hemisphere(boxes, sys, is_geodetic_type(sys, *geom_col));

  std::string sql_hint;
  if (MS_SQL == sys)
  {
    auto p_idx = std::find_if(tbl.indexes.begin(), tbl.indexes.end(), [&](const index_definition& idx){ return idx.type == Spatial && idx.columns.front() == tbl.box_filter_column; });
    if (p_idx == tbl.indexes.end()) throw std::runtime_error("SQL error");
    sql_hint = "WITH(INDEX(" + sql_identifier(sys, p_idx->id) + "))";
  }

  std::vector<column_definition> unique_cols;
  auto p_idx = std::find_if(tbl.indexes.begin(), tbl.indexes.end(), [&](const index_definition& idx){ return idx.type == Primary; });
  if (p_idx == tbl.indexes.end()) p_idx = std::find_if(tbl.indexes.begin(), tbl.indexes.end(), [&](const index_definition& idx){ return idx.type == Unique; });
  if (p_idx != tbl.indexes.end()) unique_cols = get_columns(cols, p_idx->columns);

  // key table
  std::string sql_key_tbl, sql_tbl(sql_identifier(sys, tbl.id));
  if (MS_SQL == sys && boxes.size() > 1)
  {
    if (unique_cols.empty()) throw std::runtime_error("SQL error");
    for (size_t i(0); i < boxes.size(); ++i)
    {
      if (i > 0) sql_key_tbl += " UNION ";
      sql_key_tbl += "(SELECT " + sql_select_list(dct, unique_cols) + " FROM " + sql_tbl + " " + sql_hint + " WHERE " + sql_box_filter(sys, *geom_col, boxes[i]) + ")";
    }
  }
  else if (Oracle == sys && boxes.size() > 1)
  {
    if (unique_cols.empty()) throw std::runtime_error("SQL error");
    sql_key_tbl += "SELECT " + sql_infix + " DISTINCT * FROM (";
    for (size_t i(0); i < boxes.size(); ++i)
    {
      if (i > 0) sql_key_tbl += " UNION ALL ";
      sql_key_tbl += "(SELECT " + sql_infix + " " + sql_select_list(dct, unique_cols) + " FROM " + sql_tbl + " WHERE " + sql_box_filter(sys, *geom_col, boxes[i]);
      if (!sql_condition.empty()) sql_key_tbl += " AND " + sql_condition;
      sql_key_tbl += ")";
    }
    sql_key_tbl += ")";
  }
  else if (SQLite == sys)
  {
    if (unique_cols.size() != 1) throw std::runtime_error("SQL error");
    sql_key_tbl += "SELECT pkid " + sql_identifier(sys, unique_cols[0].name) + " FROM " + sql_identifier(sys, "idx_" + tbl.id.name + "_" + tbl.box_filter_column) + " WHERE ";
    for (size_t i(0); i < boxes.size(); ++i)
    {
      if (i > 0) sql_key_tbl += " OR ";
      sql_key_tbl += sql_box_filter(sys, *geom_col, boxes[i]);
    }
  }

  // result
  std::string res;
  if (!sql_condition.empty()) res += "SELECT * FROM (";
  res += "SELECT " + sql_infix + " ";
  if (sql_key_tbl.empty())
  {
    res += sql_select_list(dct, select_cols) + " FROM " + sql_tbl + " " + sql_hint + " WHERE (";
    for (size_t i(0); i < boxes.size(); ++i)
    {
      if (i > 0) res += " OR ";
      res += sql_box_filter(sys, *geom_col, boxes[i]);
    }
    res += ")";
    if (!tbl.sql_filter.empty()) res += " AND " + tbl.sql_filter;
  }
  else
  {
    for (size_t i(0); i < select_cols.size(); ++i)
    {
      if (i > 0) res += ", ";
      const std::string id(sql_identifier(sys, select_cols[i].name));
      res += "v." + id + " " + id;
    }
    res += " FROM (" + sql_key_tbl + ") k JOIN (SELECT " + sql_select_list(dct, select_cols);
    for (size_t i(0); i < unique_cols.size(); ++i)
      if (std::find_if(select_cols.begin(), select_cols.end(), [&](const column_definition& col){ return col.name == unique_cols[i].name; }) == select_cols.end())
        res += ", " + dct->sql_column(unique_cols[i]);
    res += " FROM " + sql_tbl;
    if (!tbl.sql_filter.empty()) res += " WHERE " + tbl.sql_filter;
    res += ") v ON ";
    for (size_t i(0); i < unique_cols.size(); ++i)
    {
      if (i > 0) res += " AND ";
      res += "k." + sql_identifier(sys, unique_cols[i].name) + " = v." + sql_identifier(sys, unique_cols[i].name);
    }
  }
  res += " " + sql_suffix;
  if (!sql_condition.empty()) res += ") WHERE " + sql_condition;
  return res;
}

} } } // brig::database::detail

#endif // BRIG_DATABASE_DETAIL_SQL_TABLE_HPP
