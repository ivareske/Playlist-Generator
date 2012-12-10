#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <time.h>
#include <stdio.h>
#include <string>
#include <QtGui>
#include "PlayList.h"
#include "GlobalFunctions.h"
#include "SettingsDialog.h"
#include "AddFilesDialog.h"
#include "TextViewer.h"
#include "PlayListCollection.h"
#include "StyleSheetEditor.h"
#include "SimpleTextEdit.h"
#include "PMSettings.h"
#include "ScriptWrappers.h"
#include "metaTypes.h"
#include "ui_PlaylistManager.h"
#include "ScriptEngine.h"
#include <QtScriptTools/QtScriptTools>
#include "QDebugStream.h"
#include "CodeEditor.h"
#include "tabwidget.h"



class PlaylistManager : public QMainWindow, private Ui::playListGenerator {
        Q_OBJECT

    public:

    PlaylistManager(QWidget* parent = 0);
    ~PlaylistManager();


public slots:

    bool runScript(const QString &script, const QString &commonScript, const QString &scriptName="", bool guiMode=true);
    protected:
        void closeEvent(QCloseEvent* event);

    private slots:

        void runAllScriptEditScripts();
        void checkIfCollectionChanged();
        void scriptEdited();
        void clearScriptOutput();
        void runScriptEditScript();
        void saveCollectionCheck();
        void editStyleDialog();
        void initGuiSettings();
        void showAbout();
        QString newUniqePlayListName();
        QString newUniqePlayListCollectionName( const QDir &dir );
        void guiModeChanged();
        void addIndividualFiles();
        void createActions();
        void loadCollection(const QFileInfo& file);
        void saveCollection(bool checkExistence = true);
        void updateCollection();
        void saveCollectionAs();
        void generateAllPlayLists();
        void generateSelectedPlayLists();
        void generatePlayLists( const QList<PlayList*> &playLists );
        PlayList* addPlayList( QString name="" );
        void removePlayList();

        void showRulesAndFolders();
        void clearRulesAndFolders();
        void blockPlayListSignals(bool block);
        void showSettings();
        //void removeFolder();
        //void renameFolder(QListWidgetItem* item);
        //void changeFolder(QListWidgetItem* item);
        void addFolder();
        void open();
        void newCollection();
        void initialize();
        void writeGUISettings();
        void readGUISettings();

        void getCopyDir();
        void enableOptions(bool state);
        void clearTags();
        void updatePlayList();
        QList<QDir> selectFolders( QAbstractItemView::SelectionMode mode = QAbstractItemView::ExtendedSelection );


    private:                
        void initializeScriptEngine();

        PlayList* playListItem(int row);
        SimpleTextEdit *folderTable;        
        CodeEditor *RuleScript;
        PlayList* currentPlayList();
        PlayListCollection collection_, lastSavedCollection_;
        PMSettings* guiSettings;
        QHash<QString, Tag*> tags_;
        ScriptEngine engine_;
        QScriptEngineDebugger scriptDebugger_;
        QTextEdit *console_;        
        QTimer checkIfCollectionChangedTimer_;
        TabWidget *tabWidget;
        QScriptEngineDebugger *debugger;
        QMainWindow *dmw;

};


#endif






