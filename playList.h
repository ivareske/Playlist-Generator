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


class playList : public QListWidgetItem{

public:
    playList( const QStringList &defaultExtensions = QStringList(), QListWidget *parent = 0 );
    playList( const playList &other );
    bool generate( QList<m3uEntry> *songsOut, QStatusBar *s, QString *log, QHash<QString,Tag> *tags, const settingsClass &settings );
    void copyFoundFiles( QList<m3uEntry> songs, QString *log );

    QString fileNameWithoutExtension() const;
    QString name() const;
    QVector<rule> rules() const;
    QList<QDir> folders() const;
    QStringList extensions() const;
    bool randomize() const;
    bool includeSubFolders() const;
    bool relativePath() const;
    bool allRulesTrue() const;
    bool includeExtInf() const;
    bool makeUnique() const;    
    QDir copyFilesToDir() const;
    bool copyFiles() const;
    QList<QFileInfo> individualFiles() const;
    QString script() const;
    QDir outPutFolder() const;

    void setName( const QString &name );
    void setRules( const QVector<rule> &rules );
    void setFolders( const QList<QDir> &folders );
    void setExtensions( const QStringList &extensions );
    void setRandomize( bool randomize );
    void setIncludeSubFolders( bool includeSubFolders );
    void setRelativePath( bool setRelativePath );
    void setAllRulesTrue( bool allRulesTrue );
    void setIncludeExtInf( bool includeExtInf );
    void setMakeUnique( bool makeUnique );    
    void setCopyFilesToDir( const QDir &copyFilesToDir );
    void setCopyFiles( bool copyFiles );
    void setIndividualFiles( const QList<QFileInfo> &individualFiles );
    void setScript( const QString &script );
    void setOutPutFolder( const QDir &outPutFolder );

    operator QVariant () const;

private:

    QList<m3uEntry> findFiles( QStatusBar *s, bool *canceled, QString *log, QHash<QString,Tag> *tags );
    QList<QFileInfo> getDirContent( const QString& aPath );
    void checkRange( QVector<int> intvals, int tmp, bool *allOk, bool *anyOk, bool shouldBeTrue );
    bool writeM3U( QList<m3uEntry> songs, QString *log=0 );
    void checkRule( bool ruleCheck, bool *allOk, bool *anyOk, bool shouldBeTrue );
    QList<m3uEntry> processFile( bool *wasCanceled, QFileInfo fileInfo, QStatusBar *s, bool hasTagRule, bool hasAudioRule, QString *log, QHash<QString,Tag> *tags, QHash<QString,Tag> *tagscopy );
    QString createExtinfString( Tag tag, QString file, QString format );
    void evaluateRules( Tag tag, QString file, bool *allOk, bool* anyOk );

    settingsClass settings_;


    //QString name_;
    QVector<rule> rules_;
    QList<QDir> folders_;
    QStringList extensions_;
    bool randomize_;
    bool includeSubFolders_;
    bool relativePath_;
    bool allRulesTrue_;
    bool includeExtInf_;
    bool makeUnique_;
    int uniqueId_;
    QDir copyFilesToDir_;
    bool copyFiles_;
    QList<QFileInfo> individualFiles_;
    QString script_;
    QDir outPutFolder_;
    QStringList scriptVariables_;

    /*
 QSettings *guiSettings;
 QString format;

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

 void getGuiSettings();
        */


};

Q_DECLARE_METATYPE(playList);

QDataStream &operator>>(QDataStream &in, playList &p);
QDataStream &operator<<(QDataStream &out, const playList &p);

#endif

