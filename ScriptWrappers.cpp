#include "ScriptWrappers.h"

namespace ScriptWrappers{

QStringList scriptArrayToStringList( const QScriptValue &array ){

    QStringList list;
    int length = array.property("length").toInteger();
    for(int i = 0; i < length; i++){
        list<<array.property(i).toString();
    }
    return list;
}

QScriptValue scriptGetDirContent(QScriptContext *context, QScriptEngine *engine){

    QList<QFileInfo> info;
    if (context->argumentCount()!=3 ){
        return context->throwError("3 arguments required. path (string), includeSubFolders (bool) and extensions (array) ");
    }
    QString path = context->argument(0).toString();
    bool includeSubFolders = context->argument(1).toBool();
    QScriptValue array = context->argument(2);
    QStringList extensions = scriptArrayToStringList( array );
    info = Global::getDirContent( path, includeSubFolders, extensions );

    return engine->toScriptValue(info);
}


QScriptValue toQFileInfo(QScriptEngine *e, const QFileInfo &info) {
    return e->newVariant(info.absoluteFilePath());
}
void fromQFileInfo(const QScriptValue &v, QFileInfo &info) {
    info = QFileInfo(v.toString());
}


QScriptValue constructTag(QScriptContext *context, QScriptEngine *engine){
    if (!context->isCalledAsConstructor()){
        return context->throwError(QScriptContext::SyntaxError, "please use the 'new' operator");
    }
    if(context->argumentCount()!=1 ){
        return context->throwError("1 argument required (Full filename/string) ");
    }
    QString file = context->argument(0).toString();
    // store the shared pointer in the script object that we are constructing
    return engine->newVariant(context->thisObject(), qVariantFromValue(new Tag(file)));
}



}
