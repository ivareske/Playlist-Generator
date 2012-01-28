
#include "PlayList.h"


/*!
 \brief

 \param name
 \param parent
*/
PlayList::PlayList(const QString& name, QListWidget* parent) : QListWidgetItem(parent) {

    guiSettings = Global::guiSettings();

    setName(name);
    extensions_ = guiSettings->value("defaultExtensions").toStringList();
    randomize_ = false;
    includeSubFolders_ = true;
    relativePath_ = true;
    allRulesTrue_ = false;
    includeExtInf_ = true;
    makeUnique_ = false;
    copyFiles_ = false;
    script_ = "";

    setFlags( flags() | Qt::ItemIsEditable );

}


/*!
 \brief

 \param other
 \return bool PlayList::operator
*/
bool PlayList::operator==(const PlayList& other) const {

    bool res = name() == other.name();
    res &= rules_ == other.rules();
    res &= folders_ == other.folders();
    res &= extensions_ == other.extensions() && randomize_ == other.randomize() && includeSubFolders_ == other.includeSubFolders();
    res &= relativePath_ == other.relativePath() && allRulesTrue_ == other.allRulesTrue() && includeExtInf_ == other.includeExtInf();
    res &= makeUnique_ == other.makeUnique() && copyFilesToDir_ == other.copyFilesToDir() && copyFiles_ == other.copyFiles();
    res &= individualFiles_ == other.individualFiles() && script_ == other.script();    

    return res;
}

/*
QString PlayList::copyFilesName( const QString &fileName ){

    QString newname = copyFilesToDir_.absolutePath() + "/" + songs[j].file();
    if( keepFolderStructure ){
        QFileInfo fi(songs[j].originalFile());
        QStringList dirs = songs[j].originalFile().replace("\\","/").split("/");
        QString root = dirs[0];
        newname = songs[j].originalFile().replace( root, copyFilesToDir_.absolutePath() );
    }
    return newname;

}
*/



/*!
 \brief

 \param e
 \return QString
*/
QString PlayList::playListEntry(const M3uEntry& e) const {
    //obtain the filename to be used in the m3u playlist
    //not nescessary the same name as the original file
    //could be pointing to the location the original file is copied, if copied
    //could be relative or absolute
    //will also be different from originalFile if keepFolderStructure

    const QDir outPutPath(guiSettings->value("outPutPath") .toString());
    const QFileInfo file = e.originalFile();
    //qDebug() << "original file: " << file.absoluteFilePath();
    QString playListEntryName = file.absoluteFilePath();

    bool useCopyFilesToPath = guiSettings->value("useCopyFilesToPath").toBool();
    if (copyFiles_ && useCopyFilesToPath) {
        playListEntryName = copyFilesToDir_.absolutePath() + "/" + file.fileName();
        bool keepFolderStructure = guiSettings->value("keepFolderStructure").toBool();
        if (keepFolderStructure) {
            //qDebug()<<"file.absolutePath() "<<file.absolutePath();
            QStringList dirs = file.absolutePath().replace("\\", "/").split("/");
            QString root = dirs[0];
            //qDebug()<<"root: "<<root;
            //qDebug()<<"copyFilesToDir_.absolutePath() "<<copyFilesToDir_.absolutePath();
            playListEntryName = file.absoluteFilePath().replace(root, copyFilesToDir_.absolutePath());

        }
    }

    if (relativePath_) {
        playListEntryName = outPutPath.relativeFilePath(playListEntryName);
    }

    //qDebug() << "playListEntryName: " << playListEntryName;

    return playListEntryName;
}




/*!
 \brief

 \param songsOut
 \param log
 \param QHash<QString
 \param tags
 \return bool
*/
bool PlayList::generate(QList<M3uEntry> *songsOut, QString* log, QHash<QString, Tag*> *tags) {


    QTime timer;
    timer.start();

    bool canceled = false;
    QList<M3uEntry> songs = findFiles(&canceled, log, tags);

    songsOut->append(songs);
    if (canceled) {
        return canceled;
    }

    int milliSecs = timer.elapsed();
    QString timeUsed = Global::timeString(milliSecs);
    qDebug() << "time used: " << timeUsed<<milliSecs;

    log->append("\n Found " + QString::number(songs.size()) + " songs for " + name());
    log->append("\nTime used: " + timeUsed);

    if (songs.isEmpty()) {
        QMessageBox::information(0, "",
                                 "No songs matching your criteria were found for playlist " + name(),
                                 QMessageBox::Ok, QMessageBox::Ok);                        
        return true;
    }

    QString outPutPath = guiSettings->value("outPutPath").toString();
    QString file = outPutPath + "/" + name() + ".m3u";
    writeM3U(file, songs, log);

    return canceled;
}

/*!
 \brief

 \param file
 \param songs
 \param log
 \return bool
*/
bool PlayList::writeM3U(const QString& file, const QList<M3uEntry> &songs, QString* log) const {

    QFile f(file);
    if (!f.open(QIODevice::WriteOnly)) {

        if (log) {
            log->append("\nCould not open " + file + " for writing");
        }
        return false;
    }
    QTextStream ts(&f);
    if (includeExtInf_) {
        ts << "#EXTM3U" << endl;
    }
    for (int i = 0; i < songs.size(); i++) {
        if (includeExtInf_) {
            ts << songs[i].extInf() << endl;
        }
        ts << playListEntry(songs[i]) << endl;
    }

    f.close();
    qDebug() << "Finished writing " << file;
    return true;
}

/*!
 \brief

 \param canceled
 \param log
 \param QHash<QString
 \param tags
 \return QList<M3uEntry>
*/
QList<M3uEntry> PlayList::findFiles(bool* canceled, QString* log, QHash<QString, Tag*> *tags){

    qDebug() << "finding files...";

    //GATHER SOME SETTINGS HERE SO WE DON`T HAVE TO READ THEM FROM PMSettings EVERY TIME IN THE LOOP
    //UNSURE IF THAT WOULD HAVE SLOWED THINGS DOWN

    //store the ID3v2 fields and APE item keys before processing files
    frameFields = guiSettings->value("frameFields").toHash();

    bool keepTags = guiSettings->value("keepTags").toBool();
    bool useScript = guiSettings->value("useScript").toBool();
    showTaglibDebug = guiSettings->value("ShowTaglibDebug").toBool();
    QString format = guiSettings->value("format").toString();
    artistEmpty = guiSettings->value("artistEmpty").toBool();
    titleEmpty = guiSettings->value("titleEmpty").toBool();
    albumEmpty = guiSettings->value("albumEmpty").toBool();
    commentEmpty = guiSettings->value("commentEmpty").toBool();
    genreEmpty = guiSettings->value("genreEmpty").toBool();
    trackEmpty = guiSettings->value("trackEmpty").toBool();
    yearEmpty = guiSettings->value("yearEmpty").toBool();    

    //the two first mandatory rules_
    QList<M3uEntry> plist;


    bool hasTagRule = false;
    bool hasAudioRule = false;
    if (guiSettings->value("useScript").toBool()) {       
        hasAudioRule = true;
        hasTagRule = true;
    } else {
        for (int i = 0; i < rules_.size(); i++) {
            Rule r = rules_[i];
            Rule::RuleType t = r.type();
            if (t != Rule::FILENAME_CONTAINS) {
                hasTagRule = true;
                hasAudioRule = true;
                break;
            }
        }
        if (includeExtInf_) {
            hasAudioRule = true;
        }
        if (hasAudioRule) {
            hasTagRule = true;
        }
    }

    //just add all individually specified files first
    for(int i=0;i<individualFiles_.size();i++){
        QFileInfo f(individualFiles_[i]);
        if(!f.exists()){
            log->append("\nSpecified file does not exist: "+individualFiles_[i].absoluteFilePath());
            continue;
        }
        M3uEntry e;
        if (includeExtInf_) {
            Tag *tag = tags->value(f.absoluteFilePath());
            if (tag->fileName().isEmpty()) {
                tag = new Tag(f.absoluteFilePath());                
                if (keepTags) {
                    tags->insert(f.absoluteFilePath(), tag);
                }
            }
            if (!tag->tagOk()) {
                log->append("Could not read tag for " + f.absoluteFilePath() + "\n");
            }
            e.setExtInf(createExtInfString(tag, f.absoluteFilePath(), format));
            if(!keepTags){
                delete tag;
            }
        }
        e.setOriginalFile(f);
        plist.append(e);
    }

    if( !(folders_.size()==0 && individualFiles_.size()!=0) ){

        if (folders_.size()==0) {
            QMessageBox::critical(0, "",
                                  "You must specify at least one folder",
                                  QMessageBox::Ok, QMessageBox::Ok);
            *canceled = true;
            return plist;
        }
        if (extensions_.size() == 0) {
            QMessageBox::critical(0, "",
                                  "Extensions can`t be empty",
                                  QMessageBox::Ok, QMessageBox::Ok);
            *canceled = true;
            return plist;
        }

        QList<QFileInfo> fileInfo;
        for (int i = 0; i < folders_.size(); i++) {
            QDir d(folders_[i]);
            if (!d.exists()) {
                QMessageBox::critical(0, "",
                                      "Folder" + folders_[i].absolutePath() + " doesn`t exist",
                                      QMessageBox::Ok, QMessageBox::Ok);
                *canceled = true;
                return plist;
            }
            if (!d.isAbsolute()) {
                QMessageBox::critical(0, "",
                                      "Folder" + folders_[i].absolutePath() + " is not an absolute path",
                                      QMessageBox::Ok, QMessageBox::Ok);
                plist.clear();
                *canceled = true;
                return plist;
            }

            //get content from the specified folder(s)
            fileInfo = fileInfo + Global::getDirContent(folders_[i].absolutePath(),includeSubFolders_,extensions_, canceled);
            if(*canceled){
                log->append("\n\nCanceled by user");
                return plist;
            }
        }
        qDebug() << "found " << fileInfo.size() << " files to process";

        //make unique
        fileInfo = fileInfo.toSet().toList();


        //local copy of QHash tags: new read tags are inserted into global tags. used files are removed from local qhash tags to increase lookup speed
        QHash<QString, Tag*> tagsCopy = *tags;

        int n = fileInfo.size();
        QList<M3uEntry> tmplist;
        QProgressDialog p("Processing files for playlist " + name(), "Abort", 0, n, 0);
        p.setWindowModality(Qt::WindowModal);
        QPushButton* cancelButton = new QPushButton;
        p.setCancelButton(cancelButton);
        p.setCancelButtonText("Cancel");



        //process each of the files matching the specified extensions, and see if
        //they matches the rules/script
        for (int i = 0; i < n; i++) {
            p.setValue(i);
            if (p.wasCanceled()) {
                *canceled = true;
                log->append("\n\nCanceled by user");
                return plist;
            }
            QString fileLog;
            tmplist = processFile(fileInfo[i], hasTagRule, hasAudioRule, keepTags, format, useScript, &fileLog, tags, &tagsCopy, canceled );
            if(!fileLog.isEmpty()){
                fileLog.prepend("\n-------------------------------\n"+fileInfo[i].absoluteFilePath()+"\n");
                log->append(fileLog);
            }

            if (*canceled) {
                return plist;
            }
            plist = plist + tmplist;
        }
        p.setValue(n);


        qDebug() << "finished finding files";
    }
    if (randomize_) {
        std::random_shuffle(plist.begin(), plist.end());
    }


    if (makeUnique_) {
        int size = plist.size();
        plist = plist.toSet().toList();
        if (plist.size() != size) {
            log->append("\nRemoved " + QString::number(size - plist.size()) + " duplicate files\n");
        }
    }

    return plist;

}



/*!
 \brief

 \param fileInfo
 \param hasTagRule
 \param hasAudioRule
 \param log
 \param QHash<QString
 \param tags
 \param QHash<QString
 \param tagsCopy
 \return QList<M3uEntry>
*/
QList<M3uEntry>  PlayList::processFile(const QFileInfo& fileInfo, bool hasTagRule, bool hasAudioRule, bool keepTags, const QString &format, bool useScript, QString* log, QHash<QString, Tag*> *tags, QHash<QString, Tag*> *tagsCopy, bool *wasCanceled ) const {

    //qDebug()<<fileInfo.filePath();

    QList<M3uEntry> list;

    QString file = fileInfo.fileName();
    QString fullfile = fileInfo.absoluteFilePath();


    Tag *tag=0;
    if (hasTagRule || hasAudioRule || includeExtInf_) {
        //take tag from the inital copy of tags. If it doesnt exist there, read it, and insert into
        //the original qhash of tags
        tag = tagsCopy->take(fullfile);        
        if (tag->fileName().isEmpty()) {
            tag = new Tag(fullfile);            
            if (keepTags) {
                tags->insert(fullfile, tag);
            }
        }
        if (!tag->tagOk()) {
            log->append("\nCould not read tag");
        }
    }

    if(showTaglibDebug){
        QString taglibDebug = tag->tagLibDebug();
        if(!taglibDebug.isEmpty()){
            log->append("\nTaglib Debug info: "+taglibDebug);
        }
    }

    //only these are needed
    bool anyOk;
    bool allOk;
    if (rules_.size() == 0) {
        anyOk = true;
        allOk = true;
    }
    else {
        allOk = true;
        anyOk = false;
    }
    //loop list of individual files. If file matches one of the indiviudally specified files, it is
    //added.

    //if use script and nos cript specified, always include file
    bool skipRules = false;
    if (useScript && script_.isEmpty()) {
        skipRules = true;
    }

    //if the file equals one of the specified individual files,
    //always include it regardless of rules
    for (int i = 0; i < individualFiles_.size(); i++) {
        if (file == individualFiles_[i].absoluteFilePath()) {
            allOk = true;
            anyOk = true;
            skipRules = true;
            break;
        }
    }

    //process script    
    if (!skipRules) {
        if( useScript ){
            QString errorLog;
            bool scriptOk = evaluateScript( tag, fileInfo, &errorLog );
            //qDebug()<<"scriptOk: "<<scriptOk;
            allOk = scriptOk; //both will be either false or true
            anyOk = scriptOk;
            if( !errorLog.isEmpty() ){
                //cancel generation if an error is found in the script
                *wasCanceled = true;
                log->append("\nError occurred in script: "+errorLog);
                return list;
            }
        }else{
            evaluateRules(tag, file, &allOk, &anyOk);
        }
    }
    //decide to include or not
    M3uEntry e;
    if ( (allRulesTrue_ && allOk) || (!allRulesTrue_ && anyOk) ) {
        //extinf info for m3u
        if (includeExtInf_) {
            e.setExtInf(createExtInfString(tag, file, format));
        }
        e.setOriginalFile(fileInfo);
        list.append(e);
    }
    if(!keepTags){
        delete tag;
    }

    return list;
}




/*!
 \brief

 \param tag
 \param fileInfo
 \param log
*/
bool PlayList::evaluateScript( Tag* tag, const QFileInfo& fileInfo, QString *log ) const {

    //qDebug()<<script_;
    if(script_.isEmpty()){
        return true;
    }

    QScriptEngine engine;

    //add all possible/specified id3v2 frames/ape items etc. to script, with empty array as value
    QHash< QString, QHash<QString,QStringList> > tagFrames = tag->frames(); //returns both ID3v2 frames and APE items
    QStringList frameTypes = frameFields.keys(); //defined list of possible frames
    for(int j=0;j<frameTypes.size();j++){
        QString type = frameTypes[j]; //e.g. APE, ID3V2 etc.
        QStringList frameTypeKeys = frameFields[type].toStringList();
        QScriptValue array = engine.newArray(frameTypeKeys.size());
        QHash<QString,QStringList> tagTypeFrames = tagFrames[type];
        QStringList tagTypeFramesKeys = tagTypeFrames.keys();
        //loop for instance all ID3V2 frames
        QScriptValue v = engine.newArray(0);
        for(int i=0;i<frameTypeKeys.size();i++){
            v.setProperty(0, QScriptValue(&engine, "")); //set default value an empty array
            array.setProperty(frameTypeKeys[i].toUpper(), v);
        }//now add all frames from current tag
        for(int i=0;i<tagTypeFramesKeys.size();i++){
            QStringList list = tagTypeFrames[tagTypeFramesKeys[i]];
            for(int k=0;k<list.size();k++){
                v.setProperty(k, list[k]);
                qDebug()<<type<<tagTypeFramesKeys[i]<<list[k];
            }
            array.setProperty(tagTypeFramesKeys[i].toUpper(), v);
        }
        engine.globalObject().setProperty(type,array);
    }

    QString containsFunctions = "\nfunction contains(a, str) { print(a.length); for ( i = 0; i < a.length; i++) { print(a[i]); if(a[i]==str){return true;} } return false; }\n";

    //PROBLEMET MED SCRIPT ER AT . IKKE ER TILLAT I VARIABEL NAVN! HUSK og SLETTE INI SETTINGS

    //add tag data  as variables to script
    engine.globalObject().setProperty("ARTIST",tag->artist());
    engine.globalObject().setProperty("ALBUM",tag->album());
    engine.globalObject().setProperty("GENRE",tag->genre());
    engine.globalObject().setProperty("TITLE",tag->title());
    engine.globalObject().setProperty("COMMENT",tag->comment());
    engine.globalObject().setProperty("YEAR",tag->year());
    engine.globalObject().setProperty("TRACK",tag->track());
    engine.globalObject().setProperty("LENGTH",tag->length());
    engine.globalObject().setProperty("BITRATE",tag->bitRate());
    engine.globalObject().setProperty("SAMPLERATE",tag->sampleRate());
    engine.globalObject().setProperty("CHANNELS",tag->channels());

    engine.globalObject().setProperty("FILENAME",fileInfo.fileName());
    engine.globalObject().setProperty("FILEPATH",fileInfo.filePath());    

    qDebug()<<"------------------------------evaluating script";
    QScriptValue result = engine.evaluate( containsFunctions+script_ );
    qDebug()<<"------------------------------end evaluating script";
    if( engine.hasUncaughtException() ){
        QString err = engine.uncaughtExceptionBacktrace().join("\n");
        log->append(err);
        return false;
    }
    bool res=false;
    if( result.isBool() ){
        res = result.toBool();
    }else{
        log->append("\nResult of evaluated script is not boolean");
    }
    return res;

}

/*!
 \brief

 \param tag
 \param file
 \param allOk
 \param anyOk
*/
void PlayList::evaluateRules( Tag* tag, const QString& file, bool* allOk, bool* anyOk) const {

    bool ok;
    for (int i = 0; i < rules_.size(); i++) {
        Rule r = rules_[i];
        Rule::RuleType t = r.type();
        bool shouldBeTrue = r.shouldBeTrue();
        Qt::CaseSensitivity s = Qt::CaseInsensitive;
        if (r.caseSensitive()) {
            s = Qt::CaseSensitive;
        }
        if (t == Rule::FILENAME_CONTAINS) {
            checkRule(file.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::FILENAME_EQUALS) {
            checkRule(file.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_TITLE_CONTAINS) {
            QString tmp = tag->title();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_TITLE_EQUALS) {
            QString tmp = tag->title();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_ARTIST_CONTAINS) {
            QString tmp = tag->artist();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_ARTIST_EQUALS) {
            QString tmp = tag->artist();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_ALBUM_CONTAINS) {
            QString tmp = tag->album();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_ALBUM_EQUALS) {
            QString tmp = tag->album();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_COMMENT_CONTAINS) {
            QString tmp = tag->comment();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_COMMENT_EQUALS) {
            QString tmp = tag->comment();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_GENRE_CONTAINS) {
            QString tmp = tag->genre();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_GENRE_EQUALS) {
            QString tmp = tag->genre();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_TRACK_IS) {
            int tmp = tag->track();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_YEAR_IS) {
            int tmp = tag->year();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::AUDIO_BITRATE_IS) {
            int tmp = tag->bitRate();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::AUDIO_SAMPLERATE_IS) {
            int tmp = tag->sampleRate();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::AUDIO_CHANNELS_IS) {
            int tmp = tag->channels();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::AUDIO_LENGTH_IS) {
            int tmp = tag->length();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
    }

}


/*!
 \brief

 \param tag
 \param file
 \param format_
 \return QString
*/
QString PlayList::createExtInfString(Tag *tag, const QString& file, const QString& format_) const {

    QString format = format_;

    if (tag->tagOk()) {
        QString artist = tag->artist();
        QString title = tag->title();
        QString album = tag->album();
        QString comment = tag->comment();
        QString genre = tag->genre();
        QString year = QString::number(tag->year());
        QString track = QString::number(tag->track());

        format.replace(QString("%artist%"), artist);
        format.replace(QString("%title%"), title);
        format.replace(QString("%album%"), album);
        format.replace(QString("%comment%"), comment);
        format.replace(QString("%genre%"), genre);
        format.replace(QString("%year%"), year);
        format.replace(QString("%track%"), track);
        format.replace(QString("%filename%"), file);

        int sum = artistEmpty+titleEmpty+albumEmpty+commentEmpty+genreEmpty+trackEmpty+yearEmpty;
        int sum2 = 0;
        if (artist.isEmpty() && artistEmpty) {
            sum2++;
        }
        if (title.isEmpty() && titleEmpty) {
            sum2++;
        }
        if (album.isEmpty() && albumEmpty) {
            sum2++;
        }
        if (comment.isEmpty() && commentEmpty) {
            sum2++;
        }
        if (genre.isEmpty() && genreEmpty) {
            sum2++;
        }
        if (track.isEmpty() && trackEmpty) {
            sum2++;
        }
        if (year.isEmpty() && yearEmpty) {
            sum2++;
        }
        if (sum2 == sum) {
            format = file;
        }

    }
    else {
        format = file;
    }

    QString result = QString("#EXTINF:") + QString::number(tag->length()) + "," + format;

    return result;
}

/*!
 \brief

 \param value
 \param allOk
 \param anyOk
 \param shouldBeTrue
*/
void PlayList::checkRule(bool value, bool* allOk, bool* anyOk, bool shouldBeTrue) const {
    if (value) {
        *allOk = *allOk && shouldBeTrue;
        *anyOk = shouldBeTrue;
    }
    else {
        *allOk = *allOk && !shouldBeTrue;
        *anyOk = !shouldBeTrue;
    }
}

/*!
 \brief

 \param intvals
 \param value
 \param allOk
 \param anyOk
 \param shouldBeTrue
*/
void PlayList::checkRange(const QVector<int> &intvals, int value, bool* allOk, bool* anyOk, bool shouldBeTrue) const {


    int n = intvals.size();
    if (n == 1) {
        if (value == intvals[0] && shouldBeTrue) {
            *allOk = *allOk && true;
            *anyOk = true;
        }
        else {
            *allOk = *allOk && false;
        }
    }
    else if (n == 2) {
        if (value >= intvals[0] && value <= intvals[1] && shouldBeTrue) {
            *allOk = *allOk && true;
            *anyOk = true;
        }
        else {
            *allOk = *allOk && false;
        }
    }

}


/*!
 \brief

 \return QString
*/
QString PlayList::name() const {
    return this->text();
}

/*!
 \brief

 \return QVector<Rule>
*/
QVector<Rule> PlayList::rules() const {
    return rules_;
}

/*!
 \brief

 \return QList<QDir>
*/
QList<QDir> PlayList::folders() const {
    return folders_;
}

/*!
 \brief

 \return QStringList
*/
QStringList PlayList::extensions() const {
    return extensions_;
}

/*!
 \brief

 \return bool
*/
bool PlayList::randomize() const {
    return randomize_;
}

/*!
 \brief

 \return bool
*/
bool PlayList::includeSubFolders() const {
    return includeSubFolders_;
}

/*!
 \brief

 \return bool
*/
bool PlayList::relativePath() const {
    return relativePath_;
}

/*!
 \brief

 \return bool
*/
bool PlayList::allRulesTrue() const {
    return allRulesTrue_;
}

/*!
 \brief

 \return bool
*/
bool PlayList::includeExtInf() const {
    return includeExtInf_;
}

/*!
 \brief

 \return bool
*/
bool PlayList::makeUnique() const {
    return makeUnique_;
}

/*!
 \brief

 \return QDir
*/
QDir PlayList::copyFilesToDir() const {
    return copyFilesToDir_;
}

/*!
 \brief

 \return bool
*/
bool PlayList::copyFiles() const {
    return copyFiles_;
}

/*!
 \brief

 \return QList<QFileInfo>
*/
QList<QFileInfo> PlayList::individualFiles() const {
    return individualFiles_;
}

/*!
 \brief

 \return QString
*/
QString PlayList::script() const {
    return script_;
}


/*!
 \brief

 \param name
*/
void PlayList::setName(const QString& name) {
    this->setText(name);
}

/*!
 \brief

 \param rules
*/
void PlayList::setRules(const QVector<Rule> &rules) {
    rules_ = rules;
}

/*!
 \brief

 \param folders
*/
void PlayList::setFolders(const QList<QDir> &folders) {
    folders_ = folders;
}

/*!
 \brief

 \param extensions
*/
void PlayList::setExtensions(const QStringList& extensions) {
    extensions_ = extensions;
}

/*!
 \brief

 \param randomize
*/
void PlayList::setRandomize(bool randomize) {
    randomize_ = randomize;
}

/*!
 \brief

 \param includeSubFolders
*/
void PlayList::setIncludeSubFolders(bool includeSubFolders) {
    includeSubFolders_ = includeSubFolders;
}

/*!
 \brief

 \param relativePath
*/
void PlayList::setRelativePath(bool relativePath) {
    relativePath_ = relativePath;
}

/*!
 \brief

 \param allRulesTrue
*/
void PlayList::setAllRulesTrue(bool allRulesTrue) {
    allRulesTrue_ = allRulesTrue;
}

/*!
 \brief

 \param includeExtInf
*/
void PlayList::setIncludeExtInf(bool includeExtInf) {
    includeExtInf_ = includeExtInf;
}

/*!
 \brief

 \param makeUnique
*/
void PlayList::setMakeUnique(bool makeUnique) {
    makeUnique_ = makeUnique;
}


/*!
 \brief

 \param copyFilesToDir
*/
void PlayList::setCopyFilesToDir(const QDir& copyFilesToDir) {
    copyFilesToDir_ = copyFilesToDir;
}

/*!
 \brief

 \param copyFiles
*/
void PlayList::setCopyFiles(bool copyFiles) {
    copyFiles_ = copyFiles;
}

/*!
 \brief

 \param individualFiles
*/
void PlayList::setIndividualFiles(const QList<QFileInfo> &individualFiles) {
    individualFiles_ = individualFiles;
}

/*!
 \brief

 \param script
*/
void PlayList::setScript(const QString& script) {
    script_ = script;
}


/*!
 \brief

 \return PlayList::operator
*/
PlayList::operator QVariant() const {
    return QVariant::fromValue(*this);
}

/*!
 \brief

 \param in
 \param p
 \return QDataStream & operator >>
*/
QDataStream& operator>>(QDataStream& in, PlayList& p) {

    bool ok;
    QString log;
    QString version = Global::versionCheck(&in, &ok, &log);
    if (!ok) {
        QMessageBox::critical(0, "", log);
    }

    if (version != "1.0") {
        in.setStatus(QDataStream::ReadCorruptData);
        return in;
    }

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

    in >> name_ >> rules_ >> folders_ >> extensions_ >> randomize_ >> includeSubFolders >> relativePath_ >> allRulesTrue_;
    in >> includeExtInf_ >> makeUnique_;
    in >> copyFilesToDir >> copyFiles;
    in >> individualFiles_ >> script_;

    QList<QDir> folders;
    for (int i = 0; i < folders_.size(); i++) {
        folders.append(QDir(folders_[i]));
    }
    QList<QFileInfo> individualFiles;
    for (int i = 0; i < individualFiles_.size(); i++) {
        individualFiles.append(QFileInfo(individualFiles_[i]));
    }

    p = PlayList();
    p.setName(name_);
    p.setRules(rules_);
    p.setFolders(folders);
    p.setExtensions(extensions_);
    p.setRandomize(randomize_);
    p.setIncludeSubFolders(includeSubFolders);
    p.setRelativePath(relativePath_);
    p.setAllRulesTrue(allRulesTrue_);
    p.setIncludeExtInf(includeExtInf_);
    p.setMakeUnique(makeUnique_);
    p.setCopyFilesToDir(QDir(copyFilesToDir));
    p.setCopyFiles(copyFiles);
    p.setIndividualFiles(individualFiles);
    p.setScript(script_);
    return in;
}

/*!
 \brief

 \param out
 \param p
 \return QDataStream & operator
*/
QDataStream& operator<<(QDataStream& out, const PlayList& p) {

    out << out.version();
    out << qApp->applicationVersion();

    QStringList folders;
    QList<QDir> folders_ = p.folders();
    for (int i = 0; i < folders_.size(); i++) {
        folders.append(folders_[i].absolutePath());
    }
    QStringList individualFiles;
    QList<QFileInfo> individualFiles_ = p.individualFiles();
    for (int i = 0; i < individualFiles_.size(); i++) {
        individualFiles.append(individualFiles_[i].absoluteFilePath());
    }
    out << p.name() << p.rules() << folders << p.extensions() << p.randomize() << p.includeSubFolders();
    out << p.relativePath() << p.allRulesTrue() << p.includeExtInf() << p.makeUnique() << p.copyFilesToDir().absolutePath();
    out << p.copyFiles() << individualFiles << p.script();
    return out;
}

