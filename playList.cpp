
#include "PlayList.h"


PlayList::PlayList( const QStringList &defaultExtensions, QListWidget *parent ) : QListWidgetItem(parent){

    setName("New playlist");
    extensions_ = defaultExtensions;
    if( extensions_.size()==0 ){
        extensions_<<"*.mp3"<<"*.wma"<<"*.wav"<<"*.ogg"<<"*.aac"<<"*.ac3";
    }
    randomize_ = false;
    includeSubFolders_ = true;
    relativePath_ = true;
    allRulesTrue_ = false;
    includeExtInf_ = true;
    makeUnique_= false;
    copyFiles_=false;
    uniqueId_ = -1;
    script_="";
    scriptVariables_<<"ARTIST"<<"ALBUM"<<"TITLE"<<"GENRE"<<"TRACK"<<"YEAR"<<"COMMENT"<<"LENGTH"<<"SAMPLERATE"<<"BITRATE"<<"CHANNELS";

}

PlayList::PlayList( const PlayList &other ){
    QListWidgetItem::operator =(other);
    setName(other.name());
    rules_ = other.rules();
    folders_ = other.folders();
    extensions_ = other.extensions();
    randomize_ = other.randomize();
    includeSubFolders_ = other.includeSubFolders();
    relativePath_ = other.relativePath();
    allRulesTrue_ = other.allRulesTrue();
    includeExtInf_ = other.includeExtInf();
    makeUnique_ = other.makeUnique();
    copyFilesToDir_ = other.copyFilesToDir();
    copyFiles_ = other.copyFiles();
    individualFiles_ = other.individualFiles();
    script_ = other.script();
    outPutFolder_ = other.outPutFolder();
    scriptVariables_<<"ARTIST"<<"ALBUM"<<"TITLE"<<"GENRE"<<"TRACK"<<"YEAR"<<"COMMENT"<<"LENGTH"<<"SAMPLERATE"<<"BITRATE"<<"CHANNELS";

}

void PlayList::copyFoundFiles( QList<M3uEntry> songs, QString *log ){

    log->append("\nResult from file copy:\n");
    int nCopied=0;

    qDebug()<<"starting to copy "<<songs.size()<<" files!";
    QProgressDialog pr("Copying files for playlist "+name()+" to "+copyFilesToDir_.absolutePath(), "Abort", 0, songs.size(), 0);
    pr.setWindowModality(Qt::WindowModal);
    QPushButton *cancelButton = new QPushButton;
    pr.setCancelButton(cancelButton);
    pr.setCancelButtonText("Cancel");
    pr.setLabelText("Copying files");
    for(int j=0;j<songs.size();j++){
        pr.setValue(j);
        if (pr.wasCanceled()){
            break;
        }
        QFile f( songs[j].originalFile() );
        QString newname = copyFilesToDir_.absolutePath() + "/" + songs[j].file();
        bool okf = f.copy( newname );
        //qDebug_<<"copy result of "<<songs[j].originalfile<<" -> "<<newname<<": "<<okf;
        if(!okf){
            QFile f2(newname);
            if( f2.exists() ){
                log->append( songs[j].file() + " was not copied as it already exists in "+copyFilesToDir_.absolutePath()+"\n" );
            }else{
                log->append( songs[j].file() + " could not be copied to "+copyFilesToDir_.absolutePath()+"\n" );
            }
        }else{
            nCopied++;
        }
    }
    pr.setValue(songs.size());
    pr.close();
    log->append(QString::number(nCopied)+" of "+QString::number(songs.size())+" files copied to "+copyFilesToDir_.absolutePath()+"\n");

}


bool PlayList::generate( QList<M3uEntry> *songsOut, QStatusBar *s, QString *log, QHash<QString, Tag> *tags, const SettingsClass &settings ){    

    //getGuiSettings();
    settings_ = settings;

    time_t Start_t, End_t;
    int totaltime;
    Start_t = time(0);

    bool canceled = false;
    QList<M3uEntry> songs = findFiles( &canceled, log, tags );

    songsOut->append(songs);
    if(canceled){
        return canceled;
    }

    if(songs.isEmpty()){
        QMessageBox::information(0, "",
                                 "No songs matching your criteria were found for playlist "+name(),
                                 QMessageBox::Ok, QMessageBox::Ok);
        End_t = time(0);    //record time that task 1 ends
        totaltime = difftime(End_t, Start_t);    //compute elapsed time of task 1
        log->append("\n Found "+QString::number(songs.size())+" songs. Time used: "+QString::number(totaltime)+" seconds\n");
        return true;
    }

    bool writeOk = writeM3U( songs, log );

    if(writeOk){
        s->showMessage( "Generated playlist '"+name()+"' with "+QString::number(songs.size())+" songs", 10000 );
    }
    End_t = time(0);    //record time that task 1 ends
    totaltime = difftime(End_t, Start_t);    //compute elapsed time of task 1
    std::cout<<"time used: "<<totaltime<<std::endl;

    log->append("\n Found "+QString::number(songs.size())+" songs for "+ name() +". Time used: "+QString::number(totaltime)+" seconds\n");
    return canceled;
}

bool PlayList::writeM3U( QList<M3uEntry> songs, QString *log ){

    QString file = outPutFolder_.absolutePath() +"/"+ name() + ".m3u";

    QFile f( file );
    if( !f.open( QIODevice::WriteOnly ) ){
        /*
        QMessageBox::warning(0, "",
                             "Could not open "+file+" for writing",
                             QMessageBox::Ok, QMessageBox::Ok);
        */
        if(log){
            log->append("\nCould not open "+file+" for writing");
        }
        return false;
    }
    QTextStream ts( &f );
    if( includeExtInf_ ){
        ts << "#EXTM3U" <<endl;
    }
    for(int i=0;i<songs.size();i++){
        if( includeExtInf_ ){
            ts << songs[i].extInf() << endl;
        }
        ts << songs[i].fullFile() << endl;
    }

    f.close();
    qDebug()<<"Finished writing "<<file;
    return true;
}

QList<M3uEntry> PlayList::findFiles( bool *canceled, QString *log, QHash<QString,Tag> *tags ){

    qDebug()<<"finding files...";

    //the two first mandatory rules_
    QList<M3uEntry> plist;



    bool hasTagRule = false;
    bool hasAudioRule = false;
    if( settings_.useScript() ){
        for(int i=0;i<scriptVariables_.size();i++){
            if( script_.contains(scriptVariables_[i]) ){
                hasAudioRule = true;
                hasTagRule = true;
                break;
            }
        }
    }else{
        for(int i=0;i<rules_.size();i++){
            Rule r = rules_[i];
            Rule::RuleType t = r.type();
            if(t!=Rule::FILENAME_CONTAINS){
                hasTagRule = true;
                hasAudioRule = true;
                break;
            }
        }
        if(includeExtInf_){
            hasAudioRule = true;
        }
        if(hasAudioRule){
            hasTagRule = true;
        }
    }

    if(folders_.size()==0){
        QMessageBox::critical(0, "",
                              "You must specify at least one folder",
                              QMessageBox::Ok, QMessageBox::Ok);
        *canceled = true;
        return plist;
    }
    if(extensions_.size()==0){
        QMessageBox::critical(0, "",
                              "Extensions can`t be empty",
                              QMessageBox::Ok, QMessageBox::Ok);
        *canceled = true;
        return plist;
    }

    QList<QFileInfo> fileInfo;
    for(int i=0;i<folders_.size();i++){
        QDir d(folders_[i]);
        if( !d.exists() ){
            QMessageBox::critical(0, "",
                                  "Folder"+folders_[i].absolutePath()+" doesn`t exist",
                                  QMessageBox::Ok, QMessageBox::Ok);
            *canceled = true;
            return plist;
        }
        if( !d.isAbsolute() ){
            QMessageBox::critical(0, "",
                                  "Folder"+folders_[i].absolutePath()+" is not an absolute path",
                                  QMessageBox::Ok, QMessageBox::Ok);
            plist.clear();
            *canceled = true;
            return plist;
        }

        //get content from the specified folder(s)
        fileInfo = fileInfo + getDirContent( folders_[i].absolutePath() );
    }
    //make unique
    fileInfo = fileInfo.toSet().toList();

    //local copy of QHash tags: new read tags are inserted into global tags. used files are removed from local qhash tags to increase lookup speed
    static QHash<QString,Tag> tagsCopy;
    if( tagsCopy.size()==0 ){
        tagsCopy = *tags;
    }

    int n=fileInfo.size(); QList<M3uEntry> tmplist;
    QProgressDialog p("Locating files for playlist "+name(), "Abort", 0, n, 0);
    p.setWindowModality(Qt::WindowModal);


    QPushButton *cancelButton = new QPushButton;
    p.setCancelButton(cancelButton);
    p.setCancelButtonText("Cancel");
    p.setLabelText("Locating files for "+name());
    bool wasCanceled=false;

    qDebug()<<"found "<<n<<" files to process";


    for(int i=0;i<n;i++){
        p.setValue(i);
        if (p.wasCanceled()){
            wasCanceled = true;
            break;
        }

        tmplist = processFile( fileInfo[i], hasTagRule, hasAudioRule, log, tags, &tagsCopy );

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

    if( randomize_ ){
        std::random_shuffle( plist.begin(),plist.end() );
    }

    qDebug()<<"finished finding files";

    if(makeUnique_){
        int size=plist.size();
        plist = plist.toSet().toList();
        if(plist.size()!=size){
            log->append("\nRemoved "+QString::number(size-plist.size())+" duplicate files\n");
        }
    }

    return plist;

}



QList<QFileInfo> PlayList::getDirContent( const QString& aPath ){

    // append the filtered files to this list

    //decide to include subfolder or not
    QDirIterator::IteratorFlag subdirFlag;
    if( includeSubFolders_ ){
        subdirFlag = QDirIterator::Subdirectories;
    }else{
        subdirFlag = QDirIterator::NoIteratorFlags;
    }

    // set dir iterator
    QDirIterator *dirIterator = new QDirIterator(aPath,
                                                 extensions_,
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

QList<M3uEntry>  PlayList::processFile( QFileInfo fileInfo, bool hasTagRule, bool hasAudioRule, QString *log, QHash<QString,Tag> *tags, QHash<QString,Tag> *tagsCopy ){

    QList<M3uEntry>  list;

    QString file = fileInfo.fileName();
    QString fullfile = fileInfo.absoluteFilePath();

    Tag tag;
    if( hasTagRule || hasAudioRule || includeExtInf_  ){
        //take tag from the inital copy of tags. If it doesnt exist there, read it, and insert into
        //the original qhash of tags
        tag = tagsCopy->take(fullfile);
        if( tag.fileName().isEmpty() ){
            tag = Tag(fullfile);
            tag.readTags();
            if(settings_.keepTags()){
                tags->insert(fullfile,tag);
            }
        }
        if( !tag.tagOk() ){
            log->append("Could not read tag for "+fullfile+"\n");
        }
    }

    //only these are needed
    bool anyOk;
    bool allOk;
    if(rules_.size()==0){
        anyOk = true;
        allOk = true;
    }else{
        allOk = true;
        anyOk = false;
    }
    //loop list of individual files. If file matches one of the indiviudally specified files, it is
    //added. All rules_ ignored.
    bool skipRules = false;
    if( settings_.useScript() && script_.isEmpty() ){
        skipRules=true;
    }
    for(int i=0;i<individualFiles_.size();i++){
        if( file == individualFiles_[i].absoluteFilePath() ){
            allOk = true;
            anyOk = true;
            skipRules = true;
            break;
        }
    }
    bool scriptok = settings_.useScript();
    if( !skipRules ){    
        evaluateRules( tag, file, &allOk, &anyOk );
    }
    //decide to include or not
    M3uEntry e;    
    if( (allRulesTrue_ && allOk) || (!allRulesTrue_ && anyOk) || scriptok ){
        //extinf info for m3u
        if( includeExtInf_ ){            
            e.setExtInf( createExtinfString( tag, file, settings_.format() ) );
        }
        e.setOriginalFile( fullfile );
        static QDir tmp;
        if( tmp.absolutePath()!=settings_.copyFilesToDir() ){
            tmp = QDir(settings_.copyFilesToDir());
        }
        static QDir d;
        if( d!=outPutFolder_ ){
            d = outPutFolder_;
        }
        if( relativePath_ ){
            if( settings_.useCopyFilesToPath() && copyFiles_ && tmp.exists() ){
                QString tmpfullfile = tmp.absolutePath()+"/"+file;
                e.setFullFile( d.relativeFilePath( tmpfullfile ) );
            }else{
                e.setFullFile( d.relativeFilePath( fullfile ) );
            }
        }else{
            if( settings_.useCopyFilesToPath() && copyFiles_ && tmp.exists() ){
                e.setFullFile( tmp.absolutePath()+"/"+file );
            }else{
                e.setFullFile( fullfile );
            }
        }
        e.setFile( file );
        list.append( e );
    }

    return list;
}

void PlayList::evaluateRules( Tag tag, QString file, bool *allOk, bool* anyOk ){

    bool ok;
    for(int i=0;i<rules_.size();i++){
        Rule r = rules_[i];
        Rule::RuleType t = r.type();
        bool shouldBeTrue = r.shouldBeTrue();
        Qt::CaseSensitivity s = Qt::CaseInsensitive;
        if(r.caseSensitive()){
            s = Qt::CaseSensitive;
        }
        if( t==Rule::FILENAME_CONTAINS ){
            checkRule( file.contains( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::FILENAME_EQUALS ){
            checkRule( file.compare( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_TITLE_CONTAINS ){
            QString tmp = tag.title();
            checkRule( tmp.contains( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_TITLE_EQUALS ){
            QString tmp = tag.title();
            checkRule( tmp.compare( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_ARTIST_CONTAINS ){
            QString tmp = tag.artist();
            checkRule( tmp.contains( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_ARTIST_EQUALS ){
            QString tmp = tag.artist();
            checkRule( tmp.compare( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_ALBUM_CONTAINS ){
            QString tmp = tag.album();
            checkRule( tmp.contains( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_ALBUM_EQUALS ){
            QString tmp = tag.album();
            checkRule( tmp.compare( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_COMMENT_CONTAINS ){
            QString tmp = tag.comment();
            checkRule( tmp.contains( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_COMMENT_EQUALS ){
            QString tmp = tag.comment();
            checkRule( tmp.compare( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_GENRE_CONTAINS ){
            QString tmp = tag.genre();
            checkRule( tmp.contains( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_GENRE_EQUALS ){
            QString tmp = tag.genre();
            checkRule( tmp.compare( r.value(), s ), allOk, anyOk, shouldBeTrue  );
        }else if( t==Rule::TAG_TRACK_IS ){
            int tmp = tag.track();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value() );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==Rule::TAG_YEAR_IS ){
            int tmp = tag.year();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value() );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==Rule::AUDIO_BITRATE_IS ){
            int tmp = tag.bitRate();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value() );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==Rule::AUDIO_SAMPLERATE_IS ){
            int tmp = tag.sampleRate();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value() );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==Rule::AUDIO_CHANNELS_IS ){
            int tmp = tag.channels();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value() );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }else if( t==Rule::AUDIO_LENGTH_IS ){
            int tmp = tag.length();
            QVector<int> intvals;
            ok = Global::checkIntValue( &intvals, r.value() );
            checkRange( intvals, tmp, allOk, anyOk, shouldBeTrue );
        }
    }

}


QString PlayList::createExtinfString( Tag tag, QString file, QString format ){

    //if( tag!=0 ){
    if( tag.tagOk() ){
        QString artist = tag.artist();
        QString title = tag.title();
        QString album = tag.album();
        QString comment = tag.comment();
        QString genre = tag.genre();
        QString year = QString::number(tag.year());
        QString track = QString::number(tag.track());

        format.replace( QString("%artist%"), artist );
        format.replace( QString("%title%"), title );
        format.replace( QString("%album%"), album );
        format.replace( QString("%comment%"), comment );
        format.replace( QString("%genre%"), genre );
        format.replace( QString("%year%"), year );
        format.replace( QString("%track%"), track );
        format.replace( QString("%filename%"), file );

        int sum = settings_.artistEmpty() + settings_.titleEmpty() + settings_.albumEmpty() \
                + settings_.commentEmpty() + settings_.genreEmpty() + settings_.trackEmpty() + settings_.yearEmpty();
        int sum2=0;
        if( artist.isEmpty() && settings_.artistEmpty() ){
            sum2++;
        }
        if( title.isEmpty() && settings_.titleEmpty() ){
            sum2++;
        }
        if( album.isEmpty() && settings_.albumEmpty() ){
            sum2++;
        }
        if( comment.isEmpty() && settings_.commentEmpty() ){
            sum2++;
        }
        if( genre.isEmpty() && settings_.genreEmpty() ){
            sum2++;
        }
        if( track.isEmpty() && settings_.trackEmpty() ){
            sum2++;
        }
        if( year.isEmpty() && settings_.yearEmpty() ){
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
    QString result = QString("#EXTINF:")+QString::number( tag.length() )+","+format;

    return result;
}

void PlayList::checkRule( bool ruleCheck, bool *allOk, bool *anyOk, bool shouldBeTrue ){
    if( ruleCheck ){
        *allOk = *allOk && shouldBeTrue;
        *anyOk = shouldBeTrue;
    }else{
        *allOk = *allOk && !shouldBeTrue;
        *anyOk = !shouldBeTrue;
    }
}

void PlayList::checkRange( QVector<int> intvals, int tmp, bool *allOk, bool *anyOk, bool shouldBeTrue ){


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


QString PlayList::fileNameWithoutExtension() const{
    return outPutFolder_.absolutePath()+"/"+name();
}

QString PlayList::name() const{
    return this->text();
}

QVector<Rule> PlayList::rules() const{
    return rules_;
}

QList<QDir> PlayList::folders() const{
    return folders_;
}

QStringList PlayList::extensions() const{
    return extensions_;
}

bool PlayList::randomize() const{
    return randomize_;
}

bool PlayList::includeSubFolders() const{
    return includeSubFolders_;
}

bool PlayList::relativePath() const{
    return relativePath_;
}

bool PlayList::allRulesTrue() const{
    return allRulesTrue_;
}

bool PlayList::includeExtInf() const{
    return includeExtInf_;
}

bool PlayList::makeUnique() const{
    return makeUnique_;
}

QDir PlayList::copyFilesToDir() const{
    return copyFilesToDir_;
}

bool PlayList::copyFiles() const{
    return copyFiles_;
}

QList<QFileInfo> PlayList::individualFiles() const{
    return individualFiles_;
}

QString PlayList::script() const{
    return script_;
}

QDir PlayList::outPutFolder() const{
    return outPutFolder_;
}


void PlayList::setName( const QString &name ){
    this->setText(name);
}

void PlayList::setRules( const QVector<Rule> &rules ){
    rules_ = rules;
}

void PlayList::setFolders( const QList<QDir> &folders ){
    folders_ = folders;
}

void PlayList::setExtensions( const QStringList &extensions ){
    extensions_ = extensions;
}

void PlayList::setRandomize( bool randomize ){
    randomize_ = randomize;
}

void PlayList::setIncludeSubFolders( bool includeSubFolders ){
    includeSubFolders_ = includeSubFolders;
}

void PlayList::setRelativePath( bool relativePath ){
    relativePath_ = relativePath;
}

void PlayList::setAllRulesTrue( bool allRulesTrue ){
    allRulesTrue_ = allRulesTrue;
}

void PlayList::setIncludeExtInf( bool includeExtInf ){
    includeExtInf_ = includeExtInf;
}

void PlayList::setMakeUnique( bool makeUnique ){
    makeUnique_ = makeUnique;
}


void PlayList::setCopyFilesToDir( const QDir &copyFilesToDir ){
    copyFilesToDir_ = copyFilesToDir;
}

void PlayList::setCopyFiles( bool copyFiles ){
    copyFiles_ = copyFiles;
}

void PlayList::setIndividualFiles( const QList<QFileInfo> &individualFiles ){
    individualFiles_ = individualFiles;
}

void PlayList::setScript( const QString &script ){
    script_ = script;
}

void PlayList::setOutPutFolder( const QDir &outPutFolder ){
    outPutFolder_ = outPutFolder;
}


PlayList::operator QVariant () const{
    return QVariant::fromValue(*this);
}

QDataStream &operator>>(QDataStream &in, PlayList &p){

    QString name_;
    QVector<Rule> rules_;
    QList<QString> folders_;
    QStringList extensions_;
    bool randomize_;
    bool includeSubFolders;
    bool relativePath_;
    bool allRulesTrue_;
    bool includeExtInf_;
    bool makeUnique_;    
    QString copyFilesToDir;
    bool copyFiles;
    QList<QString> individualFiles_;
    QString script_;
    QString outPutFolder;

    in >> name_ >> rules_ >> folders_ >> extensions_ >> randomize_ >> includeSubFolders >> relativePath_ >> allRulesTrue_;
    in >> includeExtInf_ >> makeUnique_;
    in >> copyFilesToDir >> copyFiles;
    in >> individualFiles_ >> script_ >> outPutFolder;

    QList<QDir> folders;
    for(int i=0;i<folders_.size();i++){
        folders.append(QDir(folders_[i]));
    }
    QList<QFileInfo> individualFiles;
    for(int i=0;i<individualFiles_.size();i++){
        individualFiles.append(QFileInfo(individualFiles_[i]));
    }

    p = PlayList();
    p.setName(name_); p.setRules(rules_); p.setFolders(folders); p.setExtensions(extensions_); p.setRandomize(randomize_);
    p.setIncludeSubFolders(includeSubFolders); p.setRelativePath(relativePath_); p.setAllRulesTrue(allRulesTrue_);
    p.setIncludeExtInf(includeExtInf_); p.setMakeUnique(makeUnique_); p.setCopyFilesToDir(QDir(copyFilesToDir));
    p.setCopyFiles(copyFiles); p.setIndividualFiles(individualFiles); p.setScript(script_); p.setOutPutFolder(QDir(outPutFolder));
    return in;
}

QDataStream &operator<<(QDataStream &out, const PlayList &p){

    QStringList folders;
    QList<QDir> folders_ = p.folders();
    for(int i=0;i<folders_.size();i++){
        folders.append(folders_[i].absolutePath());
    }
    QStringList individualFiles;
    QList<QFileInfo> individualFiles_ = p.individualFiles();
    for(int i=0;i<individualFiles_.size();i++){
        individualFiles.append(individualFiles_[i].absoluteFilePath());
    }
    out << p.name() << p.rules() << folders << p.extensions() << p.randomize() << p.includeSubFolders();
    out << p.relativePath() << p.allRulesTrue() << p.includeExtInf() << p.makeUnique() << p.copyFilesToDir().absolutePath();
    out << p.copyFiles() << individualFiles << p.script() << p.outPutFolder().absolutePath();
    return out;
}

