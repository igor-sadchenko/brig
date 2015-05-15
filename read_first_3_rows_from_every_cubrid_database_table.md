how to read first 3 rows from every CUBRID database table

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

  for (auto& id: pvd.get_tables())
  {
    cout << id.to_string() << endl;

    auto tbl(pvd.get_table_def(id));
    for (auto& col: tbl.columns) cout << col.name << "|";
    cout << endl;

    tbl.query_rows = 3;
    for (auto& row: pvd.select(tbl))
    {
      for (auto& cell: row) cout << cell << "|";
      cout << endl;
    }

    cout << endl;
  }

  return 0;
}
```

# Qt pro file #

```
SOURCES +=\
  main.cpp

INCLUDEPATH +=\ # boost, brig, cubrid h-files
  ../include/\
  ../include/cubrid/

*-g++*:QMAKE_CXXFLAGS +=\
  -std=c++0x

unix:LIBS +=\
  -ldl
```

# step by step guide #

## compiler ##

  * linux: [GCC 4.7](http://en.wikipedia.org/wiki/GNU_Compiler_Collection) or higher
  * windows: download and extract [MinGW 4.7 (threads-posix, sjlj)](http://sourceforge.net/projects/mingwbuilds/) or higher, add "./mingw/bin" to Path environment variable

## CUBRID CCI driver ##

  * download and extract [CUBRID CCI driver](http://www.cubrid.org/?mid=downloads&item=cci_driver&os=detect)
  * linux: copy content of "./cubrid-cci-8.4.1.2032/lib" to "/usr/lib", restart
  * windows: add "./cubrid-cci-8.4.1.2032/lib" to Path environment variable

## Boost ##

  * download and extract [Boost](http://www.boost.org/users/download/)

## brig ##

  * download and extract [brig](http://code.google.com/p/brig/)

## example ##

  * compile main.cpp with h-files of CUBRID, Boost, brig
  * linux: `g++ -std=c++0x -I"./CUBRID/include" -I"./boost_1_49_0" -I"." main.cpp -ldl`
  * windows: `g++ -std=c++0x -I"./cubrid-cci-8.4.1.2032/include" -I"./boost_1_49_0" -I"." main.cpp`