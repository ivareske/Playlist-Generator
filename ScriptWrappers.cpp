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

QScriptValue scriptRandomize(QScriptContext *context, QScriptEngine *engine){

    int nargin = context->argumentCount();
    if (nargin!=1){
        return context->throwError("1 argument required: QStringList randomize(const QStringList &list)");
    }
    QScriptValue array = context->argument(0);
    QStringList list = scriptArrayToStringList( array );
    std::random_shuffle(list.begin(), list.end());
    QScriptValue arr = engine->newArray(list.size());
    for(int i=0;i<list.size();i++){
        arr.setProperty(i,list[i]);
    }
    return arr;
}

QScriptValue scriptRelativeTo(QScriptContext *context, QScriptEngine *engine){

    int nargin = context->argumentCount();
    if (nargin!=2){
        return context->throwError(QString("2 arguments required: QString relativeTo(const QString &dir,const QString &file)\n")+\
                                   "QStringList relativeTo(const QString &dir,const QStringList &files)");
    }
    QString dir = context->argument(0).toString();
    if(context->argument(1).isArray()){

        QScriptValue array = context->argument(1);
        QStringList list = scriptArrayToStringList( array );
        QDir d(dir);
        QScriptValue arr = engine->newArray(list.size());
        for(int i=0;i<list.size();i++){
            QString relFile = d.relativeFilePath(list[i]);
            arr.setProperty(i,relFile);
        }
        return arr;
    }else{
        QString file = context->argument(1).toString();
        QDir d(dir);
        QString relFile = d.relativeFilePath(file);
        return engine->toScriptValue(relFile);
    }

}

QScriptValue scriptUnique(QScriptContext *context, QScriptEngine *engine){

    int nargin = context->argumentCount();
    if (nargin!=1){
        return context->throwError("1 argument required: QStringList unique(const QStringList &list)");
    }
    QScriptValue array = context->argument(0);
    QStringList list = scriptArrayToStringList( array );
    list = list.toSet().toList();
    QScriptValue arr = engine->newArray(list.size());
    for(int i=0;i<list.size();i++){
        arr.setProperty(i,list[i]);
    }
    return arr;
}

QScriptValue scriptContains(QScriptContext *context, QScriptEngine *engine){

    int nargin = context->argumentCount();
    if (nargin<2 || nargin>4 ){
        return context->throwError(QString("2-3 arguments required: bool contains(const QStringList &list, const QString &val, bool caseSensitive=true)\n")+\
                                   "bool contains(const QString &str, const QString &val, bool caseSensitive=true)");
    }

    QString val = context->argument(1).toString();
    bool caseSensitive = true;
    if(nargin>2){
        caseSensitive = context->argument(2).toBool();
    }
    Qt::CaseSensitivity s = Qt::CaseSensitive;
    if(!caseSensitive){
        s = Qt::CaseInsensitive;
    }

    bool res;
    if(context->argument(0).isArray()){
        QScriptValue array = context->argument(0);
        QStringList list = scriptArrayToStringList( array );
         res = list.contains(val,s);
    }else{
        QString str = context->argument(0).toString();
        res = str.contains(val,s);
    }

    return engine->toScriptValue(res);

}

QScriptValue scriptCopyFiles(QScriptContext *context, QScriptEngine *engine){

    int nargin = context->argumentCount();
    if (nargin<2 || nargin>4 ){
        return context->throwError("2-4 arguments required: [int res, QString log] = copyFoundFiles(const QStringList &files, const QString &copyFilesToDir, bool keepFolderStructure=false, bool overWrite=true)");
    }
    QScriptValue array = context->argument(0);
    QStringList files = scriptArrayToStringList( array );
    //QStringList files = context->argument(0).toStringList();
    QString dir = context->argument(1).toString();
    bool keepFolderStructure = false;
    if(nargin>2){
        keepFolderStructure = context->argument(2).toBool();
    }
    bool overWrite = true;
    if(nargin>3){
        overWrite = context->argument(3).toBool();
    }
    QString log;
    int res = Global::copyFoundFiles( files, dir, keepFolderStructure, overWrite, &log );
    QScriptValue out = engine->newArray(2);
    out.setProperty(0,res);
    out.setProperty(1,log);

    return out;
}

QScriptValue writeFile(QScriptContext *context, QScriptEngine *engine){

    int nargin=context->argumentCount();
    if (nargin<2 || nargin>3){
        return context->throwError("2 or 3 arguments required: writeFile( const QStringList &lines, const QString &file, bool append=false )");
    }
    QScriptValue array = context->argument(0);
    QStringList lines = scriptArrayToStringList( array );
    QString file = context->argument(1).toString();
    bool append = false;
    if(nargin==3){
        append = context->argument(2).toBool();
    }

    QFile f(file);
    if(append){
        f.open(QIODevice::Append | QIODevice::Text);
    }else{
        f.open(QIODevice::WriteOnly | QIODevice::Text);
    }
    if(!f.isOpen()){
        return engine->toScriptValue(false);
    }
    QTextStream out(&f);
    for(int i=0;i<lines.size();i++){
        out << lines[i];
        if(i<lines.size()-1){
            out << "\n";
        }
    }
    f.close();
    return engine->toScriptValue(true);
}


QScriptValue toQFileInfo(QScriptEngine *e, const QFileInfo &info) {
    return e->newVariant(info.absoluteFilePath());
}
void fromQFileInfo(const QScriptValue &v, QFileInfo &info) {
    info = QFileInfo(v.toString());
}

QScriptValue toStringStringListHash(QScriptEngine *e, const QHash<QString,QStringList> &hash ) {

    QStringList keys = hash.keys();
    QScriptValue array = e->newArray(keys.size());
    for (int i=0;i<keys.size();i++){
        QStringList list = hash[keys[i]];
        QScriptValue array2 = e->newArray(list.size());
        for (int k=0;k<list.size();k++){
            array2.setProperty(k, QScriptValue(e, list[k]));
        }        
        array.setProperty(keys[i], array2);
    }
    return array;


}
void fromStringStringListHash(const QScriptValue &v, QHash<QString,QStringList> &hash) {

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
