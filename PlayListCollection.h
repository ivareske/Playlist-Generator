#ifndef PLAYLISTCOLLECTION_H
#define PLAYLISTCOLLECTION_H

#include <QtGui>
#include "PlayList.h"
#include "globalFunctions.h"

class PlayListCollection
{
public:
    PlayListCollection( const QString &name="" );
    QString name() const;
    void setName( const QString &name );
    QList<PlayList> playLists() const;
    void setPlayLists( const QList<PlayList> &playLists );
    static QString defaultCollectionName();
    operator QVariant () const;
    bool operator==(const PlayListCollection &other) const;

private:
    QString name_;
    QList<PlayList> playLists_;

};

Q_DECLARE_METATYPE(PlayListCollection);

QDataStream &operator>>(QDataStream &in, PlayListCollection &p);
QDataStream &operator<<(QDataStream &out, const PlayListCollection &p);

#endif // PLAYLISTCOLLECTION_H
