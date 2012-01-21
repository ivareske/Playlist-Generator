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

QScriptValue toStringStringListHash(QScriptEngine *e, const QHash<QString,QStringList> &hash ) {
    qDebug()<<"toStringStringListHash";

    QStringList keys = hash.keys();
    QScriptValue array = e->newArray(keys.size());
    for (int i=0;i<keys.size();i++){
        QStringList list = hash[keys[i]];
        QScriptValue array2 = e->newArray(list.size());
        for (int k=0;k<list.size();k++){
            array2.setProperty(k, QScriptValue(e, list[k]));
        }
        qDebug()<<"toStringStringListHash "<<keys[i]<<list;
        array.setProperty(keys[i], array2);
    }
    return array;


}
void fromStringStringListHash(const QScriptValue &v, QHash<QString,QStringList> &hash) {
    qDebug()<<"fromStringStringListHash";

    //QHash<QString,QVector<double> > hash;
    QScriptValueIterator it(v);
    while(it.hasNext()){
        it.next();
        //qDebug() << it.name();// << ": " << it.value().toVariant();
        QScriptValue v2 = it.value();
        if(v2.isArray()){
            QStringList vector;
            qScriptValueToSequence(v2,vector);
            hash.insert(it.name(),vector);
        }
    }
    //return hash;

    /*QVariant var = v.toVariant();
    qDebug()<<"fromStringStringListHash "<<var;
    QHash<QString,QVariant> hash2 = var.toHash();
    QStringList keys = hash2.keys();
    for(int i=0;i<keys.size();i++){
        hash.insert(keys[i],hash2[keys[i]].toStringList());
    }
    */

}


//http://www.qtcentre.org/threads/33355-Use-QObject-with-QtScript
QScriptValue constructTag(QScriptContext *context, QScriptEngine *engine){
    if (!context->isCalledAsConstructor()){
        return context->throwError(QScriptContext::SyntaxError, "please use the 'new' operator");
    }
    if(context->argumentCount()!=1 ){
        return context->throwError("1 argument required (Full filename/string) ");
    }
    QString file = context->argument(0).toString();
    // store the shared pointer in the script object that we are constructing
    //return engine->newVariant(context->thisObject(), qVariantFromValue(new Tag(file)));
    Tag *tag = new Tag(file);
    return engine->newQObject(tag);
}



}
