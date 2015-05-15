how to read [SpatiaLite](http://en.wikipedia.org/wiki/Spatialite) geometry that intersected with rect

# main.cpp #

```
#include <brig/boost/as_binary.hpp>
#include <brig/boost/decode.hpp>
#include <brig/database/provider.hpp>
#include <brig/database/sqlite/command_allocator.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
  using namespace std;
  using namespace brig;
  using namespace brig::boost;

  database::provider<false> pvd(make_shared<database::sqlite::command_allocator>("./Mexico.sqlite"));
  auto lr(pvd.get_geometry_layers()[0]);
  auto tbl(pvd.get_table_def(lr));
  tbl.query_columns.push_back(lr.qualifier);
  tbl[lr.qualifier]->query_value = as_binary(box(point(-90.0, 21.0), point(-89.5, 21.5)));
  for (auto& row: pvd.select(tbl))
    cout << decode(::boost::get<blob_t>(row[0])) << endl;

  return 0;
}
```

# Qt pro file #

```
SOURCES +=\
  main.cpp

INCLUDEPATH +=\ # Boost, brig, SQLite (sqlite3.h)
  ../include/

*-g++*:QMAKE_CXXFLAGS +=\
  -std=c++0x

unix:LIBS +=\
  -ldl
```

# command prompt #

  * compile main.cpp with h-files of Boost, brig, SQLite (sqlite3.h)
  * linux: `g++ -std=c++0x -I"../include/" main.cpp -ldl`
  * windows: `g++ -std=c++0x -I"../include/" main.cpp`