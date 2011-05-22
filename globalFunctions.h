#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <QtGui>
#include "Rule.h"
#include "M3uEntry.h"
#include "PlayList.h"

class PlayList;

namespace Global{

    QString getRuleName( const Rule::ruleType &type );
    QString getRuleExample( const Rule::ruleType &type );
    bool checkIntValue( QVector<int> *intvals, const QString &val );
    bool caseInsensitiveLessThan(const PlayList &s1, const PlayList &s2);

    const QString version = "1.0";
    enum fileReadResult{ DOESNOTEXIST, EXISTS, NOTVALID };
    const QString ext = ".ini"; //extension of QSettings files
    //const QString defaultExt = "*.mp3;*.wma;*.wav;*.ogg;*.aac;*.ac3;*.m4a";


}

uint qHash(const M3uEntry &key);
uint qHash( const QFileInfo &key );

#endif
