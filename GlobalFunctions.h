#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <QtGui>
#include <QtScript>
#include <PMSettings.h>
#include "M3uEntry.h"
#include <iostream>

namespace Global {

    int copyFoundFiles(const QStringList &files, const QString &copyFilesToDir, bool keepFolderStructure=false, bool overWrite=true, QString* log=0) ;
    QList<QFileInfo> getDirContent(const QString& aPath, const QStringList &extensions, bool includeSubFolders=true, bool hiddenFiles=true, bool *canceled=0);
    QString timeString( uint milliSeconds, const QString &format="hh:mm:ss:z" );
    QString versionCheck(QDataStream* in, bool* ok, QString* log);
    PMSettings* guiSettings(QObject * parent = 0);
    bool checkIntValue(QVector<int> *intvals, const QString& val);
    bool stringLessThan(const QString& s1, const QString& s2);
    int naturalCompare(const QString& _a, const QString& _b, Qt::CaseSensitivity caseSensitivity);

    //const QString version = "1.0";
    enum fileReadResult { DOESNOTEXIST, EXISTS, NOTVALID };
    enum scriptType { RULES, SCRIPTANDPLAYLIST, SCRIPTONLY };
    const QString ext = ".ini"; //extension of PMSettings files
    //const QString defaultExt = "*.mp3;*.wma;*.wav;*.ogg;*.aac;*.ac3;*.m4a";


}

uint qHash(const QFileInfo& key);
int qHash(const QDir& dir);


#endif
