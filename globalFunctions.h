#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <QtGui>
#include "rule.h"
#include "m3uEntry.h"
#include "playList.h"

class playList;

namespace Global{

    QString getRuleName( const rule::ruleType &type );
    QString getRuleExample( const rule::ruleType &type );
    bool checkIntValue( QVector<int> *intvals, const QString &val );
    bool caseInsensitiveLessThan(const playList &s1, const playList &s2);

}

uint qHash(const m3uEntry &key);
uint qHash( const QFileInfo &key );

#endif
