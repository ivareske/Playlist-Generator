
#include "PlaylistManager.h"


/*!
 \brief

 \param parent
*/
PlaylistManager::PlaylistManager(QWidget* parent) : QMainWindow(parent) {
    setupUi(this); // this sets up GUI
    guiSettings = Global::guiSettings();

    if(!ScriptOnlyFrame->layout()){
        ScriptOnlyFrame->setLayout(new QVBoxLayout);
    }
    scriptEdit = new CodeEditor(this);    
    ScriptOnlyFrame->layout()->addWidget(scriptEdit);

    RuleScript = new CodeEditor(this);    
    if(!rulesGroupBox->layout()){
        rulesGroupBox->setLayout(new QVBoxLayout);
    }
    rulesGroupBox->layout()->addWidget(RuleScript);
    folderTable = new SimpleTextEdit(this);
    if(!folderFrame->layout()){
        folderFrame->setLayout(new QGridLayout);
    }


    console_ = new QTextEdit(ConsoleFrame);
    console_->setMinimumHeight(50);
    //console_->setTextFormat(Qt::LogText);
    console_->setReadOnly(true);
    if(!ConsoleFrame->layout()){
        ConsoleFrame->setLayout(new QVBoxLayout);
    }       
    ConsoleFrame->layout()->addWidget(console_);    


    folderFrame->layout()->addWidget(folderTable);
    QString tip = "The result of the script should return true or false, to determine if the file being processed should be included or not.\n";
    tip += "Available parameters:\n";
    tip += "String: FILENAME, FILEPATH, ARTIST, ALBUM, TITLE, GENRE, COMMENT\n";
    tip += "Unsigned int: YEAR, TRACK, LENGTH (seconds), BITRATE (kb/s), SAMPLERATE (Hz), CHANNELS \n";
    tip += "Boolean: TAGOK, AUDIOPROPERTIESOK\n";
    tip += "QHash<QString,QStringList>: ID3V2, ASF, MP4, XIPH, APE\n";
    tip += "Example to return all Beatles or Elvis music with length between 120 and 200 seconds, and a high bitrate (>300 kb/s):\n";
    tip += "LENGTH>120 && LENGTH<200 && BITRATE>300 && (ARTIST==\"Beatles\" || ARTIST==\"Elvis\")\n";
    tip += "\nAvailable ID3v2/XIPH/APE/ASF/MP4 frame/item keys for the script can be edited in the settings dialog.";
    tip += "\nExample how to use ID3v2/XIPH/APE/ASF/MP4 frame/items:";
    tip += "\nID3V2[\"TPE1\"]==\"Beatles\"\n\n";
    QStringList completions;
    QString functionTips = scriptFunctionsHelpText(&completions);
    tip.append(functionTips);
    tip += "\n\nNote: If you create a string variable named \"EXTINF\" in the script, this will be used as EXTINF in the playlist.";
    RuleScript->setToolTip(tip);
    completions<<"FILENAME"<<"FILEPATH"<<"ARTIST"<<"ALBUM"<<"TITLE"<<"GENRE"<<"COMMENT"<<\
                                         "YEAR"<<"TRACK"<<"LENGTH"<<"BITRATE"<<"SAMPLERATE"<<"CHANNELS"<<"ID3V2"<<"ASF"\
                                      <<"MP4"<<"XIPH"<<"APE"<<"TAGOK"<<"AUDIOPROPERTIESOK";
    RuleScript->addCompletionWords(completions);

    createActions();

    qRegisterMetaType<PlayList>("PlayList");
    qRegisterMetaTypeStreamOperators<PlayList>("PlayList");    
    qRegisterMetaType<PlayListCollection>("PlayListCollection");
    qRegisterMetaTypeStreamOperators<PlayListCollection>("PlayListCollection");


    //playListCollection=0;    
    initGuiSettings();

    readGUISettings();
    guiModeChanged();
    initializeScriptEngine();

    if(guiSettings->value("debugScript").toBool() && this->isVisible()){
        scriptDebugger_.attachTo(&engine_);
        //scriptDebugger_.setAutoShowStandardWindow(false);
    }

}

PlaylistManager::~PlaylistManager(){
    delete guiSettings;guiSettings=0;
}

/*!
 \brief

*/
void PlaylistManager::editStyleDialog() {

    QString style = guiSettings->value("style").toString();
    StyleSheetEditor s(style, this);
    QString styleSheet = guiSettings->value("styleSheet").toString();
    s.setStyleSheetText(styleSheet);
    s.exec();
    guiSettings->setValue("style", s.currentStyle());
    guiSettings->setValue("styleSheet", s.styleSheetText());
    guiSettings->sync();
}

/*!
 \brief

*/
void PlaylistManager::initGuiSettings() {

    if (guiSettings == 0) {
        guiSettings = Global::guiSettings();
    }

    guiSettings->setCheckForEmptyValue(false);

    if (!guiSettings->value("frameFields").canConvert(QVariant::Hash)) {
        QHash<QString,QVariant> frameFields;
        QStringList ID3v2Fields;
        ID3v2Fields<<"AENC"<<"APIC"<<"COMM"<<"COMR"<<"ENCR"<<"EQUA"<<"ETCO"<<"GEOB"<<"GRID"<<"IPLS"<<"LINK"<<"MCDI"<<"MLLT"<<"OWNE" \
                   <<"PRIV"<<"PCNT"<<"POPM"<<"POSS"<<"RBUF"<<"RVAD"<<"RVRB"<<"SYLT"<<"SYTC"<<"TALB"<<"TBPM"<<"TCOM"<<"TCON"<<"TCOP" \
                   <<"TDAT"<<"TDLY"<<"TENC"<<"TEXT"<<"TFLT"<<"TIME"<<"TIT1"<<"TIT2"<<"TIT3"<<"TKEY"<<"TLAN"<<"TLEN"<<"TMED"<<"TOAL"<< \
                      "TOFN"<<"TOLY"<<"TOPE"<<"TORY"<<"TOWN"<<"TPE1"<<"TPE2"<<"TPE3"<<"TPE4"<<"TPOS"<<"TPUB"<<"TRCK"<<"TRDA"<<"TRSN"<<"TRSO"\
                   <<"TSIZ"<<"TSRC"<<"TSSE"<<"TYER"<<"TXXX"<<"UFID"<<"USER"<<"USLT"<<"WCOM"<<"WCOP"<<"WOAF"<<"WOAR"<<"WOAS"<<"WORS"<<\
                      "WPAY"<<"WPUB"<<"WXXX"<<"ASPI"<<"EQU2"<<"RVA2"<<"SEEK"<<"SIGN"<<"TDEN"<<"TDOR"<<"TDRC"<<"TDRL"<<"TDTG"<<"TIPL"<<\
                      "TMCL"<<"TMOO"<<"TPRO"<<"TSOA"<<"TSOP"<<"TSOT"<<"TSST";
        frameFields.insert("ID3V2",ID3v2Fields);

        QStringList apeItemKeys;apeItemKeys<<"Title"<<"Subtitle"<<"Artist"<<"Album"<<"Debut"<<"album"<<"Publisher"<<\
                                              "Conductor"<<"Track"<<"Composer"<<"Comment"<<"Copyright"<<"Publicationright"<<"File"<<"EAN/UPC"<<"ISBN"<<\
                                              "Catalog"<<"LC"<<"Year"<<"Record Date"<<"Record Location"<<"Genre"<<"Media"<<"Index"<<"Related"<<"ISRC"<<"Abstract"\
                                           <<"Language"<<"Bibliography"<<"Introplay"<<"Dummy";
        frameFields.insert("APE",apeItemKeys);

        QStringList xiphFields;xiphFields<<"TITLE"<<"VERSION"<<"ALBUM"<<"TRACKNUMBER"<<"ARTIST"<<"PERFORMER"<<"COPYRIGHT"<<\
                                            "LICENSE"<<"ORGANIZATION"<<"DESCRIPTION"<<"GENRE"<<"DATE"<<"LOCATION"<<"CONTACT"<<"ISRC";
        frameFields.insert("XIPH",xiphFields);

        QStringList mp4Fields;mp4Fields<<ID3v2Fields<<"RTNG"<<"CNID"<<"PGAP"<<"HDVD"<<"STIK"<<"TVES"<<"TVEN"<<"TVNN"<<"TVSN"<<"TVSH"<<"SOSN";
        frameFields.insert("MP4",mp4Fields);

        QStringList asfFields;asfFields<<"AudioCodecDescription"<<"AudioCodecName"<<"OtherCodecDescription"<<"OtherCodecName"<<"VideoCodecDescription"<<"VideoCodecName"<<\
                                          "BannerImageType"<<"BannerImage"<<"BannerImageURL"<<"CopyrightURL"<<"Title"<<"Author"<<"Copyright"<<"Description"<<"Rating"<<\
                                          "ASFLeakyBucketPairs"<<"ASFPacketCount"<<"ASFSecurityObjectsSize"<<"AlbumArtist"<<"AlbumCoverURL"<<"AlbumTitle"<<"AspectRatioX"<<\
                                          "AspectRatioY"<<"AudioFileURL"<<"AudioSourceURL"<<"Author"<<"AuthorURL"<<"AverageLevel"<<"BannerImageData"<<"BannerImageType"<<\
                                          "BannerImageURL"<<"BeatsPerMinute"<<"Bitrate"<<"Broadcast"<<"BufferAverage"<<"Can_Skip_Backward"<<"Can_Skip_Forward"<<"Category"<<\
                                          "Codec"<<"Composer"<<"Conductor"<<"ContainerFormat"<<"ContentDistributor"<<"ContentGroupDescription"<<"Copyright"<<"CopyrightURL"<<\
                                          "CurrentBitrate"<<"DRM"<<"DRM_ContentID"<<"DRM_DRMHeader"<<"DRM_DRMHeader_ContentDistributor"<<"DRM_DRMHeader_ContentID"<<\
                                          "DRM_DRMHeader_IndividualizedVersion"<<"DRM_DRMHeader_KeyID"<<"DRM_DRMHeader_LicenseAcqURL"<<"DRM_DRMHeader_SubscriptionContentID"<<\
                                          "DRM_IndividualizedVersion"<<"DRM_KeyID"<<"DRM_LASignatureCert"<<"DRM_LASignatureLicSrvCert"<<"DRM_LASignaturePrivKey"<<\
                                          "DRM_LASignatureRootCert"<<"DRM_LicenseAcqURL"<<"DRM_V1LicenseAcqURL"<<"DVDID"<<"Description"<<"Director"<<"Duration"<<"EncodedBy"<<\
                                          "EncodingSettings"<<"EncodingTime"<<"FileSize"<<"Genre"<<"GenreID"<<"HasArbitraryDataStream"<<"HasAttachedImages"<<"HasAudio"<<\
                                          "HasFileTransferStream"<<"HasImage"<<"HasScript"<<"HasVideo"<<"ISRC"<<"InitialKey"<<"IsVBR"<<"Is_Protected"<<"Is_Trusted"<<\
                                          "Language"<<"Lyrics"<<"Lyrics_Synchronised"<<"MCDI"<<"MediaClassPrimaryID"<<"MediaClassSecondaryID"<<"MediaCredits"<<\
                                          "MediaIsDelay"<<"MediaIsFinale"<<"MediaIsLive"<<"MediaIsPremiere"<<"MediaIsRepeat"<<"MediaIsSAP"<<"MediaIsStereo"<<\
                                          "MediaIsSubtitled"<<"MediaIsTape"<<"MediaNetworkAffiliation"<<"MediaOriginalBroadcastDateTime"<<"MediaOriginalChannel"<<\
                                          "MediaStationCallSign"<<"MediaStationName"<<"ModifiedBy"<<"Mood"<<"NSC_Address"<<"NSC_Description"<<"NSC_Email"<<\
                                          "NSC_Name"<<"NSC_Phone"<<"NumberOfFrames"<<"OptimalBitrate"<<"OriginalAlbumTitle"<<"OriginalArtist"<<"OriginalFilename"<<\
                                          "OriginalLyricist"<<"OriginalReleaseTime"<<"OriginalReleaseYear"<<"ParentalRating"<<"ParentalRatingReason"<<"PartOfSet"<<\
                                          "PeakBitrate"<<"PeakValue"<<"Period"<<"PlaylistDelay"<<"Producer"<<"PromotionURL"<<"ProtectionType"<<"Provider"<<\
                                          "ProviderCopyright"<<"ProviderRating"<<"ProviderStyle"<<"Publisher"<<"RadioStationName"<<"RadioStationOwner"<<\
                                          "Rating"<<"Seekable"<<"SharedUserRating"<<"Signature_Name"<<"StreamTypeInfo"<<"Stridable"<<"SubTitle"<<\
                                          "SubTitleDescription"<<"SubscriptionContentID"<<"Text"<<"Title"<<"ToolName"<<"ToolVersion"<<"Track"<<\
                                          "TrackNumber"<<"UniqueFileIdentifier"<<"UserWebURL"<<"VBRPeak"<<"VideoClosedCaptioning"<<"VideoFrameRate"<<\
                                          "VideoHeight"<<"VideoWidth"<<"WMADRCAverageReference"<<"WMADRCAverageTarget"<<"WMADRCPeakReference"<<\
                                          "WMADRCPeakTarget"<<"WMCollectionGroupID"<<"WMCollectionID"<<"WMContentID"<<"Writer"<<"Year";
        frameFields.insert("ASF",asfFields);
        guiSettings->setValue("frameFields", frameFields );
    }

    if (!guiSettings->value("scriptType").canConvert(QVariant::Int)) {
        guiSettings->setValue("scriptType",(int)Global::SCRIPTANDPLAYLIST );
    }
    if (!guiSettings->value("styleSheet").canConvert(QVariant::String)) {
        guiSettings->setValue("styleSheet", "");
    }
    if (!guiSettings->value("style").canConvert(QVariant::String)) {
        guiSettings->setValue("style", qApp->style()->metaObject()->className());
    }        
    if (!guiSettings->value("defaultExtensions").canConvert(QVariant::List)) {
        QStringList defExts;
        defExts << "*.mp3" << "*.wma" << "*.wav" << "*.ogg" << "*.aac" << "*.ac3";
        guiSettings->setValue("defaultExtensions", defExts);
    }
    if (!guiSettings->value("format").canConvert(QVariant::String)) {
        guiSettings->setValue("format", "%artist% - %title%");
    }
    if (!guiSettings->value("UseFilenameAsExtInfIfNoTag").canConvert(QVariant::Bool)) {
        guiSettings->setValue("format", true);
    }
    if (!guiSettings->value("showLog").canConvert(QVariant::Bool)) {
        guiSettings->setValue("showLog", true);
    }
    if (!guiSettings->value("debugScript").canConvert(QVariant::Bool)) {
        guiSettings->setValue("debugScript", true);
    }
    if (!guiSettings->value("useCopyFilesToPath").canConvert(QVariant::Bool)) {
        guiSettings->setValue("useCopyFilesToPath", false);
    }
    if (!guiSettings->value("keepFolderStructure").canConvert(QVariant::Bool)) {
        guiSettings->setValue("keepFolderStructure", true);
    }
    if (!guiSettings->value("overWriteFiles").canConvert(QVariant::Bool)) {
        guiSettings->setValue("overWriteFiles", true);
    }
    if (!guiSettings->value("keepTags").canConvert(QVariant::Bool)) {
        guiSettings->setValue("keepTags", true);
    }
    if (!guiSettings->value("copyFilesToDir").canConvert(QVariant::String)) {
        guiSettings->setValue("copyFilesToDir", "");
    }

    guiSettings->setCheckForEmptyValue(true);
    guiSettings->sync();

}

void PlaylistManager::runScriptEditScript() {
    runScript(scriptEdit->toPlainText());
}


/*!
 \brief

*/
void PlaylistManager::createActions() {


    connect(runScriptButton,SIGNAL(clicked()),this,SLOT(runScriptEditScript()));

    // signals/slots mechanism in action
    connect(generateButton, SIGNAL(clicked()), this, SLOT(generateSelectedPlayLists()));
    connect(generateAllButton, SIGNAL(clicked()), this, SLOT(generateAllPlayLists()));
    connect(addPlayListButton, SIGNAL(clicked()), this, SLOT(addPlayList()));
    connect(removePlayListButton, SIGNAL(clicked()), this, SLOT(removePlayList()));    
    connect(addFolderButton, SIGNAL(clicked()), this, SLOT(addFolder()));    
    connect(addFilesButton, SIGNAL(clicked()), this, SLOT(addIndividualFiles()));

    connect(playListTable, SIGNAL(itemSelectionChanged()), this, SLOT(showRulesAndFolders()));

    //playlist settings
    connect(folderTable, SIGNAL(editingFinished()), this, SLOT(updatePlayList()));
    connect(extensions, SIGNAL(editingFinished()), this, SLOT(updatePlayList()));
    connect(RuleScript, SIGNAL(editingFinished()), this, SLOT(updatePlayList()));
    connect(scriptEdit, SIGNAL(textChanged()), this, SLOT(scriptEdited()));
    connect(randomize, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(searchSubFolders, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(includeExtInf, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(relativePath, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(makeUnique, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(copyFilesToButton, SIGNAL(clicked()), this, SLOT(getCopyDir()));
    connect(copyFilesText, SIGNAL(editingFinished()), this, SLOT(updatePlayList()));
    connect(copyFilesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));


    connect(actionEditStyle, SIGNAL(triggered()), this, SLOT(editStyleDialog()));
    connect(actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveCollection()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveCollectionAs()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newCollection()));
    connect(actionClearTags, SIGNAL(triggered()), this, SLOT(clearTags()));    
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(actionScriptOnly, SIGNAL(triggered()), this, SLOT(guiModeChanged()));
    connect(actionGUIMode, SIGNAL(triggered()), this, SLOT(guiModeChanged()));

    connect(clearScriptOutputButton,SIGNAL(clicked()),this,SLOT(clearScriptOutput()));

}

void PlaylistManager::scriptEdited(){

    qDebug()<<"PlaylistManager::scriptEdited";
    collection_.setScript(scriptEdit->text());

}

void PlaylistManager::clearScriptOutput(){
    console_->clear();
}

/*!
 \brief

*/
void PlaylistManager::showAbout() {
    QString text = qApp->applicationName() + " version " + qApp->applicationVersion();
    text += "\nAuthor: Ivar Eskerud Smith / ivar.eskerud@gmail.com";
    QMessageBox::information(this, "About", text);
}

/*!
 \brief

*/
void PlaylistManager::clearTags() {

    QStringList keys = tags_.keys();
    for(int i=0;i<keys.size();i++){
        delete tags_[keys[i]];
    }
    tags_.clear();
    statusBar()->showMessage("Tags cleared", 8000);
}

/*!
 \brief

*/
void PlaylistManager::getCopyDir() {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() == 0) {
        return;
    }

    if (selected.size() > 1) {
        int ret = QMessageBox::warning(this, "", "This will set the directory to copy files to for all selected playlists, continue?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::No) {
            return;
        }
    }

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    QString dir = copyFilesText->text();
    if (dir.isEmpty()) {
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    dialog.setDirectory(dir);
    if (dialog.exec()) {
        QStringList d = dialog.selectedFiles();
        copyFilesText->setText(d[0]);

        for (int i = 0; i < selected.size(); i++) {
            PlayList* p = static_cast<PlayList*>(selected[i]);

            p->setCopyFilesToDir(d[0]);
        }
    }
}

/*!
 \brief

*/
void PlaylistManager::updatePlayList() {


    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() == 0) {
        return;
    }

    if (selected.size() > 1) {
        folderTable->blockSignals(true);
        extensions->blockSignals(true);
        RuleScript->blockSignals(true);
        copyFilesText->blockSignals(true);
        int ret = QMessageBox::warning(this, "", "This will set the edited value for the "+QString::number(selected.size())+" selected playlists, continue?", QMessageBox::Yes, QMessageBox::No);
        folderTable->blockSignals(false);
        extensions->blockSignals(false);
        RuleScript->blockSignals(false);
        copyFilesText->blockSignals(false);
        if (ret == QMessageBox::No) {
            return;
        }        
    }   


    //setFocus();

    QObject* s = QObject::sender();
    qDebug()<<s->objectName();


    for (int i = 0; i < selected.size(); i++) {
        PlayList* p = static_cast<PlayList*>(selected[i]);        
        if (s == folderTable) {
            QStringList folders = folderTable->toPlainText().split("\n");
            QList<QDir> dirs;
            for(int k=0;k<folders.size();k++){
                if(folders[k].isEmpty()){
                    continue;
                }
                dirs << QDir(folders[k]);
            }
            p->setFolders(dirs);
        }
        else if (s == extensions) {
            p->setExtensions(extensions->text().split(";"));
        }
        else if (s == RuleScript) {
            p->setScript(RuleScript->toPlainText());
        }
        else if (s == randomize) {
            p->setRandomize(randomize->isChecked());
        }
        else if (s == searchSubFolders) {
            p->setIncludeSubFolders(searchSubFolders->isChecked());
        }
        else if (s == includeExtInf) {
            p->setIncludeExtInf(includeExtInf->isChecked());
        }
        else if (s == relativePath) {
            p->setRelativePath(relativePath->isChecked());
        }
        else if (s == makeUnique) {
            p->setMakeUnique(makeUnique->isChecked());
        }
        else if (s == copyFilesText) {
            p->setCopyFilesToDir(QDir(copyFilesText->text()));
        }
        else if (s == copyFilesCheckBox) {
            p->setCopyFiles(copyFilesCheckBox->isChecked());
        }
        else {
            qDebug() << "BUG, unknown sender to updatePlayList";
        }

    }
    showRulesAndFolders();

}


/*!
 \brief

*/
void PlaylistManager::newCollection() {

    saveCollectionCheck();

    QFileInfo f(collection_.name());
    QString name = newUniqePlayListCollectionName( f.absoluteDir() );
    collection_ = PlayListCollection(name);
    lastSavedCollection_ = collection_;

    initialize();

}


/*!
 \brief

*/
void PlaylistManager::writeGUISettings() {

    //guiSettings->clear();
    guiSettings->beginGroup("MainWindow");
    guiSettings->setValue("size", this->size());
    guiSettings->setValue("pos", this->pos());
    guiSettings->endGroup();
    guiSettings->setValue("collection", collection_.name());        
    guiSettings->sync();    

}

/*!
 \brief

*/
void PlaylistManager::readGUISettings() {

    guiSettings->beginGroup("MainWindow");
    this->resize(guiSettings->value("size", QSize(400, 400)).toSize());
    this->move(guiSettings->value("pos", QPoint(200, 200)).toPoint());
    guiSettings->endGroup();
    //setGUIStyle( guiSettings->value("style").toString() );

    qApp->setStyle(QStyleFactory::create(guiSettings->value("style").toString()));
    qApp->setStyleSheet(guiSettings->value("styleSheet").toString());

    QFileInfo collectionFile(guiSettings->value("collection", PlayListCollection::defaultCollectionName() ).toString());
    loadCollection(collectionFile);

}


/*!
 \brief

*/
void PlaylistManager::open() {

    saveCollectionCheck();


    QDir dir(collection_.name());
    if (!dir.exists()) {
        dir = QDir(QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open collection"), dir.absolutePath(), "*" + Global::ext);
    if (fileName.isEmpty()) {
        return;
    }

    QFileInfo file(fileName);

    loadCollection(file);


}

/*!
 \brief

*/
void PlaylistManager::initialize() {
    //initialize the collection_ (private member)

    //enableOptions( false );

    scriptEdit->setPlainText(collection_.script());
    playListTable->clear();
    QList<PlayList> playLists = collection_.playLists();
    for (int i = 0; i < playLists.size(); i++) {
        PlayList* p = new PlayList;
        *p = playLists[i];
        p->setFlags( p->flags() | Qt::ItemIsEditable );
        playListTable->addItem(p);
    }
    guiSettings->setValue("outPutPath", collection_.outPutPath().absolutePath());
    guiModeChanged();
    showRulesAndFolders();

    this->setWindowTitle(collection_.name());

}

/*!
 \brief

*/
void PlaylistManager::guiModeChanged() {
    //switch between using script or a set of rules

    QAction *a  = qobject_cast<QAction*>(QObject::sender());
    int type = guiSettings->value("scriptType",Global::SCRIPTANDPLAYLIST).toInt();
    actionScriptOnly->setChecked(false);
    actionGUIMode->setChecked(false);
    if(a==actionScriptOnly){
        type = Global::SCRIPTONLY;
    }else if(a==actionGUIMode){
        type=Global::SCRIPTANDPLAYLIST;
    }
    guiSettings->setValue("scriptType",type);
    if(type!=Global::SCRIPTANDPLAYLIST && type!=Global::SCRIPTONLY){
        type=Global::SCRIPTANDPLAYLIST;
    }

    actionMakeOnePlaylistForEveryArtist->setVisible(true);
    if (type==Global::SCRIPTANDPLAYLIST) {
        actionGUIMode->setChecked(true);
        ConsoleFrame->hide();
        RuleScript->show();
        OptionsFrame->show();
        PlayListGroupBox->show();
        ScriptOnlyFrame->hide();        
    }else if(type==Global::SCRIPTONLY){
        actionScriptOnly->setChecked(true);
        ConsoleFrame->show();
        RuleScript->hide();        
        OptionsFrame->hide();
        PlayListGroupBox->hide();
        ScriptOnlyFrame->show();
        actionMakeOnePlaylistForEveryArtist->setVisible(false);
    }
}


/*!
 \brief

*/
void PlaylistManager::addIndividualFiles() {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() == 0) {
        return;
    }

    if (selected.size() > 1) {
        int ret = QMessageBox::warning(this, "", "This will add/merge the files you select for all selected playlists\nSelecting 0 files will clear the files for all selected playlists. Continue?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::No) {
            return;
        }
    }

    QList<QFileInfo> files;
    if (selected.size() == 1) {
        files = currentPlayList()->individualFiles();
    }
    AddFilesDialog d(files, this);
    if (d.exec() != QDialog::Accepted) {
        return;
    }

    files = d.getFiles();

    for (int i = 0; i < selected.size(); i++) {
        PlayList* p = static_cast<PlayList*>(selected[i]);

        QList<QFileInfo> files2 = files;
        if (selected.size() > 1 && !files.isEmpty()) {
            //merge with existing if more than one PlayList chosen
            //and if more than one files selected. 0 selected = clear
            files2 += p->individualFiles();
            files2 = files2.toSet().toList(); //unique
        }
        p->setIndividualFiles(files2);

        //show bold text on "add individual files" if individual files exist
        QFont f = addFilesButton->font();
        if (p->individualFiles().size() > 0) {
            f.setBold(true);
        }
        else {
            f.setBold(false);
        }
        addFilesButton->setFont(f);

    }
}


/*!
 \brief

 \param state
*/
void PlaylistManager::enableOptions(bool state) {


    foldersGroupBox->setEnabled(state);
    rulesGroupBox->setEnabled(state);
    optionsGroupBox->setEnabled(state);
    addFilesButton->setEnabled(state);
    copyFilesFrame->setEnabled(state);

}

/*!
 \brief

 \return PlayList *
*/
PlayList* PlaylistManager::currentPlayList() {
    PlayList* p = static_cast<PlayList*>(playListTable->currentItem());
    return p;
}

/*!
 \brief

 \return QList<QDir>
*/
QList<QDir> PlaylistManager::selectFolders( QAbstractItemView::SelectionMode mode ) {

    QList<QDir> dirs;

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    //dialog.setOption(QFileDialog::ShowDirsOnly, true);
    QString lastFolder = guiSettings->value("lastFolder", QDesktopServices::storageLocation(QDesktopServices::MusicLocation)).toString();
    if (!lastFolder.isEmpty()) {
        dialog.setDirectory(lastFolder);
    }
    QListView* l = dialog.findChild<QListView*>("listView");
    if (l) {
        l->setSelectionMode(mode);
    }
    if (dialog.exec() != QDialog::Accepted) {
        return dirs;
    }
    QStringList d = dialog.selectedFiles();
    for (int i = 0; i < d.size(); i++) {
        if (i == 0) {
            QFileInfo f(d[i]);
            guiSettings->setValue("lastFolder", f.absolutePath());
        }
        dirs.append(QDir(d[i]));
    }
    return dirs;

}


/*!
 \brief

*/
void PlaylistManager::addFolder() {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() == 0) {
        return;
    }

    if (selected.size() > 1) {
        int ret = QMessageBox::warning(this, "", "This will add/merge the folders you select for all selected playlists, continue?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::No) {
            return;
        }
    }

    QList<QDir> dirs = selectFolders();
    if (dirs.size() == 0) {
        return;
    }

    for (int i = 0; i < selected.size(); i++) {
        PlayList* p = static_cast<PlayList*>(selected[i]);

        QList<QDir> folders = p->folders();
        folders += dirs;
        folders = folders.toSet().toList();
        p->setFolders(folders);
    }

    showRulesAndFolders();
}



/*!
 \brief

*/
void PlaylistManager::saveCollectionCheck() {

    updateCollection();
    //QFileInfo f(collection_.name());
    //qDebug()<<"savecollectioncheck "<<(!(lastSavedCollection_==collection_))<<(lastSavedCollection_!=PlayListCollection());
    if ( !(lastSavedCollection_==collection_) ) {
        int ret = QMessageBox::warning(this, "", "Save existing collection first?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            saveCollection();
        }
    }
}


/*!
 \brief

 \param event
*/
void PlaylistManager::closeEvent(QCloseEvent* event) {

    saveCollectionCheck();

    writeGUISettings();

    event->accept();
}

/*!
 \brief

 \param block
*/
void PlaylistManager::blockPlayListSignals(bool block) {

    QList<QWidget*> children = optionsFrame->findChildren<QWidget*>();
    for (int i = 0; i < children.size(); i++) {
        children[i]->blockSignals(block);
    }

}


/*!
 \brief

*/
void PlaylistManager::clearRulesAndFolders() {
    //clears shon playList data

    blockPlayListSignals(true);

    folderTable->clear();
    extensions->clear();
    copyFilesCheckBox->setChecked(false);
    copyFilesText->clear();
    randomize->setChecked(false);
    searchSubFolders->setChecked(false);
    includeExtInf->setChecked(false);
    relativePath->setChecked(false);
    makeUnique->setChecked(false);

    blockPlayListSignals(false);

}

/*!
 \brief

*/
void PlaylistManager::showRulesAndFolders() {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() == 0) {
        enableOptions(false);
        return;
    }

    enableOptions(true);

    PlayList* p = static_cast<PlayList*>(selected[0]);//currentPlayList();


    blockPlayListSignals(true);

    folderTable->clear();
    extensions->setText("");

    //folders
    QList<QDir> folders = p->folders();
    QString foldersText;
    for (int i = 0; i < folders.size(); i++) {
        foldersText.append(folders[i].absolutePath()+"\n");
    }
    folderTable->setText(foldersText);

    //extensions & settings
    extensions->setText(p->extensions().join(";"));
    randomize->setChecked(p->randomize());
    searchSubFolders->setChecked(p->includeSubFolders());
    includeExtInf->setChecked(p->includeExtInf());
    relativePath->setChecked(p->relativePath());
    makeUnique->setChecked(p->makeUnique());
    copyFilesCheckBox->setChecked(p->copyFiles());
    copyFilesText->setText(p->copyFilesToDir().absolutePath());

    //show bold text on "add individual files" if individual files exist
    QFont f = addFilesButton->font();
    if (p->individualFiles().size() > 0) {
        f.setBold(true);
    }
    else {
        f.setBold(false);
    }
    addFilesButton->setFont(f);

    //rules
    RuleScript->setText(p->script());    
    infoLabel->setText(p->name());

    blockPlayListSignals(false);
}


/*!
 \brief

 \param file
*/
void PlaylistManager::loadCollection(const QFileInfo& file) {

    PlayListCollection collection;

    if (!file.exists()) {
        if (file != QFileInfo(PlayListCollection::defaultCollectionName())) {
            QMessageBox::critical(this, "",
                                  file.absoluteFilePath() + " does not exist",
                                  QMessageBox::Ok, QMessageBox::Ok);
        }
    }
    else {

        PMSettings playListCollection(file.absoluteFilePath(), PMSettings::IniFormat, this);
        playListCollection.setCheckForEmptyValue(false);
        QVariant tmp = playListCollection.value("collection");
        if (!tmp.canConvert<PlayListCollection>()) {
            QMessageBox::critical(this, "",
                                  file.absoluteFilePath() + " is not a valid " + qApp->applicationName() + " file",
                                  QMessageBox::Ok, QMessageBox::Ok);
        }
        else {
            collection = tmp.value<PlayListCollection>();
            qDebug() << "Successfully loaded collection " << file.fileName();
        }
    }

    collection_ = collection;
    lastSavedCollection_ = collection_;
    guiSettings->setValue("collection",collection_.name());
    guiSettings->sync();
    initialize();


}

/*!
 \brief

*/
void PlaylistManager::saveCollectionAs() {

    /*
    QDir dir(collection_.name());
    if (!dir.exists()) {
        dir = QDir(QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    }
    */
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), collection_.name(), "*" + Global::ext);

    if (fileName.isNull() || fileName.isEmpty()) {
        return;
    }

    collection_.setName(fileName);
    saveCollection(false);

}

/*!
 \brief

 \param row
 \return PlayList *
*/
PlayList* PlaylistManager::playListItem(int row) {
    if (row >= playListTable->count() || row < 0) {
        return 0;
    }
    return static_cast<PlayList*>(playListTable->item(row));
}

/*!
 \brief

*/
void PlaylistManager::updateCollection() {
    //must be called before saving! The playlists in the PlayListCollection is
    //not updated as the playlists in the playListTable change.

    QList<PlayList> playLists;
    for (int i = 0; i < playListTable->count(); i++) {
        PlayList* tmp = playListItem(i);
        PlayList p = *tmp;
        playLists.append(p);

    }
    collection_.setScript(scriptEdit->toPlainText());
    collection_.setPlayLists(playLists);

}


/*!
 \brief

 \param checkExistence
*/
void PlaylistManager::saveCollection(bool checkExistence) {

    QFileInfo file(collection_.name());

    if (checkExistence) {
        if (!file.exists()) {
            saveCollectionAs();
            return;
        }
    }

    updateCollection();

    PMSettings playListCollection(collection_.name(), PMSettings::IniFormat, this);
    playListCollection.clear();
    playListCollection.setValue("collection", collection_);
    playListCollection.sync();
    lastSavedCollection_ = collection_;

    statusBar()->showMessage(file.baseName() + " saved", 8000);
    qDebug()<<"Saved "<<file.baseName();
}


/*!
 \brief

*/
void PlaylistManager::generateSelectedPlayLists() {

    //int ind = playListTable->currentRow();
    QModelIndexList indexes = playListTable->selectionModel()->selectedIndexes();
    if (indexes.size() == 0) {
        QMessageBox::information(this, "",
                                 "Please select a playlist first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QList<PlayList*> playLists;
    for (int i = 0; i < indexes.size(); i++) {
        playLists.append( playListItem(indexes[i].row()) );
    }

    generatePlayLists(playLists);

}

/*!
 \brief

*/
void PlaylistManager::generateAllPlayLists() {

    QList<PlayList*> playLists;
    for (int i = 0; i < playListTable->count(); i++) {
        playLists.append( playListItem(i) );
    }

    generatePlayLists(playLists);

}


/*!
 \brief

 \param inds
*/
void PlaylistManager::generatePlayLists(const QList<PlayList*> &playLists) {

    if (playLists.size() == 0) {
        return;
    }

    guiSettings->sync();

    QDir d(collection_.outPutPath());// guiSettings->value("outPutPath").toString();
    if (!d.exists()) {
        /*QMessageBox::critical(this, "",
                              "Output folder is not a valid path",
                              QMessageBox::Ok, QMessageBox::Ok);                              
        return;
        */
        d.mkpath(d.absolutePath());
    }

    QString log;
    QStringList names;
    QTime timer; timer.start();

    bool overWrite = guiSettings->value("overWriteFiles").toBool();
    bool keepFolderStructure = guiSettings->value("keepFolderStructure").toBool();

    for (int i = 0; i < playLists.size(); i++) {

        PlayList* p = playLists[i];
        names.append(p->name());
        log.append("\n==================================================\nInfo for generation of playlist '" + p->name() + "'\n");

        QList<M3uEntry> songs;
        bool wasCanceled = p->generate(&songs, &log, &tags_);
        if (wasCanceled) {
            //log.append("\n\nAborted by user");
            break;
        }

        if (p->copyFiles()) {
            QStringList list;
            for(int i=0;i<songs.size();i++){
                list.append(songs[i].originalFile().absoluteFilePath());
            }
            Global::copyFoundFiles(list,p->copyFilesToDir().absolutePath(), keepFolderStructure, overWrite, &log);
        }
        //log.append("\n----------------------------------------------------------\n");
    }

    //int milliSecs = timer.elapsed();
    if (names.size() != names.toSet().toList().size()) {
        //if some names are equal
        log.append("Warning, some playlists have identical names!\n");
    }
    if (playLists.size() > 1) {        
        int time = timer.elapsed();
        log.append("\n\n Total time used: " + Global::timeString(time));
    }


    if (guiSettings->value("showLog").toBool()) {
        TextViewer t(this, log);
        t.exec();
    }
}

/*!
 \brief
 \param dir
 \return QString
*/
QString PlaylistManager::newUniqePlayListCollectionName( const QDir &dir ){

    QString name = "New Collection";
    int k = 0;
    while (1) {
        if (k > 0) {
            name = "New Collection" + QString::number(k);
        }
        bool ok = true;
        if( QFile::exists( dir.absoluteFilePath(name+Global::ext) ) ){
            ok=false;
        }
        if (ok) {
            break;
        }
        k++;
    }
    return name;
}

/*!
 \brief

 \return QString
*/
QString PlaylistManager::newUniqePlayListName() {
    //generate a new and unique playlist name

    QString name = "New Playlist";
    int k = 0;
    while (1) {
        if (k > 0) {
            name = "New Playlist" + QString::number(k);
        }
        bool ok = true;
        for (int i = 0; i < playListTable->count(); i++) {
            if (static_cast<PlayList*>(playListTable->item(i))->name() == name) {
                ok = false;
                break;
            }
        }
        if (ok) {
            break;
        }
        k++;
    }

    return name;
}


/*!
 \brief

*/
PlayList* PlaylistManager::addPlayList( QString name ) {
    //add a new playlist to the table

    if(name.isEmpty()){
        name = newUniqePlayListName();
    }
    PlayList* p = new PlayList(name);
    p->setFlags(p->flags() | Qt::ItemIsEditable);
    playListTable->addItem(p);
    playListTable->setCurrentItem(p);    

    return p;
}

/*!
 \brief

*/
void PlaylistManager::removePlayList() {

    QModelIndexList indexes = playListTable->selectionModel()->selectedIndexes();

    if (indexes.size() == 0) {
        return;
    }

    enableOptions(false);
    clearRulesAndFolders();
    playListTable->blockSignals(true);
    for (int i = indexes.size() - 1; i >= 0; i--) {
        delete playListTable->takeItem(indexes[i].row());
    }
    playListTable->blockSignals(false);
    if (playListTable->count() > 0) {
        playListTable->setCurrentRow(0);
    }
    showRulesAndFolders();


}


/*!
 \brief
*/
void PlaylistManager::showSettings() {

    guiSettings->sync();

    SettingsDialog s(this);
    if (s.exec() == QDialog::Accepted) {

        collection_.setOutPutPath( QDir(guiSettings->value("outPutPath").toString()) );
        if (!guiSettings->value("keepTags").toBool()) {
            tags_.clear();
        }
        if(guiSettings->value("debugScript").toBool() && this->isVisible()){
            scriptDebugger_.attachTo(&engine_);
        }else{
            scriptDebugger_.detach();
        }
    }


}

QString PlaylistManager::scriptFunctionsHelpText( QStringList *completions  ){

    QStringList c;
    QString tip = "Available functions:\n";
    tip += "QStringList getDirContent( const QString &path, const QStringList &extensions, bool includeSubFolders=true, bool hiddenFiles=true )\n";c<<"getDirContent";
    tip += "QStringList randomize(const QStringList &list)\n";c<<"randomize";
    tip += "QString fileRelativeTo(const QString &dir,const QString &file)\n";c<<"fileRelativeTo";
    tip += "QStringList fileRelativeTo(const QString &dir,const QStringList &files)\n";
    tip += "QStringList unique(const QStringList &list)\n";c<<"unique";
    tip += "bool contains(const QStringList &list, const QString &val, bool caseSensitive=true)\n";
    tip += "bool contains(const QString &str, const QString &val, bool caseSensitive=true)\n";c<<"contains";
    tip += "(Output is array)[int res, QString log] = copyFiles(const QStringList &files,\n";c<<"copyFiles";
    tip +="    const QString &copyFilesToDir, bool keepFolderStructure=false, bool overWrite=true)\n";
    tip += "bool writeFile( const QStringList &lines, const QString &file, bool append=false )\n";c<<"writeFile";

    if(completions){
        completions->append(c);
    }

    return tip;
}

/*!
 \brief Initialize script engine
*/
void PlaylistManager::initializeScriptEngine(){

    static bool initialized = false;
    if(initialized){
        return;
    }
    initialized=true;

    //qScriptRegisterSequenceMetaType<FrameList>(&engine_);
    //qScriptRegisterSequenceMetaType<FrameListList>(&engine_);
    qScriptRegisterSequenceMetaType< QList<QFileInfo> >(&engine_);
    //qScriptRegisterSequenceMetaType< QHash<QString,QStringList> >(&engine_);
    qScriptRegisterMetaType< QHash<QString,QStringList> >(&engine_, ScriptWrappers::toStringStringListHash,ScriptWrappers::fromStringStringListHash);
    qScriptRegisterMetaType<QFileInfo>(&engine_, ScriptWrappers::toQFileInfo,ScriptWrappers::fromQFileInfo);

    engine_.globalObject().setProperty("Tag",engine_.newFunction(ScriptWrappers::constructTag) );


    QStringList c;

    QString tip = scriptFunctionsHelpText(&c);
    tip +="\nTag functions:";
    tip += "\nvar tag = new Tag(const QString &fileName)";c<<"Tag";
    tip += "bool tagOk() const;\n";
    tip += "bool audioPropertiesOk() const;\n";
    tip += "QString fileName() const;\n";
    tip += "QString artist() const;\n";
    tip += "QString title() const;\n";
    tip += "QString album() const;\n";
    tip += "QString comment() const;\n";
    tip += "QString genre() const;\n";
    tip += "uint year() const;\n";
    tip += "uint track() const;\n";
    tip += "uint length() const;\n";
    tip += "uint bitRate() const;\n";
    tip += "uint sampleRate() const;\n";
    tip += "uint channels() const;\n";
    tip += "QHash<QString,QStringList> xiphFrames() const;\n";c<<"xiphFrames";
    tip += "QHash<QString,QStringList> ID3v2Frames() const;\n";c<<"ID3v2Frames";
    tip += "QHash<QString,QStringList> APEItems() const;\n";c<<"APEItems";
    tip += "QHash<QString,QStringList> MP4Items() const;\n";c<<"MP4Items";
    tip += "QHash<QString,QStringList> ASFAttributes() const;";c<<"ASFAttributes";

    scriptEdit->setToolTip(tip);
    scriptEdit->addCompletionWords(c);
}

bool PlaylistManager::runScript(const QString &script,bool guiMode) {

    console_->append("--------"+QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss")+"--------");
    qout = new QDebugStream(std::cout, console_);
    engine_.evaluate(script);
    if(engine_.hasUncaughtException()){
        QString err = "Uncaught exception at line "
                          + QString::number(engine_.uncaughtExceptionLineNumber()) + ": "
                          + qPrintable(engine_.uncaughtException().toString())
                          + "\nBacktrace: "
                          + qPrintable(engine_.uncaughtExceptionBacktrace().join(", "));
        qDebug()<<err;
        if(guiMode){
            QMessageBox::critical(0,"Error",err);
        }
        return false;
    }
    delete qout;
    console_->append("\n\n");
    qDebug()<<"Successfully evaluated script";
    return true;
}






