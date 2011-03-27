#ifndef SETTINGS_CLASS_H
#define SETTINGS_CLASS_H

#include <QtGui>
#include "playList.h"
#include "rule.h"

class playList;

class settingsClass{

public:
	settingsClass();
		
	QString defaultOutputFolder;	
	QString defaultExtensions;	
	QString format;
	//bool openmp;
	bool showLog;
	QList<playList> readSettings( QString settingsFile );
	void saveSettings( QString settingsFile, QList<playList> playLists );	
	QStringList readSettingsStringList( QString settingsFile );
	QStringList createSettingsStringList( QString settingsFile, QList<playList> playLists );
	bool settingsHaveChanged( QString settingsFile, QList<playList> playLists );

	//use filename if the following checked fields are empty
	bool artistEmpty;
	bool titleEmpty;
	bool albumEmpty;
	bool commentEmpty;
	bool genreEmpty;
	bool trackEmpty;
	bool yearEmpty;

};


 
#endif

