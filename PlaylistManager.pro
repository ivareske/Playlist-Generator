#generate other playlist formats?
#sjekk at frames/items leses rett
#lag noe hjelpetekst for scripting og evt. annet
#test redirection of clog/cout. what about qDebug(), is that redirected when cout is?
#fix line number in error for common script
#replace all usage of \n with Global::newLine()?
#fix findreplacedialog linking for unix? use subdirs template to build qtfindreplacedialog?
#go through all qDebug usage, remove unecessary ones
#add sorting of playlist
#make installer for msvc

#TEMPLATE = subdirs
#SUBDIRS += ./iRes
#SUBDIRS += ./SignalProcessorDialog


# The application version
VERSION = 1.0
#NAME = $$quote(Playlist Generator)
TARGET = PlayListGenerator


TEMPLATE=app
CONFIG += qt

#CONFIG += console
QT += script
QT += scripttools

SUFFIX_STR =
LIB_PATH = ./release
DESTDIR = ./release
CONFIG(debug, debug|release) {
    SUFFIX_STR = d
    LIB_PATH = ./debug
    DESTDIR = ./debug
}

win32{
    INCLUDEPATH  += ./taglib ./
    win32-g++ {
        LINKEXT = .dll
    }
    win32-msvc*{
        LINKEXT = .lib
    }
    LIBS += $$LIB_PATH/tag$${LINKEXT}
    LIBS += $$LIB_PATH/qtfindreplacedialog$${LINKEXT}
}
unix{
    INCLUDEPATH += /usr/include/taglib/
    LIBS += -L/usr/lib/ -ltag
    LIBS += -L/usr/lib/ -lqtfindreplacedialog    #check that this works
}

HEADERS = PlaylistManager.h SettingsDialog.h PlayList.h GlobalFunctions.h M3uEntry.h AddFilesDialog.h TextViewer.h Tag.h \
    PlayListCollection.h StyleSheetEditor.h \
    SimpleTextEdit.h \
    PMSettings.h \
    ScriptWrappers.h \
    metaTypes.h CodeEditor.h \
    ScriptEngine.h QDebugStream.h scriptsyntaxhighlighter.h ReplaceInFileNameDialog.h \
    tabwidget.h

FORMS   = PlaylistManager.ui SettingsDialog.ui AddFilesDialog.ui TextViewer.ui StyleSheetEditor.ui \
    ReplaceInFileNameDialog.ui

SOURCES = main.cpp PlaylistManager.cpp  SettingsDialog.cpp PlayList.cpp GlobalFunctions.cpp M3uEntry.cpp AddFilesDialog.cpp TextViewer.cpp Tag.cpp \
    PlayListCollection.cpp StyleSheetEditor.cpp \
    SimpleTextEdit.cpp \
    PMSettings.cpp \
    ScriptWrappers.cpp CodeEditor.cpp \
    ScriptEngine.cpp scriptsyntaxhighlighter.cpp  ReplaceInFileNameDialog.cpp \
    tabwidget.cpp

RC_FILE = PlaylistManager.rc
RESOURCES += PlaylistManager.qrc


DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"



#to build taglib (windows): download taglib source from
#http://developer.kde.org/~wheeler/taglib.html
#Download the Zlib source (zlib125.zip) and unpack it with unzip or 7zip. Then, compile Zlib like this:
#cd zlib-1.2.5/ 
#mingw32-make -f win32/Makefile.gcc
#or from visual studio command prompt:
#nmake -f win32/Makefile.msc
#install cmake, choose where the source code is (e.g. C:\taglib-1.6.3) and where to build
#for visual studio, first open a nmake command prompt (as administrator! If not, "nmake install" will fail)
#and start the cmake gui from there. I also had to remove mingw from path first.
#If not, cmake would try to use some mingw executables.
#press configure, choose "nmake makefiles" or "mingw makefiles" and "use default native compilers"
#toggle advanced view
#set CMAKE_INSTALL_DIR to where you want to install, and set ZLIB_INCLUDE_DIR to the main zlib dir
#and ZLIB_LIBRARY to zlib1.dll for mingw, or zlib.lib for nmake.
#check build_examples to build examples.
#check with_asf and with_mp4 (not possible anymore?)
#(Not needed anymore?)In CMAKE_EXE_LINKER_FLAGS and CMAKE_SHARED_LINKER_FLAGS add the following text: -Wl,--enable-auto-import
#press configure again
#press generate. in the build directory you choose, type mingw32-make, and then mingw32-make install (or nmake, nmake install)
#you can now delete the taglib source and binaries
