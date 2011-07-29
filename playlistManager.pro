#generate other playlist formats?
#generate one playlist for each artist?
#foo fighters music videos, fileref blir 0, får ikke tag eller audioproperties
#when renaming files in tag editor, specify a list of files to rename the renamed files.
#fix textviewer
#add id3 tags
#qstring bindings for qtscript
#subclass qsettings to check/make error message when non-existing value is asked for? all qsettings values should be initialized at startup
#test freopen stdout
#TEST READING OF APE TAGS/ITEMS
#git messages appear in console windows...? why?
#show total time of generated playlist

# The application version
VERSION = 1.0
#NAME = $$quote(Playlist Generator)
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
#mingw32-make -f win32/Makefile.gcc
#Manually copy the files as follows:
#copy zlib1.dll c:\mingw\bin
#copy zlib.h c:\mingw\include
#copy zconf.h c:\mingw\include
#copy libz.a c:\mingw\lib
#copy libzdll.a c:\mingw\lib\libz.dll.a
#install cmake, choose where the source code is (e.g. C:\taglib-1.6.3) and where to build
#prews configure, choose mingw makefiles and "use default native compilers"
#toggle advanced view
#set CMAKE_INSTALL_DIR to where you want to install, and set ZLIB_INCLUDE_DIR to c:/mingw/include
#and ZLIB_LIBRARY to C:/MinGW/bin/zlib1.dll
#check build_examples
#check with_asf and with_mp4
#In CMAKE_EXE_LINKER_FLAGS and CMAKE_SHARED_LINKER_FLAGS add the following text: -Wl,--enable-auto-import
#press configure again
#press generate. in the build directory you choose, type mingw32-make, and then mingw32-make install
#you can now delete the taglib source and binaries
#build qt static: configure -static -no-exceptions; mingw32-make sub-src
