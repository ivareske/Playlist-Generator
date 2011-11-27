#ifndef METATYPES_H
#define METATYPES_H

#include <QtGui>
#include <QtScript>


typedef QHash<QString,QStringList> FrameList;
Q_DECLARE_METATYPE(FrameList)
typedef QHash<QString,FrameList> FrameListList;
Q_DECLARE_METATYPE(FrameListList)
Q_DECLARE_METATYPE(QStringList)
Q_DECLARE_METATYPE(QFileInfo)
Q_DECLARE_METATYPE(QList<QFileInfo>)

#endif // METATYPES_H
