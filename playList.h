#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QtGui>
//#include <QtScript>
#include "rule.h"
#include "Tag.h"
#include <tag.h>
#include <taglib.h>
#include <fileref.h>
//#include <DebugLogger.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
//#include "omp.h"
#include <cstdlib>
#include "m3uEntry.h"
#include "settingsClass.h"
#include "globalFunctions.h"


class playList{

public:
	playList( QStringList defaultExtensions );
	bool generate( QList<m3uEntry> *songsOut, QStatusBar *s, QString *log, QHash<QString,Tag> *tags );	
	
	QString name;
	QVector<rule> rules;
	QStringList folders;
	QStringList extensions;
	bool randomize;
	bool includeSubFolders;
	bool relativePath;
	bool allRulesTrue;
	bool includeExtInf;
	bool makeUnique;
	int uniqueId;
	QString copyFilesToDir;
	bool copyFiles;
	QStringList individualFiles;
	QString script;
	
private:
	
	QSettings *guiSettings;
	QString format;
	QString outputfolder;
	bool artistEmpty;
	bool titleEmpty;
	bool albumEmpty;
	bool commentEmpty;
	bool genreEmpty;
	bool trackEmpty;
	bool yearEmpty;
	bool useCopyFilesToPath;	
	bool useScript;
	bool keepTags;
	bool ShowTagLibDebug;
	QStringList scriptvariables;
	
	void getGuiSettings();
	QList<m3uEntry> findFiles( QStatusBar *s, bool *canceled, QString *log, QHash<QString,Tag> *tags );	
	QList<QFileInfo> getDirContent( QString& aPath );
	void checkRange( QVector<int> intvals, int tmp, bool *allOk, bool *anyOk, bool shouldBeTrue );
	void writeM3U( QList<m3uEntry> songs );	
	void checkRule( bool ruleCheck, bool *allOk, bool *anyOk, bool shouldBeTrue );
	QList<m3uEntry> processFile( bool *wasCanceled, QFileInfo fileInfo, QStatusBar *s, bool hasTagRule, bool hasAudioRule, QString *log, QHash<QString,Tag> *tags, QHash<QString,Tag> *tagscopy );
	QString createExtinfString( Tag tag, QString file, QString format );
	void evaluateRules( Tag tag, QString file, bool *allOk, bool* anyOk );

};


 
#endif

