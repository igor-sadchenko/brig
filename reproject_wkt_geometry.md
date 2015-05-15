how to reproject [WKT](http://en.wikipedia.org/wiki/Well-known_text) geometry

# main.cpp #

```
#include <brig/boost/decode.hpp>
#include <brig/boost/encode.hpp>
#include <brig/proj/shared_pj.hpp>
#include <brig/proj/transform_wkb.hpp>
#include <iterator>
#include <iostream>

int main(int argc, char* argv[])
{
  const char* wkts[] =
  { "POINT (10 10)"
  , "LINESTRING ( 10 10, 20 20, 30 40)"
  , "POLYGON ((10 10, 10 20, 20 20,20 15, 10 10))"
  , "MULTIPOINT ((10 10), (20 20))"
  , "MULTILINESTRING ((10 10, 20 20),(15 15, 30 15))"
  , "MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10)),((60 60, 70 70, 80 60, 60 60 ) ))"
  , "GEOMETRYCOLLECTION (POINT (10 10),POINT (30 30),LINESTRING (15 15, 20 20))"
  };
  brig::proj::shared_pj pj_from(4326), pj_to(3031);
  for (auto& wkt: wkts)
  {
    auto wkb(brig::boost::encode(wkt));
    brig::proj::transform_wkb(wkb, pj_from, pj_to);
    std::cout << std::endl << brig::boost::decode(wkb) << std::endl;
  }
  return 0;
}
```

# Qt pro file #

```
SOURCES +=\
  main.cpp

# Boost, brig, PROJ.4 (proj_api.h)
INCLUDEPATH +=\
  ../include/

*-g++*:QMAKE_CXXFLAGS +=\
  -std=c++0x

unix:LIBS +=\
  -ldl
```

# command prompt #

  * compile main.cpp with h-files of Boost, brig, PROJ.4 (proj\_api.h)
  * linux: `g++ -std=c++0x -I"../include/" main.cpp -ldl`
  * windows: `g++ -std=c++0x -I"../include/" main.cpp`