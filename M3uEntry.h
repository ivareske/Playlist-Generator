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

        bool operator==(const M3uEntry& e1) const;

    private:
        QString extInf_;
        //QString file_;
        QFileInfo playListEntryFile_;
        QFileInfo originalFile_;

};

uint qHash(const M3uEntry& key);


#endif
