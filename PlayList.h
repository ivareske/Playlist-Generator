#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QtGui>
#include "Tag.h"
#include <tag.h>
#include <taglib.h>
#include <fileref.h>
#include <vector>
#include <time.h>
#include <cstdlib>
#include "M3uEntry.h"
#include "GlobalFunctions.h"
#include <QtScript>
#include "ScriptEngine.h"
#include "QDebugStream.h"


class PlayList : public QListWidgetItem {

    public:
        PlayList(const QString& name = "New playlist", QListWidget* parent = 0);
        ~PlayList();
        PlayList(const PlayList &other);
        PlayList & operator=(const PlayList &other);
        bool generate(QList<M3uEntry> *songsOut, QString* log, QHash<QString, Tag*> *tags);        

        QString name() const;
        QList<QDir> folders() const;
        QStringList extensions() const;
        bool randomize() const;
        bool includeSubFolders() const;
        bool relativePath() const;
        bool includeExtInf() const;
        bool makeUnique() const;
        QDir copyFilesToDir() const;
        bool copyFiles() const;
        QList<QFileInfo> individualFiles() const;
        QString script() const;
        QString playListEntry(const M3uEntry& e) const;


        void setName(const QString& name);        
        void setFolders(const QList<QDir> &folders);
        void setExtensions(const QStringList& extensions);
        void setRandomize(bool randomize);
        void setIncludeSubFolders(bool includeSubFolders);
        void setRelativePath(bool setRelativePath);
        void setIncludeExtInf(bool includeExtInf);
        void setMakeUnique(bool makeUnique);
        void setCopyFilesToDir(const QDir& copyFilesToDir);
        void setCopyFiles(bool copyFiles);
        void setIndividualFiles(const QList<QFileInfo> &individualFiles);
        void setScript(const QString& script);

        operator QVariant() const;
        bool operator==(const PlayList& other) const;

    private:

        QList<M3uEntry> findFiles(bool* canceled, QString* log, QHash<QString, Tag*> *tags);        
        bool writeM3U(const QString& path, const QString &name, const QList<M3uEntry> &songs, QString* log = 0) const;
        QList<M3uEntry> processFile(const QFileInfo& fileInfo, bool keepTags, const QString &format, QString* log, QHash<QString, Tag*> *tags, QHash<QString, Tag*> *tagsCopy, bool *wasCanceled) const;
        QString createExtInfString( Tag* tag, const QString& file, const QString& format_) const;        
        bool evaluateScript(Tag *tag, const QFileInfo& fileInfo, QString *log, QString *extInf , QString *sortBy) const;

        //QString name_;
        QList<QDir> folders_;
        QStringList extensions_;
        bool randomize_;
        bool includeSubFolders_;
        bool relativePath_;
        bool includeExtInf_;
        bool makeUnique_;
        QDir copyFilesToDir_;
        bool copyFiles_;
        QList<QFileInfo> individualFiles_;
        QString script_;
        PMSettings* guiSettings;        




};

Q_DECLARE_METATYPE(PlayList)

QDataStream& operator>>(QDataStream& in, PlayList& p);
QDataStream& operator<<(QDataStream& out, const PlayList& p);


#endif

