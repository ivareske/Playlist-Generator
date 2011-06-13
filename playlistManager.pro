#generate other playlist formats?
#make scripting work
#generate one playlist for each artist
#foo fighters music videos, får feilmelding om S:/Min Musikk/F/Foo Fighters/Foo Fighters music videos/Foo Fighters - Walking After You.mpg, TagLib: FileRef::tag() - Called without a valid file., TagLib: FileRef::audioProperties() - Called without a valid file.
#om ingen folder funnet, får aborted by user. som jo ikke er tilfelle
#when renaming files in tag editor, specify a list of files to rename the renamed files.
#fix stylesheet actions/checked etc. when style menu is triggered, check the correct action
#fix textviewer
#add id3 tags

# The application version
VERSION = 1.0
TARGET = PlayListGenerator

TEMPLATE=app
CONFIG += qt
# release
#CONFIG += static
CONFIG += console
QT += script
 
INCLUDEPATH  += ./taglib
LIBS += -L./ libtag


HEADERS = PlaylistManager.h SettingsDialog.h RuleDialog.h PlayList.h Rule.h globalFunctions.h M3uEntry.h AddFilesDialog.h TextViewer.h Tag.h \
    PlayListCollection.h StyleSheetEditor.h \
    TextEdit.h
FORMS   = PlaylistManager.ui SettingsDialog.ui RuleDialog.ui AddFilesDialog.ui TextViewer.ui StyleSheetEditor.ui
SOURCES = main.cpp PlaylistManager.cpp  SettingsDialog.cpp RuleDialog.cpp PlayList.cpp Rule.cpp globalFunctions.cpp M3uEntry.cpp AddFilesDialog.cpp TextViewer.cpp Tag.cpp \
    PlayListCollection.cpp StyleSheetEditor.cpp \
    TextEdit.cpp

RC_FILE = PlaylistManager.rc


DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"



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
