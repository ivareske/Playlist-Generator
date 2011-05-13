
#include "playList.h"


playList::playList( QStringList defaultExtensions ){

    name="";
    extensions = defaultExtensions; //"*.mp3;*.wma;*.wav;*.ogg;*.aac;*.ac3";
    randomize = false;
    includeSubFolders = true;
    relativePath = true;
    allRulesTrue = false;
    includeExtInf = true;
    makeUnique = false;
    copyFiles=false;
    keepTags=true;
    uniqueId = -1;
    script="";
    scriptvariables<<"ARTIST"<<"ALBUM"<<"TITLE"<<"GENRE"<<"TRACK"<<"YEAR"<<"COMMENT"<<"LENGTH"<<"SAMPLERATE"<<"BITRATE"<<"CHANNELS";

}

void playList::getGuiSettings(){

    guiSettings = new QSettings("playListGenerator"+ext,QSettings::IniFormat,0);
    artistEmpty = guiSettings->value("artistEmpty",true).toBool();
    titleEmpty = guiSettings->value("titleEmpty",true).toBool();
    albumEmpty = guiSettings->value("albumEmpty",false).toBool();
    commentEmpty = guiSettings->value("commentEmpty",false).toBool();
    genreEmpty = guiSettings->value("genreEmpty",false).toBool();
    trackEmpty = guiSettings->value("trackEmpty",false).toBool();
    yearEmpty = guiSettings->value("yearEmpty",false).toBool();
    outputfolder = guiSettings->value("defaultOutputFolder","").toString();
    format = guiSettings->value("format","%artist% - %title%").toString();
    useScript = guiSettings->value("useScript",false).toBool();
    keepTags = guiSettings->value("keepTags",true).toBool();
    useCopyFilesToPath = guiSettings->value("useCopyFilesToPath",true).toBool();
    ShowTagLibDebug = guiSettings->value("ShowTagLibDebug",true).toBool();

}

bool playList::generate( QList<m3uEntry> *songsOut, QStatusBar *s, QString *log, QHash<QString, Tag> *tags ){

    getGuiSettings();

    time_t Start_t, End_t;
    int totaltime;
    Start_t = time(NULL);

    bool canceled = false;
    QList<m3uEntry> songs = findFiles( s, &canceled, log, tags );

    songsOut->append(songs);
    if(canceled){
        return canceled;
    }

    if(songs.isEmpty()){
        QMessageBox::information(0, "",
                                 "No songs matching your criteria were found for playlist "+name,
                                 QMessageBox::Ok, QMessageBox::Ok);
        End_t = time(NULL);    //record time that task 1 ends
        totaltime = difftime(End_t, Start_t);    //compute elapsed time of task 1
        log->append("\n Found "+QString::number(songs.size())+" songs. Time used: "+QString::number(totaltime)+" seconds\n");
        return true;
    }

    writeM3U( songs );

    s->showMessage( "Generated playlist '"+name+"' with "+QString::number(songs.size())+" songs", 10000 );

    End_t = time(NULL);    //record time that task 1 ends
    totaltime = difftime(End_t, Start_t);    //compute elapsed time of task 1
    std::cout<<"time used: "<<totaltime<<std::endl;

    log->append("\n Found "+QString::number(songs.size())+" songs for "+ name +". Time used: "+QString::number(totaltime)+" seconds\n");
    return canceled;
}

void playList::writeM3U( QList<m3uEntry> songs ){

    if(outputfolder.right(1)!="\\" && outputfolder.right(1)!="/"){
        outputfolder.append("/");
    }
    QString file = outputfolder + name + ".m3u";

    QFile f( file );
    if( !f.open( QIODevice::WriteOnly ) ){
        QMessageBox::warning(0, "",
                             "Could not open "+file+" for writing",
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    QTextStream ts( &f );
    if( includeExtInf ){
        ts << "#EXTM3U" <<endl;
    }
    for(int i=0;i<songs.size();i++){
        if( includeExtInf ){
            ts << songs[i].extInf() << endl;
        }
        ts << songs[i].fullFile() << endl;
    }

    f.close();
    qDebug()<<"Finished writing "<<file;
}

QList<m3uEntry> playList::findFiles( QStatusBar *s, bool *canceled, QString *log, QHash<QString,Tag> *tags ){

    qDebug()<<"finding files...";

    //the two first mandatory rules
    QList<m3uEntry> plist;



    bool hasTagRule = false;
    bool hasAudioRule = false;
    if( useScript ){
        for(int i=0;i<scriptvariables.size();i++){
            if( script.contains(scriptvariables[i]) ){
                hasAudioRule = true;
                hasTagRule = true;
                break;
            }
        }
    }else{
        for(int i=0;i<rules.size();i++){
            rule r = rules[i];
            ruleType t = r.type;
            if(t!=FILENAME_CONTAINS){
                hasTagRule = true;
                hasAudioRule = true;
                break;
            }
            /*
                        //check that this coresponds with the ruleTypes enum!
                        if( (int)t>=(int)TAG_TITLE_CONTAINS && (int)t<(int)AUDIO_BITRATE_IS){
                                hasTagRule = true;
                        }
                        if( (int)t>=(int)AUDIO_BITRATE_IS ){
                                hasAudioRule = true;
                        }
                        */
        }
        if(includeExtInf){
            hasAudioRule = true;
        }
        if(hasAudioRule){
            hasTagRule = true;
        }
    }

    if(folders.size()==0){
        QMessageBox::critical(0, "",
                              "You must specify at least one folder",
                              QMessageBox::Ok, QMessageBox::Ok);
        *canceled = true;
        return plist;
    }
    if(extensions.size()==0){
        QMessageBox::critical(0, "",
                              "Extensions can`t be empty",
                              QMessageBox::Ok, QMessageBox::Ok);
        *canceled = true;
        return plist;
    }

    QList<QFileInfo> fileInfo;
    for(int i=0;i<folders.size();i++){
        QDir d(folders[i]);
        if( !d.exists() ){
            QMessageBox::critical(0, "",
                                  "Folder"+folders[i]+" doesn`t exist",
                                  QMessageBox::Ok, QMessageBox::Ok);
            *canceled = true;
            return plist;
        }
        if( !d.isAbsolute() ){
            QMessageBox::critical(0, "",
                                  "Folder"+folders[i]+" is not an absolute path",
                                  QMessageBox::Ok, QMessageBox::Ok);
            plist.clear();
            *canceled = true;
            return plist;
        }

        //get content from the specified folder(s)
        fileInfo = fileInfo + getDirContent( folders[i] );
    }
    //make unique
    fileInfo = fileInfo.toSet().toList();

    //local copy of QHash tags: new read tags are inserted into global tags. used files are removed from local qhash tags to increase lookup speed
    QHash<QString,Tag> tagscopy = *tags;

    int n=fileInfo.size(); QList<m3uEntry> tmplist;
    QProgressDialog p("Locating files for playlist "+name, "Abort", 0, n, 0);
    p.setWindowModality(Qt::WindowModal);


    QPushButton *cancelButton = new QPushButton;
    p.setCancelButton(cancelButton);
    p.setCancelButtonText("Cancel");
    p.setLabelText("Locating files for "+name);
    bool wasCanceled=false;

    qDebug()<<"found "<<n<<" files to process";


    for(int i=0;i<n;i++){
        p.setValue(i);
        if (p.wasCanceled()){
            wasCanceled = true;
            break;
        }

        //TagLib::DebugLogger::Instance()->clear();
        tmplist = processFile( &wasCanceled, fileInfo[i], s, hasTagRule, hasAudioRule, log, tags, &tagscopy );
        /*std::vector<std::string> taglibDebug = TagLib::DebugLogger::Instance()->log();
        if( taglibDebug.size()!=0 && ShowTagLibDebug ){
            log->append( "\n" + fileInfo[i].absoluteFilePath() );
            for(unsigned j=0;j<taglibDebug.size();j++){
                log->append( ", " + QString(taglibDebug[j].c_str()) );
            }
            log->append( "\n" );
        }
        */

        if (wasCanceled){
            break;
        }
        plist = plist + tmplist;
    }

    p.setValue(n);
    if(wasCanceled){
        *canceled = true;
        return plist;
    }

    if( randomize ){
        std::random_shuffle( plist.begin(),plist.end() );
    }

    qDebug()<<"finished finding files";

    if(makeUnique){
        int size=plist.size();
        plist = plist.toSet().toList();
        if(plist.size()!=size){
            log->append("\nRemoved "+QString::number(size-plist.size())+" duplicate files\n");
        }
    }

    return plist;

}



QList<QFileInfo> playList::getDirContent( QString& aPath ){

    // append the filtered files to this list

    //decide to include subfolder or not
    QDirIterator::IteratorFlag subdirFlag;
    if( includeSubFolders ){
        subdirFlag = QDirIterator::Subdirectories;
    }else{
        subdirFlag = QDirIterator::NoIteratorFlags;
    }

    // set dir iterator
    QDirIterator *dirIterator = new QDirIterator(aPath,
                                                 extensions,
                                                 QDir::Files|QDir::NoSymLinks,
                                                 subdirFlag);


    QList<QFileInfo> fileInfo;
    while(dirIterator->hasNext()){
        dirIterator->next();
        fileInfo.append(dirIterator->fileInfo());
    }

    delete dirIterator;
    qDebug()<<"Finished!";
    return fileInfo;
}

QList<m3uEntry>  playList::processFile( bool *wasCanceled, QFileInfo fileInfo, QStatusBar *s, bool hasTagRule, bool hasAudioRule, QString *log, QHash<QString,Tag> *tags, QHash<QString,Tag> *tagscopy ){

    //QTime t; t.start();

    QList<m3uEntry>  list;

    QString file = fileInfo.fileName();
    QString fullfile = fileInfo.absoluteFilePath();
    //qDebug()<<"processing file "<<fullfile;

    Tag tag;
    if( hasTagRule || hasAudioRule || includeExtInf  ){
        //tag is extracted from local copy of qhash tags, delete it to increase lookup speed
        tag = tagscopy->take(fullfile);
        if( tag.fileName().isEmpty() ){
            tag = Tag(fullfile);
            tag.readTags();
            if(keepTags){
                tags->insert(fullfile,tag);
            }
        }
        if( !tag.tagOk() ){
            log->append("Could not read tag for "+fullfile+"\n");
        }
    }
    //qDebug()<<tags->size()<<tagscopy->size();

    //only these are needed
    bool anyOk;
    bool allOk;
    if(rules.size()==0){
        anyOk = true;
        allOk = true;
    }else{
        allOk = true;
        anyOk = false;
    }
    //loop list of individual files. If file matches one of the indiviudally specified files, it is
    //added. All rules ignored.
    bool skipRules = false;
    if( useScript && script.isEmpty() ){
        skipRules=true;
    }
    for(int i=0;i<individualFiles.size();i++){
        if( file == individualFiles[i] ){
            allOk = true;
            anyOk = true;
            skipRules = true;
            break;
        }
    }
    bool scriptok = useScript;
    if( !skipRules ){
        /*if(useScript){
                                anyOk = false;
                                allOk = false;
                                //if script is empty result will be true/ok/include

                                QScriptEngine e;
                                if( !e.importedExtensions().contains("qt.core") ){
                                        QScriptValue tmp = e.importExtension("qt.core");
                                        if( e.hasUncaughtException() ){
                                                log->append("\n"+tmp.toString());
                                        }
                                }
                                QString tmpscript=script;
                                qDebug()<<"before: "<<tmpscript;
                                QString definitons;
                                for(int i=0;i<scriptvariables.size();i++){
                                        if( tmpscript.contains(scriptvariables[i]) ){
                                                QVariant tmp = tag.getTag( scriptvariables[i].toLower() );
                                                QString var="";
                                                if(tmp.canConvert(QVariant::String)){
                                                        var = tmp.toString();
                                                }else if(tmp.canConvert(QVariant::Int)){
                                                        var = QString::number( tmp.toInt() );
                                                }
                                                definitons.append("var "+scriptvariables[i]+"="+"\""+var+"\"; ");
                                                //definitons.append("var timer = new QTimer;");
                                                //e.globalObject().setProperty( scriptvariables[i], var );
                                        }
                                }
                                tmpscript.prepend(definitons);
                                qDebug()<<"after: "<<tmpscript;
                                QScriptValue res = e.evaluate(tmpscript);
                                if( e.hasUncaughtException() ){
                                        QString err = e.uncaughtException().toString();
                                        qDebug() << "Error when evaluating script: \n"+tmpscript+"\n"+err;
                                        log->append("\nError when evaluating script: \n"+tmpscript+"\n"+err);
                                        *wasCanceled=true;
                                        scriptok=false;
                                }else{
                                        scriptok = res.toBool();
                                }
                                qDebug()<<"SCRIPT RESULT: "<<scriptok;
                        }else{
                        */
        //evaluate the set of specified rules
        evaluateRules( tag, file, &allOk, &anyOk );
        //qDebug()<<allOk<<anyOk;
        //}
    }
    //decide to include or not
    m3uEntry e;    
    if( (allRulesTrue && allOk) || (!allRulesTrue && anyOk) || scriptok ){
        //extinf info for m3u
        if( includeExtInf ){
            //e.extinf = createExtinfString( format, tag, ap, file );
            //e.extinf = createExtinfString( tag, ap, file, format );
            e.setExtInf( createExtinfString( tag, file, format ) );
        }
        e.setOriginalFile( fullfile );
        static QDir tmp;
        if( tmp.absolutePath()!=copyFilesToDir ){
            tmp = QDir(copyFilesToDir);
        }
        static QDir d;
        if( d.absolutePath()!=outputfolder ){
            d = QDir(outputfolder);
        }
        if( relativePath ){
            if( useCopyFilesToPath && copyFiles && tmp.exists() ){
                QString tmpfullfile = tmp.absolutePath()+"/"+file;
                e.setFullFile( d.relativeFilePath( tmpfullfile ) );
            }else{
                e.setFullFile( d.relativeFilePath( fullfile ) );
            }
        }else{
            if( useCopyFilesToPath && copyFiles && tmp.exists() ){
                e.setFullFile( tmp.absolutePath()+"/"+file );
            }else{
                e.setFullFile( fullfile );
            }
        }
        e.setFile( file );
        list.append( e );
    }

    //append cerr info to log, and reset cerr redirection
    //log->append("\n"+QString(ostr.str().c_str())+"\n");
    //std::cerr.rdbuf(cerr_buffer);

    /*int msec = t.elapsed();
    if(msec>10){
        log->append("\n"+fullfile+": Time used: "+QString::number(msec));
    }
*/
    return list;
}

void playList::evaluateRules( Tag tag, QString file, bool *allOk, bool* anyOk ){

    bool ok;
    for(int i=0;i<rules.size();i++){
        rule r = rules[i];
        ruleType t = r.type;
        bool shouldBeTrue = r.shouldBeTrue;
        Qt::CaseSensitivity s = Qt::CaseInsensitive;
        if(r.caseSensitive){
            s = Qt::CaseSensitive;
        }
        if( t==FILENAME_CONTAINS ){
            checkRule( file.contains( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==FILENAME_EQUALS ){
            checkRule( file.compare( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_TITLE_CONTAINS ){
            QString tmp = tag.getTag("title").toString();
            checkRule( tmp.contains( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_TITLE_EQUALS ){
            QString tmp = tag.getTag("title").toString();
            checkRule( tmp.compare( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_ARTIST_CONTAINS ){
            QString tmp = tag.getTag("artist").toString();
            checkRule( tmp.contains( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_ARTIST_EQUALS ){
            QString tmp = tag.getTag("artist").toString();
            checkRule( tmp.compare( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_ALBUM_CONTAINS ){
            QString tmp = tag.getTag("album").toString();
            checkRule( tmp.contains( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_ALBUM_EQUALS ){
            QString tmp = tag.getTag("album").toString();
            checkRule( tmp.compare( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_COMMENT_CONTAINS ){
            QString tmp = tag.getTag("comment").toString();
            checkRule( tmp.contains( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_COMMENT_EQUALS ){
            QString tmp = tag.getTag("comment").toString();
            checkRule( tmp.compare( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_GENRE_CONTAINS ){
            QString tmp = tag.getTag("genre").toString();
            checkRule( tmp.contains( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_GENRE_EQUALS ){
            QString tmp = tag.getTag("genre").toString();
            checkRule( tmp.compare( r.value, s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==TAG_TRACK_IS ){
            int tmp = tag.getTag("track").toInt();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==TAG_YEAR_IS ){
            int tmp = tag.getTag("year").toInt();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==AUDIO_BITRATE_IS ){
            int tmp = tag.getTag("bitrate").toInt();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==AUDIO_SAMPLERATE_IS ){
            int tmp = tag.getTag("sampleRate").toInt();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==AUDIO_CHANNELS_IS ){
            int tmp = tag.getTag("channels").toInt();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==AUDIO_LENGTH_IS ){
            int tmp = tag.getTag("length").toInt();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }
    }

}


QString playList::createExtinfString( Tag tag, QString file, QString format ){

    //if( tag!=NULL ){
    if( tag.tagOk() ){
        QString artist = tag.getTag("artist").toString(); //tag->artist().toCString();
        QString title = tag.getTag("title").toString(); //tag->title().toCString();
        QString album = tag.getTag("album").toString(); //tag->album().toCString();
        QString comment = tag.getTag("comment").toString(); //tag->comment().toCString();
        QString genre = tag.getTag("genre").toString(); //tag->genre().toCString();
        QString year = QString::number( tag.getTag("year").toInt() ); //QString::number( tag->year() );
        QString track = QString::number( tag.getTag("track").toInt() ); //QString::number( tag->track() );

        format.replace( QString("%artist%"), artist );
        format.replace( QString("%title%"), title );
        format.replace( QString("%album%"), album );
        format.replace( QString("%comment%"), comment );
        format.replace( QString("%genre%"), genre );
        format.replace( QString("%year%"), year );
        format.replace( QString("%track%"), track );
        format.replace( QString("%filename%"), file );

        int sum = (int)artistEmpty + (int)titleEmpty + (int)albumEmpty + (int)commentEmpty + (int)genreEmpty + (int)trackEmpty + (int)yearEmpty;
        int sum2=0;
        if( artist.isEmpty() && artistEmpty ){
            sum2++;
        }
        if( title.isEmpty() && titleEmpty ){
            sum2++;
        }
        if( album.isEmpty() && albumEmpty ){
            sum2++;
        }
        if( comment.isEmpty() && commentEmpty ){
            sum2++;
        }
        if( genre.isEmpty() && genreEmpty ){
            sum2++;
        }
        if( track.isEmpty() && trackEmpty ){
            sum2++;
        }
        if( year.isEmpty() && yearEmpty ){
            sum2++;
        }
        if(sum2==sum){
            format=file;
        }

    }else{
        format = file;
    }
    /*int length=-1;;
        if( ap ){
                length = ap->length();
        }
        */
    QString result = QString("#EXTINF:")+QString::number( tag.getTag("length").toInt() )+","+format;

    return result;
}

void playList::checkRule( bool ruleCheck, bool *allOk, bool *anyOk, bool shouldBeTrue ){
    if( ruleCheck ){
        *allOk = *allOk && shouldBeTrue;
        *anyOk = shouldBeTrue;
    }else{
        *allOk = *allOk && !shouldBeTrue;
        *anyOk = !shouldBeTrue;
    }
}

void playList::checkRange( QVector<int> intvals, int tmp, bool *allOk, bool *anyOk, bool shouldBeTrue ){


    int n=intvals.size();
    if(n==1){
        if( tmp==intvals[0] && shouldBeTrue ){
            *allOk = *allOk && true;
            *anyOk = true;
        }else{
            *allOk = *allOk && false;
        }
    }else if(n==2){
        if( tmp>=intvals[0] && tmp<=intvals[1] && shouldBeTrue ){
            *allOk = *allOk && true;
            *anyOk = true;
        }else{
            *allOk = *allOk && false;
        }
    }

}



