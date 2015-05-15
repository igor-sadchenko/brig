how to read CUBRID database table with condition

# main.cpp #

```
#include <brig/database/provider.hpp>
#include <brig/database/cubrid/command_allocator.hpp>
#include <iostream>
#include <iterator>

int main(int argc, char* argv[])
{
  using namespace std;

  brig::database::provider<false> pvd(make_shared<brig::database::cubrid::command_allocator>
    ( "192.168.1.152"
    , 33000
    , "demodb"
    , "dba"
    , ""
    ));

  brig::identifier id = { "PUBLIC", "athlete", "" };
  auto tbl(pvd.get_table_def(id));
  for (auto& col: tbl.columns) cout << col.name << "|";
  cout << endl;

  tbl["nation_code"]->query_value = "IRL";
  for (auto& row: pvd.select(tbl))
  {
    for (auto& cell: row) cout << cell << "|";
    cout << endl;
  }

  return 0;
}
```

# Qt pro file #

```
SOURCES +=\
  main.cpp

INCLUDEPATH +=\ # Boost, brig, CUBRID (cas_cci.h, ...)
  ../include/\
  ../include/cubrid/

*-g++*:QMAKE_CXXFLAGS +=\
  -std=c++0x

unix:LIBS +=\
  -ldl
```

# command prompt #

  * compile main.cpp with h-files of Boost, brig, CUBRID (cas\_cci.h, ...)
  * linux: `g++ -std=c++0x -I"../include/" -I"../include/cubrid/" main.cpp -ldl`
  * windows: `g++ -std=c++0x -I"../include/" -I"../include/cubrid/" main.cpp`