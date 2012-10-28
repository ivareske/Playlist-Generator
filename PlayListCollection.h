#ifndef PLAYLISTCOLLECTION_H
#define PLAYLISTCOLLECTION_H

#include <QtGui>
#include <QPair>
#include "PlayList.h"
#include "GlobalFunctions.h"



class PlayListCollection {
    public:
        PlayListCollection(const QString& name = "");
        QList< QPair<QString,QString> > scripts() const;
        void setScripts(const QList<QPair<QString, QString> > &scripts );
        QString name() const;
        void setName(const QString& name);
        QList<PlayList> playLists() const;
        void setPlayLists(const QList<PlayList> &playLists);
        static QString defaultCollectionName();
        void setOutPutPath( const QDir &outPutPath );
        QDir outPutPath() const;
        QString commonScript() const;
        void setCommonScript( const QString &commonScript );
        operator QVariant() const;
        bool operator==(const PlayListCollection& other) const;
        bool operator!=(const PlayListCollection& other) const;

        QList<QPair<QString, QString> > sortedScripts() const;
private:
        QString name_;
        QList<PlayList> playLists_;
        QDir outPutPath_;
        QList< QPair<QString,QString> > scripts_;
        QString commonScript_;

};


Q_DECLARE_METATYPE(PlayListCollection)

QDataStream& operator>>(QDataStream& in, PlayListCollection& p);
QDataStream& operator<<(QDataStream& out, const PlayListCollection& p);

#endif // PLAYLISTCOLLECTION_H
