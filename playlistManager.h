#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <time.h> 
#include <QtGui>
#include "Rule.h"
#include "PlayList.h"
#include "globalFunctions.h"
#include "SettingsDialog.h"
#include "AddFilesDialog.h"
#include "TextViewer.h"
#include "RuleDialog.h"
#include "PlayListCollection.h"
#include "StyleSheetEditor.h"
#include "ui_playlistManager.h"



class PlaylistManager : public QMainWindow, private Ui::playListGenerator
{
    Q_OBJECT

public:

    PlaylistManager(QWidget *parent = 0);

protected:
    void closeEvent( QCloseEvent *event );

private slots:
    void editStyleDialog();
    void initGuiSettings();
    void showAbout();
    QString newUniqePlayListName();
    void updateUseScript();
    void addIndividualFiles();
    void createActions();
    void loadCollection( const QFileInfo &file );
    void saveCollection( bool checkExistence=true );
    void updateCollection();
    void saveCollectionAs();
    void generateAllPlayLists();
    void generateSelectedPlayLists();
    void generatePlayLists( const QList<int> &inds );
    void addPlayList();
    void removePlayList();
    void changeFolder(QListWidgetItem *item);

    void newRule();
    void editRule();
    void removeRule();
    void showRulesAndFolders();
    void clearRulesAndFolders();
    void blockPlayListSignals( bool block );
    void showSettings();
    void removeFolder();
    void renameFolder(QListWidgetItem *item);
    void addFolder();    
    void open();
    void newCollection();    
    void initialize();
    void writeGUISettings();
    void readGUISettings();
    /*
    void uncheckStyleActions();
    void checkStyleAction( const QString &actionText, bool state );
    bool loadStyleSheet( const QString &file );
    void setGUIStyle( const QString &style );
    void openStyleSheet();
    */
    void getCopyDir();
    void enableOptions( bool state );
    void clearTags();
    void updatePlayList();
    QList<QDir> selectFolders();


private:
    PlayList* playListItem( int row );

    PlayList* currentPlayList();

    //QFileInfo playListCollectionFile;
    PlayListCollection collection_,lastSavedCollection_;
    QSettings* guiSettings;
    //QSettings *playListCollection;
    //QList<PlayList> playLists_;
    QHash<QString, Tag> tags_;    

};


//Q_DECLARE_METATYPE(QList<PlayList>)

#endif






