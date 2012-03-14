#ifndef SCRIPTWRAPPERS_H
#define SCRIPTWRAPPERS_H

#include "GlobalFunctions.h"
#include "metaTypes.h"
#include "Tag.h"
#include "TextViewer.h"

namespace ScriptWrappers{

QStringList scriptArrayToStringList( const QScriptValue &array );
QScriptValue scriptGetDirContent(QScriptContext *context, QScriptEngine *engine);
QScriptValue scriptCopyFiles(QScriptContext *context, QScriptEngine *engine);
QScriptValue writeFile(QScriptContext *context, QScriptEngine *engine);
QScriptValue scriptContains(QScriptContext *context, QScriptEngine *engine);
QScriptValue scriptUnique(QScriptContext *context, QScriptEngine *engine);
QScriptValue scriptRandomize(QScriptContext *context, QScriptEngine *engine);
QScriptValue scriptRelativeTo(QScriptContext *context, QScriptEngine *engine);
QScriptValue toQFileInfo(QScriptEngine *e, const QFileInfo &info);
void fromQFileInfo(const QScriptValue &v, QFileInfo &info);
QScriptValue constructTag(QScriptContext *context, QScriptEngine *engine);
void fromStringStringListHash(const QScriptValue &v, QHash<QString,QStringList> &hash);
QScriptValue toStringStringListHash(QScriptEngine *e, const QHash<QString,QStringList> &hash );


}

#endif // SCRIPTWRAPPERS_H
