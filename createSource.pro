
include(PlaylistManager.pro)

#this creates a zip-file with the source code!


#whitespace separated list of files to be included in source!
FILES = $$SOURCES $$HEADERS $$FORMS $$RESOURCES $$RC_FILE taglib/*.* libtag.dll installer/iRes.nsi *.pro installer/playListGenerator.nsi playlist4.ico

system( del PlaylistManager_($$APP_VERSION)_src.zip )
SYSTEM_CMD = $$quote(7za a -tzip PlaylistManager_($$APP_VERSION)_src.zip $$FILES)
system( $$SYSTEM_CMD )
#message($$FILES)
