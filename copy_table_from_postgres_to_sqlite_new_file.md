how to copy table from Postgres to SQLite new file

# main.cpp #

```
#include <algorithm>
#include <brig/boost/as_binary.hpp>
#include <brig/database/provider.hpp>
#include <brig/database/postgres/command_allocator.hpp>
#include <brig/database/sqlite/command_allocator.hpp>

int main(int argc, char* argv[])
{
  brig::database::provider<false> pvd_from(std::make_shared<brig::database::postgres::command_allocator>
    ( "gis-lab.info"
    , 5432
    , "osm_shp"
    , "guest"
    , "guest"
    ));
  brig::identifier id = { "layer", "RU-NIZ railway-line", "" };
  auto tbl_from = pvd_from.get_table_def(id);

  brig::database::provider<false> pvd_to(std::make_shared<brig::database::sqlite::command_allocator>
    ( "./new_file.sqlite"
    ));
  pvd_to.get_command()->exec("SELECT InitSpatialMetaData();"); // SQLite: only once
  auto tbl_to = pvd_to.fit_to_create(tbl_from);
  for (auto& col_to: tbl_to.columns)
  {
    // MS_SQL, Oracle: there is a chance to get an extra field in the tail - the primary key with automatic generation
    if (tbl_to.query_columns.size() < tbl_from.columns.size())
      tbl_to.query_columns.push_back( col_to.name );
    // Ingres, MS_SQL, Oracle need a bounding box during the creation
    if (col_to.is_extent_requested())
      col_to.query_value = brig::boost::as_binary( pvd_from.get_extent(tbl_from) );
  }
  pvd_to.create(tbl_to);

  auto rs = pvd_from.select(tbl_from);
  auto ins = pvd_to.get_inserter(tbl_to);
  std::copy(std::begin(rs), std::end(rs), brig::insert_iterator(ins));
  return 0;
}
```

# Qt pro file #

```
SOURCES +=\
  main.cpp

# Boost, brig, Postgres (libpq-fe.h, ...), SQLite (sqlite3.h)
windows:INCLUDEPATH +=\
  ../include/\
  ../include/postgresql/

unix:INCLUDEPATH +=\
  ../include/\
  /usr/include/postgresql/

*-g++*:QMAKE_CXXFLAGS +=\
  -std=c++0x

unix:LIBS +=\
  -ldl
```

# command prompt #

  * compile main.cpp with h-files of Boost, brig, Postgres (libpq-fe.h, ...), SQLite (sqlite3.h)
  * linux: `g++ -std=c++0x -I"../include/" -I"/usr/include/postgresql/" main.cpp -ldl`
  * windows: `g++ -std=c++0x -I"../include/" -I"../include/postgresql/" main.cpp`