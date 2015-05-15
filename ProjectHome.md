Brig is a collection of cross-platform [C++ 11](http://en.wikipedia.org/wiki/C%2B%2B) [header only](http://en.wikipedia.org/wiki/Header-only) libraries that offers interaction with many databases and usability of the geometric data.


## Libraries ##

---


### boost ###
  * use to convert a geometry between [Boost.Geometry](http://www.boost.org/libs/geometry), [WKB](http://en.wikipedia.org/wiki/Well-known_text#Well-known_binary) / [OpenGIS 99-049](http://www.opengeospatial.org/standards/sfs) and [WKT](http://en.wikipedia.org/wiki/Well-known_text)
  * depends on [Boost](http://www.boost.org/) ([header only](http://en.wikipedia.org/wiki/Header-only))

### database ###
  * use as a compact wrapper for a unified access to databases:
    1. [CUBRID](http://en.wikipedia.org/wiki/CUBRID) (no spatial data)
    1. [DB2](http://en.wikipedia.org/wiki/IBM_DB2) + [Spatial Extender](http://www.ibm.com/software/data/spatial/db2spatial/)
    1. [Informix](http://en.wikipedia.org/wiki/IBM_Informix) + [Spatial DataBlade](http://www.ibm.com/software/data/informix/blades/spatial/)
    1. [Ingres](http://en.wikipedia.org/wiki/Ingres_database) (no valid spatial index)
    1. [Microsoft SQL Server](http://en.wikipedia.org/wiki/Microsoft_SQL_Server)
    1. [MySQL](http://en.wikipedia.org/wiki/MySQL)
    1. [Oracle](http://en.wikipedia.org/wiki/Oracle_Database)
    1. [Postgres](http://en.wikipedia.org/wiki/PostgreSQL) + [PostGIS](http://en.wikipedia.org/wiki/PostGIS)
    1. [SQLite](http://en.wikipedia.org/wiki/SQLite) + [SpatiaLite](http://en.wikipedia.org/wiki/Spatialite)
  * supported formats: [WKB](http://en.wikipedia.org/wiki/Well-known_text#Well-known_binary) / [OpenGIS 99-049](http://www.opengeospatial.org/standards/sfs) (geometry), [UTF-8](http://en.wikipedia.org/wiki/UTF-8) (text), [ISO 8601](http://en.wikipedia.org/wiki/ISO_8601) (datetime)
  * supported rasters: [RasterLite](https://www.gaia-gis.it/fossil/librasterlite/index), [WKTRaster](http://trac.osgeo.org/postgis/wiki/WKTRaster), [simple\_rasters](http://code.google.com/p/brig/wiki/simple_rasters)
  * ability to receive metadata (TABLES, COLUMNS, INDEXES) and to use basic SQL commands (SELECT, INSERT, CREATE, DROP) without a line of code in SQL
  * multithreading
  * depends on [Boost](http://www.boost.org/) ([header only](http://en.wikipedia.org/wiki/Header-only))
  * optionally depends on ([header only](http://en.wikipedia.org/wiki/Header-only) + [dynamic loading](http://en.wikipedia.org/wiki/Dynamic_loading)):
    1. [CUBRID C Client Interface](http://www.cubrid.org/manual/90/en/CCI%20Overview-Overview)
    1. [libpq C Library](http://www.postgresql.org/docs/9.2/static/libpq.html)
    1. [MySQL C API](http://dev.mysql.com/doc/refman/5.6/en/c.html)
    1. [ODBC](http://en.wikipedia.org/wiki/ODBC) / [unixODBC](http://en.wikipedia.org/wiki/UnixODBC)
    1. [Oracle Call Interface](http://en.wikipedia.org/wiki/Oracle_Call_Interface)
    1. [SQLite C Interface](http://www.sqlite.org/c3ref/intro.html)

### gdal ###
  * use to read all georeferenced rasters [supported](http://www.gdal.org/formats_list.html) by [GDAL](http://en.wikipedia.org/wiki/GDAL)
  * use to read (and sometimes write) a variety of vector file formats [supported](http://www.gdal.org/ogr/ogr_formats.html) by [OGR](http://www.gdal.org/ogr/)
  * depends on [Boost](http://www.boost.org/) ([header only](http://en.wikipedia.org/wiki/Header-only)) and [GDAL](http://en.wikipedia.org/wiki/GDAL) ([header only](http://en.wikipedia.org/wiki/Header-only) + [dynamic loading](http://en.wikipedia.org/wiki/Dynamic_loading))

### osm ###
  * use to read [OpenStreetMap](http://en.wikipedia.org/wiki/OpenStreetMap) [tiles](http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames)
  * depends on [Boost](http://www.boost.org/) ([header only](http://en.wikipedia.org/wiki/Header-only)) and [cURL](http://en.wikipedia.org/wiki/CURL) ([header only](http://en.wikipedia.org/wiki/Header-only) + [dynamic loading](http://en.wikipedia.org/wiki/Dynamic_loading))

### proj ###
  * use to change a projection of [WKB](http://en.wikipedia.org/wiki/Well-known_text#Well-known_binary) / [OpenGIS 99-049](http://www.opengeospatial.org/standards/sfs)
  * [EPSG](http://www.epsg.org/) support
  * depends on [PROJ.4](http://trac.osgeo.org/proj/) ([header only](http://en.wikipedia.org/wiki/Header-only) + [dynamic loading](http://en.wikipedia.org/wiki/Dynamic_loading))

### qt ###
  * use to draw [WKB](http://en.wikipedia.org/wiki/Well-known_text#Well-known_binary) / [OpenGIS 99-049](http://www.opengeospatial.org/standards/sfs)
  * depends on [Qt](http://en.wikipedia.org/wiki/Qt_%28framework%29)

### unicode ###
  * use to transform a unicode text between [UTF-8](http://en.wikipedia.org/wiki/UTF-8), [UTF-16](http://en.wikipedia.org/wiki/UTF-16) and [UTF-32](http://en.wikipedia.org/wiki/UTF-32)
  * use for case mappings


## Test compilers ##

---


  * [GCC 4.7.3](http://en.wikipedia.org/wiki/GNU_Compiler_Collection)
  * [MinGW 4.7.2 (threads-posix, sjlj)](http://sourceforge.net/projects/mingwbuilds/)
  * [Visual C++ Compiler November 2012 CTP](http://aka.ms/vc-ctp)


## See also ##

---

  * [CUBRID website](http://www.cubrid.org/wiki_apps/entry/brig-key-features)
  * [GIS commander](http://code.google.com/p/brigantine/)
  * [Flagship game](https://sites.google.com/site/flagshipgame/)



---

andrew.naplavkov@gmail.com