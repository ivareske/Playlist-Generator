#ifndef M3UENTRY_H
#define M3UENTRY_H

#include <QtGui>
#include "GlobalFunctions.h"

class M3uEntry {

    public:
        M3uEntry();

        QString extInf() const;
        QFileInfo originalFile() const;
        void setExtInf(const QString& extInf);
        void setOriginalFile(const QFileInfo& originalFile);
        void setSortBy( const QString &sortBy );
        QString sortBy() const;

        bool operator==(const M3uEntry& e1) const;

    private:
        QString extInf_,sortBy_;
        //QString file_;
        QFileInfo playListEntryFile_;
        QFileInfo originalFile_;

};

bool m3uEntryLessThan(const M3uEntry& s1, const M3uEntry& s2);
uint qHash(const M3uEntry& key);


#endif
