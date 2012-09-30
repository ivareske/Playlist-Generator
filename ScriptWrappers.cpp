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

QScriptValue myPrint( QScriptContext * ctx, QScriptEngine * eng ) {
    Q_UNUSED(eng);
    QString result;
    for (int i=0; i<ctx->argumentCount(); i++){
        if(i!=0){
            result.append(QLatin1String(" "));
        }
        QString s = ctx->argument(i).toString();
        if (ctx->state() == QScriptContext::ExceptionState){
            break;
        }
        result.append(s);
    }
    std::clog<<result.toStdString()<<std::endl;
    return ctx->engine()->toScriptValue(result);
}


QScriptValue scriptGetDirContent(QScriptContext *context, QScriptEngine *engine){

    QList<QFileInfo> info;
    int nargin=context->argumentCount();
    if( nargin>4 || nargin<2 ){
        return context->throwError("2-4 arguments required. QStringList getDirContent( const QString &path, const QStringList &extensions, bool includeSubFolders=true, bool hiddenFiles=true ) ");
    }
    QString path = context->argument(0).toString();    
    QScriptValue array = context->argument(1);
    QStringList extensions = scriptArrayToStringList( array );
    bool includeSubFolders = true;
    bool hiddenFiles = true;
    if(nargin>2){
        includeSubFolders = context->argument(2).toBool();
    }
    if(nargin>3){
        hiddenFiles = context->argument(3).toBool();
    }
    info = Global::getDirContent( path, extensions, includeSubFolders, hiddenFiles );

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
        return context->throwError("3 or 4 arguments required:\nbool writeFile( const QStringList &lines, const QString &path, const QString &name, bool append=false )\n"\
                                   "bool writeFile( const QString &content, const QString &path, const QString &name, bool append=false )");
    }
    QString content;
    QScriptValue arg0 = context->argument(0);
    if(arg0.isArray()){
        QScriptValue array = arg0;
        content = scriptArrayToStringList( array ).join("\n");
    }else if(arg0.isString()){
        content = arg0.toString();
    }else{
        return context->throwError("The first argument (the content of the file) must be an array or string or a string.");
    }
    if(!context->argument(1).isString()){
        return context->throwError("The second argument (path) must be a string.");
    }
    if(!context->argument(2).isString()){
        return context->throwError("The third argument (filename) must be a string.");
    }
    if(context->argumentCount()==4){
        if(!context->argument(3).isBool()){
            return context->throwError("The fourth argument (append) must be boolean/logical.");
        }
    }
    QString path = context->argument(1).toString();
    QString name = context->argument(2).toString();
    QString sep="";
    if(path.right(1)!="\\"&&path.right(1)!="/"){
        sep="/";
    }
    QString file = path+sep+Global::createValidFileName(name);
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
    out << content;
    /*
    for(int i=0;i<lines.size();i++){
        out << lines[i].replace("/","\\");
        if(i<lines.size()-1){
            out << "\n";
        }
    }
    */
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
        QScriptValue v2 = it.value();
        if(v2.isArray()){
            QStringList vector;
            qScriptValueToSequence(v2,vector);
            hash.insert(it.name(),vector);
        }
    }    

}


//http://www.qtcentre.org/threads/33355-Use-QObject-with-QtScript
QScriptValue constructTag(QScriptContext *context, QScriptEngine *engine){
    if (!context->isCalledAsConstructor()){
        return context->throwError(QScriptContext::SyntaxError, "please use the 'new' operator (var tag = new Tag(fileName);)");
    }
    if(context->argumentCount()!=1 ){
        return context->throwError("1 argument required (Full filename/string): var tag = new Tag(fileName);");
    }
    QString file = context->argument(0).toString();
    // store the shared pointer in the script object that we are constructing
    //return engine->newVariant(context->thisObject(), qVariantFromValue(new Tag(file)));
    Tag *tag = new Tag(file);
    return engine->newQObject(tag);
}



}
