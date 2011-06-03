#generate other playlist formats?
#http://nsis.sourceforge.net/Category:Real_World_Installers
#//http://doc.trolltech.com/4.5/widgets-stylesheet-stylesheeteditor-cpp.html
#make scripting work
#generate one playlist for each unique combination of artist and album (with number of songs larger than a specified limit)... nødvendig? kommer dette til å bli brukt?
#edit extensions for multiple lists
#foo fighters music videos, får feilmelding om S:/Min Musikk/F/Foo Fighters/Foo Fighters music videos/Foo Fighters - Walking After You.mpg, TagLib: FileRef::tag() - Called without a valid file., TagLib: FileRef::audioProperties() - Called without a valid file.
#om ingen folder funnet, får aborted by user. som jo ikke er tilfelle
#implement stream operators for classes for easy saving with qsettings...
#cleanup so only source files are in folder
#I know a lot of the design could have been done better, but this was one of my first larger programs. And it works...
#prøve å kompilere taglib med qt? pro file ligge ved
#fix const correctness and implement stream operators

# The application version
VERSION = 1.0


TEMPLATE=app
TARGET = PlayListGenerator
CONFIG += qt release warn_on 
#CONFIG += static
CONFIG += console
#QT += script
 
INCLUDEPATH  += ./taglib
LIBS += -L./ libtag
#LIBS += $$quote(C:/Qt/4.6.3/plugins/script/qtscript_cored.dll)


HEADERS = PlaylistManager.h SettingsDialog.h RuleDialog.h PlayList.h Rule.h globalFunctions.h M3uEntry.h AddFilesDialog.h TextViewer.h Tag.h \
    PlayListCollection.h
FORMS   = PlaylistManager.ui SettingsDialog.ui RuleDialog.ui AddFilesDialog.ui TextViewer.ui
SOURCES = main.cpp PlaylistManager.cpp  SettingsDialog.cpp RuleDialog.cpp PlayList.cpp Rule.cpp globalFunctions.cpp M3uEntry.cpp AddFilesDialog.cpp TextViewer.cpp Tag.cpp \
    PlayListCollection.cpp

RC_FILE = PlaylistManager.rc


DEFINES += APP_VERSION=$$VERSION
DEFINES += APP_NAME=$$TARGET



#to build taglib (windows): download taglib source from
#http://developer.kde.org/~wheeler/taglib.html
#Download the Zlib source (zlib125.zip) and unpack it with unzip or 7zip. Then, compile Zlib like this:
#cd zlib-1.2.5/ 
#make -f win32/Makefile.gcc
#Manually copy the files as follows:
#cp -iv zlib1.dll c:/mingw/bin
#cp -iv zconf.h zlib.h c:/mingw/include
#cp -iv libz.a c:/mingw/lib
#cp -iv libzdll.a c:/mingw/lib/libz.dll.a
#install cmake, choose where the source code is (e.g. C:\taglib-1.6.3) and where to build
#prews configure, choose mingw makefiles and "use default native compilers"
#toggle advanced view
#set CMAKE_INSTALL_DIR to where you want to install, and set ZLIB_INCLUDE_DIR to c:/mingw/include
#and ZLIB_LIBRARY to C:/MinGW/bin/zlib1.dll
#check with_asf and with_mp4
#In CMAKE_EXE_LINKER_FLAGS and CMAKE_SHARED_LINKER_FLAGS add the following text: -Wl,--enable-auto-import
#press configure again
#press generate. in the build directory you choose, type make, and then make install
#you can now delete the taglib source and binaries
#build qt static: configure -static -no-exceptions; make sub-src
