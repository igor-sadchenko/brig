## Compiler ##
  * [MinGW](http://en.wikipedia.org/wiki/MinGW) (if you plan to use Qt, it already has a MinGW)
    1. download and extract [MinGW 4.7.2 (threads-posix, sjlj)](http://sourceforge.net/projects/mingwbuilds/)
    1. correct and add "<i>C:\mingw\bin;</i>" to <b>PATH</b> environment variable
  * [Microsoft Visual C++](http://en.wikipedia.org/wiki/MSVC)
    1. download and install [Visual C++ 2012 Express](http://www.microsoft.com/visualstudio/eng/products/visual-studio-express-for-windows-desktop)
    1. download and install [Visual C++ Compiler November 2012 CTP](http://aka.ms/vc-ctp)
    1. copy files <br> from "<i>C:\Program Files (x86)\Microsoft Visual C++ Compiler Nov 2012 CTP</i>" <br> to "<i>C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC</i>"</li></ul>

## Qt (optional) ##
  1. download and install [Qt](http://qt-project.org/downloads)

## Brig with third-party ##
  1. download and extract [include\_1306\_windows.zip](http://brig.googlecode.com/files/include_1306_windows.zip)
  1. correct and add "<i>C:\include;C:\include\cubrid;C:\include\gdal;C:\include\mysql;C:\include\postgresql;C:\include\oci;</i>" to environment variable: <br> <b>CPLUS_INCLUDE_PATH</b> for MinGW <br> <b>INCLUDE</b> for Microsoft Visual C++<br>
<ol><li>download and extract <a href='http://brig.googlecode.com/files/bin_1306_windows_32bit.zip'>bin_1306_windows_32bit.zip</a> or <a href='http://brig.googlecode.com/files/bin_1306_windows_64bit.zip'>bin_1306_windows_64bit.zip</a>
</li><li>correct and add "<i>C:\bin;</i>" to <b>PATH</b> environment variable