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

TEMPLATE=app
TARGET = playListGenerator
CONFIG += qt release warn_on 
#CONFIG += static
CONFIG += console
#QT += script
 
INCLUDEPATH  += $$quote(C:/Program Files/taglib/include/taglib/)
LIBS += $$quote(C:/Program Files/taglib/bin/libtag.dll)
#LIBS += $$quote(C:/Qt/4.6.3/plugins/script/qtscript_cored.dll)


HEADERS = playListManager.h settingsDialog.h ruleDialog.h playList.h rule.h globalFunctions.h m3uEntry.h addFilesDialog.h textViewer.h structs.h Tag.h settingsClass.h
FORMS   = playlistManager.ui settingsDialog.ui ruleDialog.ui addFilesDialog.ui textViewer.ui
SOURCES = main.cpp playListManager.cpp  settingsDialog.cpp ruleDialog.cpp playList.cpp rule.cpp globalFunctions.cpp m3uEntry.cpp addFilesDialog.cpp textViewer.cpp Tag.cpp settingsClass.cpp

RC_FILE = playListManager.rc



#to build taglib (windows): download taglib source from
#http://developer.kde.org/~wheeler/taglib.html
#add the debuglogger class source file in cmakelists.txt and the debuglogger class header with the install files in the cmakelists.txt, and modify the taglib:.debug function to be as follows:
#void TagLib::debug(const String &s){
#  std::cerr << "TagLib: " << s << std::endl;
# DebugLogger::Instance()->append( "TagLib: " + s.to8Bit() );
#} 
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







