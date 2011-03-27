#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <time.h> 
#include <QtGui>
//#include <QtScript>
#include "structs.h"
#include "rule.h"
#include "playList.h"
#include "globalFunctions.h"
#include "settingsDialog.h"
#include "addFilesDialog.h"
#include "textViewer.h"
#include "ruleDialog.h"
#include "settingsClass.h"
#include "ui_playlistManager.h"



class playlistManager : public QMainWindow, private Ui::playListGenerator
{
    Q_OBJECT
 
public:

    playlistManager(QWidget *parent = 0);
	
public slots:

	void addIndividualFiles();
	void createActions();
	fileReadResult readSettings( QFileInfo file );
	void saveSettings( QFileInfo file );
	void saveSettingsAs();
	void saveCurrentSettings();
	//void generatePlayList( int ind );
	void generateAllPlayLists();
	void generateSelectedPlayLists();
	void generatePlayLists( QList<int> inds );
	void addPlayList();
	void removePlayList();
	void changeFolder(QListWidgetItem *item);
	bool loadStyleSheet( QString file );
	void newRule();
	void editRule();
	void removeRule();
	void renamePlayList(QListWidgetItem *item);
	void showRulesAndFolders();
	void clearRulesAndFolders();
	void showSettings();
	//void writeM3U( QString file, m3uList songs );
	void removeFolder();
	void renameFolder(QListWidgetItem *item);
	void addFolder();
	void sortPlayLists();
	void setGUIStyle( const QString &style );
	void open();
	void newCollection();
	void openStyleSheet();
	void updateExtensions( const QString & exts );
	void updateRandomize( int state );
	void updateAllRulesTrue( int state );
	void updateSearchSubfolders( int state );
	void updateIncludeExtInf( int state );
	void updateUseRelativePath( int state );
	void updateMakeUnique( int state );
	void giveUniqueIds();
	void initialize( QFileInfo file );
	void writeGUISettings();
	void readGUISettings();
	void uncheckStyleActions();
	void checkStyleAction( QString actionText, bool state );
	void getCopyDir();
	void updateCopyFiles( const QString &text );
	void updateCopyTo( int state );
	void copyFiles( playList p, QList<m3uEntry> songs, QString *log );
	void enableOptions( bool state );
	void clearTags();
	void updateScript();
	void runscript();

	
	
protected:
	void closeEvent( QCloseEvent *event );
	
private:
	QString style;
	QFileInfo settingsFile;
	QSettings *guiSettings;
	bool artistEmpty;
	bool titleEmpty;
	bool albumEmpty;
	bool commentEmpty;
	bool genreEmpty;
	bool trackEmpty;
	bool yearEmpty;
	QString defaultOutputFolder;	
	QStringList defaultExtensions;	
	QString format;
	bool useScript;
	//bool openmp;
	bool showLog;
	bool ShowTagLibDebug;
	bool useCopyFilesToPath;
	bool keepTags;
	QString copyFilesToDir;
	QSettings *playListSettings;
	QList<playList> playLists;	
	QHash<QString, Tag> tags;
	QSize textViewerSize;
};
 
 
#endif






