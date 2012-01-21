#ifndef SCRIPTWRAPPERS_H
#define SCRIPTWRAPPERS_H

#include "GlobalFunctions.h"
#include "metaTypes.h"
#include "Tag.h"

namespace ScriptWrappers{

QStringList scriptArrayToStringList( const QScriptValue &array );
QScriptValue scriptGetDirContent(QScriptContext *context, QScriptEngine *engine);
QScriptValue toQFileInfo(QScriptEngine *e, const QFileInfo &info);
void fromQFileInfo(const QScriptValue &v, QFileInfo &info);
QScriptValue constructTag(QScriptContext *context, QScriptEngine *engine);
void fromStringStringListHash(const QScriptValue &v, QHash<QString,QStringList> &hash);
QScriptValue toStringStringListHash(QScriptEngine *e, const QHash<QString,QStringList> &hash );


}

#endif // SCRIPTWRAPPERS_H
