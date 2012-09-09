#ifndef M3UENTRY_H
#define M3UENTRY_H

#include <QtGui>
#include "GlobalFunctions.h"
#include "Tag.h"

class PlayListEntry {

    public:
        PlayListEntry( const Tag *tag, const QString &format );
        PlayListEntry( const QFileInfo &file );

        QString extInf() const;
        QFileInfo file() const;
        //void setExtInf(const QString& extInf);
        //void setFile(const QFileInfo& file);
        //void setTag( const Tag *tag );
        bool operator==(const PlayListEntry& e1) const;

    private:
        QString createExtInfString(const Tag* tag, const QString& format_) const;
        QString extInf_;        
        //QFileInfo playListEntryFile_;
        QFileInfo file_;

};

uint qHash(const PlayListEntry& key);


#endif
