how to draw [WKT](http://en.wikipedia.org/wiki/Well-known_text) geometry

# main.cpp #

```
#include <brig/boost/encode.hpp>
#include <brig/qt/draw.hpp>
#include <QApplication>
#include <QWidget>

class MyWidget : public QWidget {
protected:
  void paintEvent(QPaintEvent*) override
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
    const QPointF center(45, 40);
    const double scale(std::max<>(75. / width(), 65. / height()));
    brig::qt::frame fr(center, scale, size());
    QPainter painter(this);
    for (auto& wkt: wkts)
      brig::qt::draw( brig::boost::encode(wkt), fr, painter );
  }
}; // MyWidget

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  MyWidget w;
  w.show();
  return a.exec();
}
```

# Qt pro file #

```
QT +=\
  core\
  gui

greaterThan(QT_MAJOR_VERSION, 4):QT +=\
  widgets

SOURCES +=\
  main.cpp

# boost, brig
INCLUDEPATH +=\
  ../include/

*-g++*:QMAKE_CXXFLAGS +=\
  -std=c++0x

unix:LIBS +=\
  -ldl
```


![http://dl.dropbox.com/u/34248515/frame.png](http://dl.dropbox.com/u/34248515/frame.png)