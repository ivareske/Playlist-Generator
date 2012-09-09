#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

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
#include "GlobalFunctions.h"
#include <QtScript>


class PlayListItem : public QListWidgetItem {

    public:
        PlayListItem(const QString& name = "New playlist", QListWidget* parent = 0);
        //~PlayList();
        //PlayList( const PlayList &other );
        bool generate(QList<M3uEntry> *songsOut, QString* log, QHash<QString, Tag*> *tags);
        void copyFoundFiles(QList<M3uEntry> songs, QString* log);        

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
        QString playListEntry(const M3uEntry& e) const;
        QString copyFilesName(const M3uEntry& e);


        void setName(const QString& name);
        void setRules(const QVector<Rule> &rules);
        void setFolders(const QList<QDir> &folders);
        void setExtensions(const QStringList& extensions);
        void setRandomize(bool randomize);
        void setIncludeSubFolders(bool includeSubFolders);
        void setRelativePath(bool setRelativePath);
        void setAllRulesTrue(bool allRulesTrue);
        void setIncludeExtInf(bool includeExtInf);
        void setMakeUnique(bool makeUnique);
        void setCopyFilesToDir(const QDir& copyFilesToDir);
        void setCopyFiles(bool copyFiles);
        void setIndividualFiles(const QList<QFileInfo> &individualFiles);
        void setScript(const QString& script);

        operator QVariant() const;
        bool operator==(const PlayListItem& other) const;

    private:

        QList<M3uEntry> findFiles(bool* canceled, QString* log, QHash<QString, Tag*> *tags);
        void checkRange(const QVector<int> &intvals, int value, bool* allOk, bool* anyOk, bool shouldBeTrue) const;
        bool writeM3U(const QString& file, const QList<M3uEntry> &songs, QString* log = 0) const;
        void checkRule(bool value, bool* allOk, bool* anyOk, bool shouldBeTrue) const;
        QList<M3uEntry> processFile(const QFileInfo& fileInfo, bool hasTagRule, bool hasAudioRule,bool keepTags, const QString &format, bool useScript, QString* log, QHash<QString, Tag*> *tags, QHash<QString, Tag*> *tagsCopy, bool *wasCanceled) const;
        QString createExtInfString(const Tag* tag, const QString& file, const QString& format_) const;
        void evaluateRules(const Tag* tag, const QString& file, bool* allOk, bool* anyOk) const;
        bool evaluateScript( const Tag* tag, const QFileInfo& fileInfo, QString *log ) const;
        static QScriptValue contains(QScriptContext *context, QScriptEngine *engine);
        static QScriptValue myAdd(QScriptContext *context, QScriptEngine *engine);

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
        bool showTaglibDebug;
        QDir copyFilesToDir_;
        bool copyFiles_;
        QList<QFileInfo> individualFiles_;
        QString script_;        
        //QDir outPutFolder_;        
        PMSettings* guiSettings;        
        std::stringstream *buffer;
        std::streambuf *sbuf;

        QHash<QString,QVariant> frameFields;
        bool artistEmpty,titleEmpty,albumEmpty,commentEmpty,genreEmpty,trackEmpty,yearEmpty;




};

Q_DECLARE_METATYPE(PlayListItem);

QDataStream& operator>>(QDataStream& in, PlayListItem& p);
QDataStream& operator<<(QDataStream& out, const PlayListItem& p);


#endif

