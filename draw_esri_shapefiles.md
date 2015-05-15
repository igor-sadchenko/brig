how to draw [ESRI Shapefiles](http://en.wikipedia.org/wiki/Shapefile)

# main.cpp #

```
#include <brig/gdal/ogr/provider.hpp>
#include <brig/qt/draw.hpp>
#include <QApplication>
#include <QFileDialog>
#include <QPainter>
#include <QString>
#include <QWidget>

class MyWidget : public QWidget {
  brig::gdal::ogr::provider m_pvd;
  brig::table_def m_tbl;
  double m_width, m_height;
  QPointF m_center;

public:
  MyWidget(QString file) : m_pvd(file.toUtf8().data())
  {
    auto layer(m_pvd.get_geometry_layers()[0]);
    m_tbl = m_pvd.get_table_def(layer);
    m_tbl.query_columns.push_back(layer.qualifier);
    auto box(m_pvd.get_extent(m_tbl));
    m_width = box.max_corner().get<0>() - box.min_corner().get<0>();
    m_height = box.max_corner().get<1>() - box.min_corner().get<1>();
    m_center = QPointF(box.min_corner().get<0>() + m_width / 2., box.min_corner().get<1>() + m_height / 2.);
  }

protected:
  void paintEvent(QPaintEvent*) override
  {
    const double scale(std::max<>(m_width / width(), m_height / height()));
    brig::qt::frame fr(m_center, scale, size());
    QPainter painter(this);
    for (auto& row: m_pvd.select(m_tbl))
      brig::qt::draw( boost::get<brig::blob_t>(row[0]), fr, painter );
  }
}; // MyWidget

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  QFileDialog dlg(0, "open ESRI Shapefile", "", "ESRI Shapefiles (*.shp)");
  dlg.setFileMode(QFileDialog::ExistingFile);
  if (dlg.exec() != QDialog::Accepted) return -1;
  MyWidget w(dlg.selectedFiles()[0]);
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

# boost, brig, gdal/ogr h-files
INCLUDEPATH +=\
  ../include/

windows:INCLUDEPATH +=\
  ../include/gdal/

unix:INCLUDEPATH +=\
  /usr/include/gdal/

*-g++*:QMAKE_CXXFLAGS +=\
  -std=c++0x

unix:LIBS +=\
  -ldl
```