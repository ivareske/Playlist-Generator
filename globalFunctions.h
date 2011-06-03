#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <QtGui>
#include "Rule.h"
#include "M3uEntry.h"
#include "PlayList.h"

class PlayList;

namespace Global{

    QSettings* guiSettings();
    bool checkIntValue( QVector<int> *intvals, const QString &val );
    bool stringLessThan( const QString &s1, const QString &s2 );
    int naturalCompare( const QString &_a, const QString &_b, Qt::CaseSensitivity caseSensitivity );

    const QString version = "1.0";
    enum fileReadResult{ DOESNOTEXIST, EXISTS, NOTVALID };
    const QString ext = ".ini"; //extension of QSettings files
    //const QString defaultExt = "*.mp3;*.wma;*.wav;*.ogg;*.aac;*.ac3;*.m4a";


}

uint qHash(const M3uEntry &key);
uint qHash( const QFileInfo &key );

#endif
