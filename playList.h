#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QtGui>
//#include <QtScript>
#include "Rule.h"
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
#include "M3uEntry.h"
#include "globalFunctions.h"


class PlayList : public QListWidgetItem{

public:
    PlayList( const QString &name = "New playlist", QListWidget *parent = 0 );
    //~PlayList();
    //PlayList( const PlayList &other );
    bool generate( QList<M3uEntry> *songsOut, QString *log, QHash<QString,Tag> *tags );
    void copyFoundFiles( QList<M3uEntry> songs, QString *log );

    QString fileNameWithoutExtension() const;
    QString name() const;
    QVector<Rule> rules() const;
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
    QString playListEntry( const M3uEntry &e ) const;


    void setName( const QString &name );
    void setRules( const QVector<Rule> &rules );
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

    operator QVariant () const;
    bool operator==(const PlayList &other) const;

private:

    QList<M3uEntry> findFiles( bool *canceled, QString *log, QHash<QString,Tag> *tags ) const;
    QList<QFileInfo> getDirContent( const QString& aPath ) const;
    void checkRange( const QVector<int> &intvals, int value, bool *allOk, bool *anyOk, bool shouldBeTrue ) const;
    bool writeM3U( const QString &file, const QList<M3uEntry> &songs, QString *log=0 ) const;
    void checkRule( bool value, bool *allOk, bool *anyOk, bool shouldBeTrue ) const;
    QList<M3uEntry> processFile( const QFileInfo &fileInfo, bool hasTagRule, bool hasAudioRule, QString *log, QHash<QString,Tag> *tags, QHash<QString,Tag> *tagsCopy ) const;
    QString createExtInfString( const Tag &tag, const QString &file, const QString &format_ ) const;
    void evaluateRules( const Tag &tag, const QString &file, bool *allOk, bool* anyOk ) const;

    //QString name_;
    QVector<Rule> rules_;
    QList<QDir> folders_;
    QStringList extensions_;
    bool randomize_;
    bool includeSubFolders_;
    bool relativePath_;
    bool allRulesTrue_;
    bool includeExtInf_;
    bool makeUnique_;    
    QDir copyFilesToDir_;
    bool copyFiles_;
    QList<QFileInfo> individualFiles_;
    QString script_;
    //QDir outPutFolder_;
    QStringList scriptVariables_;
    QSettings *guiSettings;



};

Q_DECLARE_METATYPE(PlayList);

QDataStream &operator>>(QDataStream &in, PlayList &p);
QDataStream &operator<<(QDataStream &out, const PlayList &p);

#endif

