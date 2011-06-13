
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

/*
PlayList::~PlayList(){
    if(guiSettings!=0){
        delete guiSettings;
        guiSettings=0;
    }
}
*/

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
    qDebug() << "original file: " << file.absoluteFilePath();
    QString playListEntryName = file.absoluteFilePath();

    bool useCopyFilesToPath = guiSettings->value("useCopyFilesToPath").toBool();
    if (copyFiles_ && useCopyFilesToPath) {
        playListEntryName = copyFilesToDir_.absolutePath() + "/" + file.fileName();
        bool keepFolderStructure = guiSettings->value("keepFolderStructure").toBool();
        if (keepFolderStructure) {
            qDebug()<<"file.absolutePath() "<<file.absolutePath();
            QStringList dirs = file.absolutePath().replace("\\", "/").split("/");
            QString root = dirs[0];
            qDebug()<<"root: "<<root;
            qDebug()<<"copyFilesToDir_.absolutePath() "<<copyFilesToDir_.absolutePath();
            playListEntryName = file.absoluteFilePath().replace(root, copyFilesToDir_.absolutePath());

        }
    }

    if (relativePath_) {
        playListEntryName = outPutPath.relativeFilePath(playListEntryName);
    }

    qDebug() << "playListEntryName: " << playListEntryName;

    return playListEntryName;
}


/*!
 \brief

 \param songs
 \param log
*/
void PlayList::copyFoundFiles(QList<M3uEntry> songs, QString* log) {

    log->append("\nResult from file copy:\n");
    int nCopied = 0;

    bool ok = true;

    //check if main directory to copy files to exist.
    //if not, try to create it
    QDir c(copyFilesToDir_);
    if (!c.exists()) {
        ok = c.mkpath(copyFilesToDir_.absolutePath());
        if (ok) {
            log->append("Created directory " + copyFilesToDir_.absolutePath() + "\n");
        }
        else {
            log->append("Could not create directory " + copyFilesToDir_.absolutePath() + ", no copying performed\n");
            return;
        }
    }

    QTime time;
    time.start();

    //set the use of relative paths to false while copying, to
    //get the absolute name from the playListEntry function
    bool originalRelativePath_ = relativePath_;
    relativePath_ = false;

    qDebug() << "starting to copy " << songs.size() << " files!";
    QProgressDialog pr("Copying files for playlist " + name() + " to " + copyFilesToDir_.absolutePath(), "Abort", 0, songs.size(), 0);
    pr.setWindowModality(Qt::WindowModal);
    QPushButton* cancelButton = new QPushButton;
    pr.setCancelButton(cancelButton);
    pr.setCancelButtonText("Cancel");
    pr.setLabelText("Copying files for playlist "+name());
    for (int j = 0; j < songs.size(); j++) {
        pr.setValue(j);
        if (pr.wasCanceled()) {
            break;
        }

        QString playListEntryName = playListEntry(songs[j]);
        QFileInfo fi(playListEntryName);
        if (!fi.absoluteDir().exists()) {
            QDir dir;
            bool createPathOk = dir.mkpath(fi.dir().absolutePath());
            if (!createPathOk) {
                log->append("\nCould not create path: " + fi.dir().absolutePath());
                continue;
            }else{
                qDebug()<<"Created path "<<fi.dir().absolutePath();
            }
        }


        QFile f(songs[j].originalFile().absoluteFilePath());
        //pr.setLabelText("Copying "+songs[j].originalFile().absoluteFilePath()+" to\n"+playListEntryName);
        bool okf = f.copy(playListEntryName);
        if (!okf) {
            QFile f2(playListEntryName);
            if (f2.exists()) {
                log->append(songs[j].originalFile().filePath() + " was not copied as it already exists in " + copyFilesToDir_.absolutePath() + "\n");
            }
            else {
                log->append(songs[j].originalFile().filePath() + " could not be copied to " + copyFilesToDir_.absolutePath() + "\n");
            }
        }
        else {
            nCopied++;
        }
    }
    pr.setValue(songs.size());
    pr.close();

    int secs = time.elapsed();

    log->append(QString::number(nCopied) + " of " + QString::number(songs.size()) + " files copied to " + copyFilesToDir_.absolutePath() + "\n");
    log->append("Time used copying files: "+QString::number(secs)+" seconds\n");

    relativePath_ = originalRelativePath_; // set the use of relative paths back
}


/*!
 \brief

 \param songsOut
 \param log
 \param QHash<QString
 \param tags
 \return bool
*/
bool PlayList::generate(QList<M3uEntry> *songsOut, QString* log, QHash<QString, Tag> *tags) {


    time_t Start_t, End_t;
    int totaltime;
    Start_t = time(0);

    bool canceled = false;
    QList<M3uEntry> songs = findFiles(&canceled, log, tags);

    songsOut->append(songs);
    if (canceled) {
        return canceled;
    }

    if (songs.isEmpty()) {
        QMessageBox::information(0, "",
                                 "No songs matching your criteria were found for playlist " + name(),
                                 QMessageBox::Ok, QMessageBox::Ok);
        End_t = time(0);    //record time that task 1 ends
        totaltime = difftime(End_t, Start_t);    //compute elapsed time of task 1
        log->append("\n Found " + QString::number(songs.size()) + " songs. Time used: " + QString::number(totaltime) + " seconds\n");
        return true;
    }

    QString outPutPath = guiSettings->value("outPutPath").toString();
    QString file = outPutPath + "/" + name() + ".m3u";
    writeM3U(file, songs, log);

    End_t = time(0);    //record time that task 1 ends
    totaltime = difftime(End_t, Start_t);    //compute elapsed time of task 1
    std::cout << "time used: " << totaltime << std::endl;

    log->append("\n Found " + QString::number(songs.size()) + " songs for " + name() + ". Time used: " + QString::number(totaltime) + " seconds\n");
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
QList<M3uEntry> PlayList::findFiles(bool* canceled, QString* log, QHash<QString, Tag> *tags) const {

    qDebug() << "finding files...";

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

    if (folders_.size() == 0) {
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
        fileInfo = fileInfo + getDirContent(folders_[i].absolutePath());
    }
    //make unique
    fileInfo = fileInfo.toSet().toList();

    //local copy of QHash tags: new read tags are inserted into global tags. used files are removed from local qhash tags to increase lookup speed
    QHash<QString, Tag> tagsCopy = *tags;

    int n = fileInfo.size();
    QList<M3uEntry> tmplist;
    QProgressDialog p("Locating files for playlist " + name(), "Abort", 0, n, 0);
    p.setWindowModality(Qt::WindowModal);


    QPushButton* cancelButton = new QPushButton;
    p.setCancelButton(cancelButton);
    p.setCancelButtonText("Cancel");
    p.setLabelText("Locating files for " + name());
    bool wasCanceled = false;

    qDebug() << "found " << n << " files to process";


    for (int i = 0; i < n; i++) {
        p.setValue(i);
        if (p.wasCanceled()) {
            wasCanceled = true;
            break;
        }

        tmplist = processFile(fileInfo[i], hasTagRule, hasAudioRule, log, tags, &tagsCopy, &wasCanceled );

        if (wasCanceled) {
            break;
        }
        plist = plist + tmplist;
    }

    p.setValue(n);
    if (wasCanceled) {
        *canceled = true;
        return plist;
    }

    if (randomize_) {
        std::random_shuffle(plist.begin(), plist.end());
    }

    qDebug() << "finished finding files";

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

 \param aPath
 \return QList<QFileInfo>
*/
QList<QFileInfo> PlayList::getDirContent(const QString& aPath) const {

    // append the filtered files to this list

    //decide to include subfolder or not
    QDirIterator::IteratorFlag subdirFlag;
    if (includeSubFolders_) {
        subdirFlag = QDirIterator::Subdirectories;
    }
    else {
        subdirFlag = QDirIterator::NoIteratorFlags;
    }

    // set dir iterator
    QDirIterator* dirIterator = new QDirIterator(aPath,
            extensions_,
            QDir::Files | QDir::NoSymLinks,
            subdirFlag);


    QList<QFileInfo> fileInfo;
    while (dirIterator->hasNext()) {
        dirIterator->next();
        fileInfo.append(dirIterator->fileInfo());
    }

    delete dirIterator;
    qDebug() << "Finished!";
    return fileInfo;
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
QList<M3uEntry>  PlayList::processFile(const QFileInfo& fileInfo, bool hasTagRule, bool hasAudioRule, QString* log, QHash<QString, Tag> *tags, QHash<QString, Tag> *tagsCopy, bool *wasCanceled ) const {

    QList<M3uEntry> list;

    QString file = fileInfo.fileName();
    QString fullfile = fileInfo.absoluteFilePath();

    Tag tag;
    if (hasTagRule || hasAudioRule || includeExtInf_) {
        //take tag from the inital copy of tags. If it doesnt exist there, read it, and insert into
        //the original qhash of tags
        tag = tagsCopy->take(fullfile);
        if (tag.fileName().isEmpty()) {
            tag = Tag(fullfile);
            tag.readTags();
            if (guiSettings->value("keepTags").toBool()) {
                tags->insert(fullfile, tag);
            }
        }
        if (!tag.tagOk()) {
            log->append("Could not read tag for " + fullfile + "\n");
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
    //added. All rules_ ignored.
    bool skipRules = false;
    if (guiSettings->value("useScript").toBool() && script_.isEmpty()) {
        skipRules = true;
    }
    for (int i = 0; i < individualFiles_.size(); i++) {
        if (file == individualFiles_[i].absoluteFilePath()) {
            allOk = true;
            anyOk = true;
            skipRules = true;
            break;
        }
    }
    bool useScript = guiSettings->value("useScript").toBool();
    if (!skipRules) {
        if( useScript ){
            QString errorLog;
            bool scriptOk = evaluateScript( tag, fileInfo, &errorLog );
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
            e.setExtInf(createExtInfString(tag, file, guiSettings->value("format").toString()));
        }
        e.setOriginalFile(fileInfo);
        list.append(e);
    }

    return list;
}

/*!
 \brief

 \param tag
 \param fileInfo
 \param log
*/
bool PlayList::evaluateScript( const Tag& tag, const QFileInfo& fileInfo, QString *log ) const {

    if(script_.isEmpty()){
        return true;
    }

    QScriptEngine engine;

    engine.globalObject().setProperty("ARTIST",tag.artist());
    engine.globalObject().setProperty("ALBUM",tag.album());
    engine.globalObject().setProperty("GENRE",tag.genre());
    engine.globalObject().setProperty("TITLE",tag.title());
    engine.globalObject().setProperty("COMMENT",tag.comment());
    engine.globalObject().setProperty("YEAR",tag.year());
    engine.globalObject().setProperty("TRACK",tag.track());
    engine.globalObject().setProperty("LENGTH",tag.length());
    engine.globalObject().setProperty("BITRATE",tag.bitRate());
    engine.globalObject().setProperty("SAMPLERATE",tag.sampleRate());
    engine.globalObject().setProperty("CHANNELS",tag.channels());

    engine.globalObject().setProperty("FILENAME",fileInfo.fileName());
    engine.globalObject().setProperty("FILEPATH",fileInfo.filePath());

    QScriptValue result = engine.evaluate( script_ );
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
void PlayList::evaluateRules(const Tag& tag, const QString& file, bool* allOk, bool* anyOk) const {

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
            QString tmp = tag.title();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_TITLE_EQUALS) {
            QString tmp = tag.title();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_ARTIST_CONTAINS) {
            QString tmp = tag.artist();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_ARTIST_EQUALS) {
            QString tmp = tag.artist();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_ALBUM_CONTAINS) {
            QString tmp = tag.album();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_ALBUM_EQUALS) {
            QString tmp = tag.album();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_COMMENT_CONTAINS) {
            QString tmp = tag.comment();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_COMMENT_EQUALS) {
            QString tmp = tag.comment();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_GENRE_CONTAINS) {
            QString tmp = tag.genre();
            checkRule(tmp.contains(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_GENRE_EQUALS) {
            QString tmp = tag.genre();
            checkRule(tmp.compare(r.value(), s), allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_TRACK_IS) {
            int tmp = tag.track();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::TAG_YEAR_IS) {
            int tmp = tag.year();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::AUDIO_BITRATE_IS) {
            int tmp = tag.bitRate();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::AUDIO_SAMPLERATE_IS) {
            int tmp = tag.sampleRate();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::AUDIO_CHANNELS_IS) {
            int tmp = tag.channels();
            QVector<int> intvals;
            ok = Global::checkIntValue(&intvals, r.value());
            checkRange(intvals, tmp, allOk, anyOk, shouldBeTrue);
        }
        else if (t == Rule::AUDIO_LENGTH_IS) {
            int tmp = tag.length();
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
QString PlayList::createExtInfString(const Tag& tag, const QString& file, const QString& format_) const {

    QString format = format_;

    if (tag.tagOk()) {
        QString artist = tag.artist();
        QString title = tag.title();
        QString album = tag.album();
        QString comment = tag.comment();
        QString genre = tag.genre();
        QString year = QString::number(tag.year());
        QString track = QString::number(tag.track());

        format.replace(QString("%artist%"), artist);
        format.replace(QString("%title%"), title);
        format.replace(QString("%album%"), album);
        format.replace(QString("%comment%"), comment);
        format.replace(QString("%genre%"), genre);
        format.replace(QString("%year%"), year);
        format.replace(QString("%track%"), track);
        format.replace(QString("%filename%"), file);

        int sum = guiSettings->value("artistEmpty").toBool() + guiSettings->value("titleEmpty").toBool() + guiSettings->value("albumEmpty").toBool() \
                  + guiSettings->value("commentEmpty").toBool() + guiSettings->value("genreEmpty").toBool() + guiSettings->value("trackEmpty").toBool() \
                  + guiSettings->value("yearEmpty").toBool();
        int sum2 = 0;
        if (artist.isEmpty() && guiSettings->value("artistEmpty").toBool()) {
            sum2++;
        }
        if (title.isEmpty() && guiSettings->value("titleEmpty").toBool()) {
            sum2++;
        }
        if (album.isEmpty() && guiSettings->value("albumEmpty").toBool()) {
            sum2++;
        }
        if (comment.isEmpty() && guiSettings->value("commentEmpty").toBool()) {
            sum2++;
        }
        if (genre.isEmpty() && guiSettings->value("genreEmpty").toBool()) {
            sum2++;
        }
        if (track.isEmpty() && guiSettings->value("trackEmpty").toBool()) {
            sum2++;
        }
        if (year.isEmpty() && guiSettings->value("yearEmpty").toBool()) {
            sum2++;
        }
        if (sum2 == sum) {
            format = file;
        }

    }
    else {
        format = file;
    }

    QString result = QString("#EXTINF:") + QString::number(tag.length()) + "," + format;

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
QString PlayList::fileNameWithoutExtension() const {
    return guiSettings->value("outPutPath").toString() + "/" + name();
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

