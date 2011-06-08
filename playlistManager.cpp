
#include "PlaylistManager.h"


/*!
 \brief

 \param parent
*/
PlaylistManager::PlaylistManager(QWidget* parent) : QMainWindow(parent) {
    setupUi(this); // this sets up GUI

    createActions();

    qRegisterMetaType<PlayList>("PlayList");
    qRegisterMetaTypeStreamOperators<PlayList>("PlayList");
    qRegisterMetaType<Rule>("Rule");
    qRegisterMetaTypeStreamOperators<Rule>("Rule");
    qRegisterMetaType<PlayListCollection>("PlayListCollection");
    qRegisterMetaTypeStreamOperators<PlayListCollection>("PlayListCollection");

    //playListCollection=0;
    guiSettings = Global::guiSettings();
    initGuiSettings();

    readGUISettings();

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
    qDebug() << "s.currentStyle() " << s.currentStyle() << qApp->style()->metaObject()->className();
    guiSettings->setValue("styleSheet", s.styleSheetText());
    qDebug() << s.styleSheetText();
    guiSettings->sync();
}

/*!
 \brief

*/
void PlaylistManager::initGuiSettings() {
    if (guiSettings == 0) {
        guiSettings = Global::guiSettings();
    }

    if (!guiSettings->value("style").canConvert(QVariant::String)) {
        guiSettings->setValue("style", qApp->style()->metaObject()->className());
    }
    if (!guiSettings->value("artistEmpty").canConvert(QVariant::Bool)) {
        guiSettings->setValue("artistEmpty", true);
    }
    if (!guiSettings->value("titleEmpty").canConvert(QVariant::Bool)) {
        guiSettings->setValue("titleEmpty", true);
    }
    if (!guiSettings->value("albumEmpty").canConvert(QVariant::Bool)) {
        guiSettings->setValue("albumEmpty", false);
    }
    if (!guiSettings->value("commentEmpty").canConvert(QVariant::Bool)) {
        guiSettings->setValue("commentEmpty", false);
    }
    if (!guiSettings->value("genreEmpty").canConvert(QVariant::Bool)) {
        guiSettings->setValue("genreEmpty", false);
    }
    if (!guiSettings->value("trackEmpty").canConvert(QVariant::Bool)) {
        guiSettings->setValue("trackEmpty", false);
    }
    if (!guiSettings->value("outPutPath").canConvert(QVariant::String)) {
        guiSettings->setValue("outPutPath", QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    }
    if (!guiSettings->value("defaultExtensions").canConvert(QVariant::List)) {
        QStringList defExts;
        defExts << "*.mp3" << "*.wma" << "*.wav" << "*.ogg" << "*.aac" << "*.ac3";
        guiSettings->setValue("defaultExtensions", defExts);
    }
    if (!guiSettings->value("format").canConvert(QVariant::String)) {
        guiSettings->setValue("format", "%artist% - %title%");
    }
    if (!guiSettings->value("useScript").canConvert(QVariant::Bool)) {
        guiSettings->setValue("useScript", false);
    }
    if (!guiSettings->value("showLog").canConvert(QVariant::Bool)) {
        guiSettings->setValue("showLog", true);
    }
    if (!guiSettings->value("useCopyFilesToPath").canConvert(QVariant::Bool)) {
        guiSettings->setValue("useCopyFilesToPath", false);
    }
    if (!guiSettings->value("keepFolderStructure").canConvert(QVariant::Bool)) {
        guiSettings->setValue("keepFolderStructure", true);
    }
    if (!guiSettings->value("keepTags").canConvert(QVariant::Bool)) {
        guiSettings->setValue("keepTags", true);
    }
    if (!guiSettings->value("copyFilesToDir").canConvert(QVariant::String)) {
        guiSettings->setValue("copyFilesToDir", "");
    }

}


/*!
 \brief

*/
void PlaylistManager::createActions() {

    //connect( actionRunScript, SIGNAL( triggered() ), this, SLOT( runscript() ) );


    // signals/slots mechanism in action
    connect(generateButton, SIGNAL(clicked()), this, SLOT(generateSelectedPlayLists()));
    connect(generateAllButton, SIGNAL(clicked()), this, SLOT(generateAllPlayLists()));
    connect(addPlayListButton, SIGNAL(clicked()), this, SLOT(addPlayList()));
    connect(removePlayListButton, SIGNAL(clicked()), this, SLOT(removePlayList()));
    connect(newRuleButton, SIGNAL(clicked()), this, SLOT(newRule()));
    connect(editRuleButton, SIGNAL(clicked()), this, SLOT(editRule()));
    connect(removeRuleButton, SIGNAL(clicked()), this, SLOT(removeRule()));
    connect(addFolderButton, SIGNAL(clicked()), this, SLOT(addFolder()));
    connect(removeFolderButton, SIGNAL(clicked()), this, SLOT(removeFolder()));
    connect(addFilesButton, SIGNAL(clicked()), this, SLOT(addIndividualFiles()));
    //connect( folderTable, SIGNAL( itemChanged ( QListWidgetItem * ) ), this, SLOT( renameFolder(QListWidgetItem *) ) );
    connect(folderTable, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(changeFolder(QListWidgetItem*)));

    //connect( playListTable, SIGNAL( itemChanged ( QListWidgetItem *) ), this, SLOT( renamePlayList(QListWidgetItem *) ) );
    //connect( playListTable, SIGNAL( currentRowChanged ( int ) ), this, SLOT( showRulesAndFolders(int) ) );
    connect(playListTable, SIGNAL(itemSelectionChanged()), this, SLOT(showRulesAndFolders()));
    connect(rulesTable, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editRule()));

    //playlist settings
    connect(extensions, SIGNAL(textEdited(const QString&)), this, SLOT(updatePlayList()));
    connect(RuleScript, SIGNAL(textChanged()), this, SLOT(updatePlayList()));
    connect(randomize, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(allRulesTrue, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(searchSubFolders, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(includeExtInf, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(relativePath, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(makeUnique, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));
    connect(copyFilesToButton, SIGNAL(clicked()), this, SLOT(getCopyDir()));
    connect(copyFilesText, SIGNAL(textEdited(const QString&)), this, SLOT(updatePlayList()));
    connect(copyFilesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updatePlayList()));


    connect(actionEditStyle, SIGNAL(triggered()), this, SLOT(editStyleDialog()));
    connect(actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveCollection()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveCollectionAs()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newCollection()));
    connect(actionClearTags, SIGNAL(triggered()), this, SLOT(clearTags()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));


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
        int ret = QMessageBox::warning(this, "", "This will set the edited value for all selected playlists, continue?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::No) {
            return;
        }
    }

    QObject* s = QObject::sender();
    for (int i = 0; i < selected.size(); i++) {
        PlayList* p = static_cast<PlayList*>(selected[i]);

        if (s == extensions) {
            p->setExtensions(extensions->text().split(";"));
        }
        else if (s == RuleScript) {
            p->setScript(RuleScript->toPlainText());
        }
        else if (s == randomize) {
            p->setRandomize(randomize->isChecked());
        }
        else if (s == allRulesTrue) {
            p->setAllRulesTrue(allRulesTrue->isChecked());
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

    updateCollection();

    if (!(lastSavedCollection_ == collection_)) {
        int ret = QMessageBox::warning(this, "", "Save existing collection first?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            saveCollection();
        }
    }

    collection_ = PlayListCollection();
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
    //delete guiSettings;



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
    qDebug() << guiSettings->value("style").toString();
    qApp->setStyleSheet(guiSettings->value("styleSheet").toString());   //qDebug()<<guiSettings->value("styleSheet").toString();

    QFileInfo collectionFile(guiSettings->value("collection", QDesktopServices::storageLocation(QDesktopServices::MusicLocation) + "/New collection" + Global::ext).toString());
    loadCollection(collectionFile);
    initialize();
}


/*!
 \brief

*/
void PlaylistManager::open() {

    updateCollection();
    if (!(lastSavedCollection_ == collection_)) {
        int ret = QMessageBox::warning(this, "", "Save existing collection first?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            saveCollection();
        }
    }


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
    initialize();

}

/*!
 \brief

*/
void PlaylistManager::initialize() {
    //initialize the collection_ (private member)

    qDebug() << "Initializing";
    //enableOptions( false );
    updateUseScript();
    showRulesAndFolders();

    this->setWindowTitle(collection_.name());

}

/*!
 \brief

*/
void PlaylistManager::updateUseScript() {
    //switch between using script or a set of rules

    if (guiSettings->value("useScript").toBool()) {
        rulesFrame->hide();
        RuleScript->show();
        allRulesTrue->setEnabled(false);
    }
    else {
        rulesFrame->show();
        RuleScript->hide();
        allRulesTrue->setEnabled(true);
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
QList<QDir> PlaylistManager::selectFolders() {

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
        l->setSelectionMode(QAbstractItemView::ExtendedSelection);
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

 \param item
*/
void PlaylistManager::renameFolder(QListWidgetItem* item) {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() > 1) {
        QMessageBox::critical(this, "", "Not allowed when more than one playlist is selected...");
        showRulesAndFolders();
        return;
    }

    PlayList* p = currentPlayList();
    if (!p || !folderTable->currentItem()) {
        return;
    }
    int ind = folderTable->currentRow();
    QList<QDir> folders = p->folders();
    QString oldName = folders[ind].absolutePath();
    QString newName = item->text();
    QDir d(newName);
    if (!d.exists()) {
        QMessageBox::critical(this, "",
                              "Folder " + newName + " does not exist",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        item->setText(oldName);
        return;
    }
    if (!d.isAbsolute()) {
        QMessageBox::critical(0, "",
                              "Folder " + newName + " is not an absolute path",
                              QMessageBox::Ok, QMessageBox::Ok);
        item->setText(oldName);
        return;
    }
    folders[ind] = QDir(newName);
    p->setFolders(folders);
    qDebug() << "renamed from " << oldName << " to " << item->text();

}

/*!
 \brief

 \param item
*/
void PlaylistManager::changeFolder(QListWidgetItem* item) {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() > 1) {
        QMessageBox::critical(this, "", "Not allowed when more than one playlist is selected...");
        showRulesAndFolders();
        return;
    }

    PlayList* p = currentPlayList();
    if (!p) {
        return;
    }
    int ind = folderTable->currentRow();
    QList<QDir> folders = p->folders();
    QString oldName = folders[ind].absolutePath();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                  oldName, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty()) {
        return;
    }
    item->setText(dir);
    folders[ind] = QDir(dir);
    p->setFolders(folders);

}

/*!
 \brief

*/
void PlaylistManager::removeFolder() {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() > 1) {
        int ret = QMessageBox::critical(this, "", "More than one playlist selected:\nThe folders you choose will be removed from all selected playlists, continue?", QMessageBox::Yes, QMessageBox::No);
        if (ret != QMessageBox::Yes) {
            return;
        }
        QList<QDir> dirs = selectFolders();
        if (dirs.size() == 0) {
            return;
        }
        for (int i = 0; i < selected.size(); i++) {
            PlayList* p = static_cast<PlayList*>(selected[i]);
            QList<QDir> folders = p->folders();
            for (int j = 0; j < dirs.size(); j++) {
                folders.removeAll(dirs[j]);
            }
            p->setFolders(folders);
        }
        showRulesAndFolders();
        return;
    }

    PlayList* p = currentPlayList();
    int ind = folderTable->currentRow();
    if (ind == -1 || !p) {
        return;
    }
    QList<QDir> folders = p->folders();
    folders.removeAt(ind);
    delete folderTable->takeItem(ind);
    p->setFolders(folders);
}

/*!
 \brief

 \param event
*/
void PlaylistManager::closeEvent(QCloseEvent* event) {

    updateCollection();

    if (!(lastSavedCollection_ == collection_)) {
        int ret = QMessageBox::warning(this, "", "Save existing collection first?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            saveCollection();
        }
    }

    writeGUISettings();
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
    rulesTable->clear();
    randomize->setChecked(false);
    allRulesTrue->setChecked(false);
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
    rulesTable->clear();
    extensions->setText("");

    //folders
    QList<QDir> folders = p->folders();
    for (int i = 0; i < folders.size(); i++) {
        QString t = folders[i].absolutePath();
        QListWidgetItem* item = new QListWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(t);
        folderTable->addItem(item);
    }

    //extensions & settings
    extensions->setText(p->extensions().join(";"));
    randomize->setChecked(p->randomize());
    searchSubFolders->setChecked(p->includeSubFolders());
    allRulesTrue->setChecked(p->allRulesTrue());
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
    QVector<Rule> rules = p->rules();
    for (int i = 0; i < rules.size(); i++) {
        Rule::RuleType t = rules[i].type();
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::UserRole, t);
        if (!rules[i].shouldBeTrue()) {
            QFont f = item->font();
            f.setItalic(true);
            item->setFont(f);
        }
        item->setText(Rule::getRuleName(t) + ": " + rules[i].value());
        rulesTable->addItem(item);
    }

    infoLabel->setText(p->name());

    blockPlayListSignals(false);
}


/*!
 \brief

 \param file
*/
void PlaylistManager::loadCollection(const QFileInfo& file) {
    qDebug() << "Reading settings...";

    PlayListCollection collection;

    qDebug() << file.filePath() << PlayListCollection::defaultCollectionName();
    if (!file.exists()) {
        if (file != QFileInfo(PlayListCollection::defaultCollectionName())) {
            QMessageBox::critical(this, "",
                                  file.absoluteFilePath() + " does not exist",
                                  QMessageBox::Ok, QMessageBox::Ok);
        }
    }
    else {

        QSettings playListCollection(file.absoluteFilePath(), QSettings::IniFormat, this);
        QVariant tmp = playListCollection.value("collection");
        if (!tmp.canConvert<PlayListCollection>()) {
            QMessageBox::critical(this, "",
                                  file.absoluteFilePath() + " is not a valid " + qApp->applicationName() + " file",
                                  QMessageBox::Ok, QMessageBox::Ok);
        }
        else {
            collection = tmp.value<PlayListCollection>();
            qDebug() << "successfully loaded collection " << file.fileName();
            qDebug() << collection_.playLists().size();
        }
    }

    collection_ = collection;
    lastSavedCollection_ = collection_;
    playListTable->clear();
    QList<PlayList> playLists = collection_.playLists();
    for (int i = 0; i < playLists.size(); i++) {
        PlayList* p = new PlayList;
        *p = playLists[i];
        playListTable->addItem(p);
    }

}

/*!
 \brief

*/
void PlaylistManager::saveCollectionAs() {

    QDir dir(collection_.name());
    if (!dir.exists()) {
        dir = QDir(QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), dir.absolutePath(), "*" + Global::ext);

    if (fileName.isNull() || fileName.isEmpty()) {
        return;
    }

    qDebug() << "PlaylistManager::saveCollectionAs " << fileName;
    collection_.setName(fileName);
    qDebug() << "PlaylistManager::saveCollectionAs " << collection_.name();
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
        qDebug() << tmp;
        PlayList p = *tmp;
        playLists.append(p);

    }
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

    qDebug() << "PlaylistManager::saveCollection " << collection_.name();

    updateCollection();

    QSettings playListCollection(collection_.name(), QSettings::IniFormat, this);
    playListCollection.clear();
    playListCollection.setValue("collection", collection_);
    playListCollection.sync();
    lastSavedCollection_ = collection_;

    statusBar()->showMessage(file.baseName() + " saved", 8000);

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

    QList<int> inds;
    for (int i = 0; i < indexes.size(); i++) {
        inds.append(indexes[i].row());
    }

    generatePlayLists(inds);

}

/*!
 \brief

*/
void PlaylistManager::generateAllPlayLists() {

    QList<int> inds;
    for (int i = 0; i < playListTable->count(); i++) {
        inds.append(i);
    }

    generatePlayLists(inds);

}


/*!
 \brief

 \param inds
*/
void PlaylistManager::generatePlayLists(const QList<int> &inds) {

    if (inds.size() == 0) {
        return;
    }

    guiSettings->sync();

    QString defOut = guiSettings->value("outPutPath").toString();
    QDir d(defOut);
    if (defOut.isEmpty() || !d.exists()) {
        QMessageBox::critical(this, "",
                              "Output folder is not a valid path",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }


    QString log;
    time_t Start_t, End_t;
    int totaltime;
    QStringList names;
    Start_t = time(0);

    for (int i = 0; i < inds.size(); i++) {

        PlayList* p = static_cast<PlayList*>(playListTable->item(i));
        names.append(p->name());
        log.append("Info for generation of playlist '" + p->name() + "'\n");

        QList<M3uEntry> songs;
        bool wasCanceled = p->generate(&songs, &log, &tags_);
        if (wasCanceled) {
            log.append("\n\nAborted by user");
            break;
        }

        if (p->copyFiles()) {
            p->copyFoundFiles(songs, &log);
        }
        log.append("\n----------------------------------------------------------\n");
    }

    End_t = time(0);
    if (names.size() != names.toSet().toList().size()) {
        //if some names are equal
        log.append("Warning, some playlists have identical names!\n");
    }
    if (inds.size() > 1) {
        totaltime = difftime(End_t, Start_t);
        log.append("\n\n Total time used: " + QString::number(totaltime) + " seconds\n");
    }
    if (guiSettings->value("showLog").toBool()) {
        TextViewer t(this, &log);
        t.exec();
    }
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
void PlaylistManager::addPlayList() {
    //add a new playlist to the table

    QString name = newUniqePlayListName();
    PlayList* p = new PlayList(name);
    p->setFlags(p->flags() | Qt::ItemIsEditable);
    playListTable->addItem(p);
    playListTable->setCurrentItem(p);


}

/*!
 \brief

*/
void PlaylistManager::removePlayList() {

    QModelIndexList indexes = playListTable->selectionModel()->selectedIndexes();

    if (indexes.size() == 0) {
        return;
    }

    qDebug() << "indexes.size() " << indexes.size();

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
void PlaylistManager::newRule() {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() == 0) {
        return;
    }

    if (selected.size() > 1) {
        int ret = QMessageBox::warning(this, "", "This will add/merge the rule you create for all selected playlists, continue?", QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::No) {
            return;
        }
    }


    RuleDialog rd;
    if (rd.exec() != QDialog::Accepted) {
        return;
    }
    Rule r = rd.getRule();

    for (int i = 0; i < selected.size(); i++) {
        PlayList* p = static_cast<PlayList*>(selected[i]);

        QVector<Rule> rules = p->rules();
        if (!rules.contains(r)) {
            rules.append(r);
        }
        p->setRules(rules);
    }

    showRulesAndFolders();


}

/*!
 \brief

*/
void PlaylistManager::editRule() {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() > 1) {
        QMessageBox::critical(this, "", "Not allowed when more than one playlist is selected...");
        showRulesAndFolders();
        return;
    }

    PlayList* p = currentPlayList();
    int rind = rulesTable->currentRow();
    QListWidgetItem* ruleItem = rulesTable->currentItem();
    if (rind == -1 || !p) {
        QMessageBox::information(this, "",
                                 "Please select a Rule first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    QVector<Rule> rules = p->rules();
    Rule r = rules[rind];
    RuleDialog rd(r, this);

    if (rd.exec() != QDialog::Accepted) {
        return;
    }
    Rule newr = rd.getRule();
    rules[rind] = newr;
    p->setRules(rules);

    ruleItem->setText(Rule::getRuleName(newr.type()) + ": " + newr.value());
    /*
    delete rulesTable->takeItem(rind);
    rulesTable->insertItem( rind, Rule::getRuleName( newr.type() )+": "+newr.value() );
    */
    showRulesAndFolders();

}

/*!
 \brief

*/
void PlaylistManager::removeRule() {

    QList<QListWidgetItem*> selected = playListTable->selectedItems();
    if (selected.size() > 1) {
        int ret = QMessageBox::critical(this, "", "More than one playlist selected:\nThe rule type (regardless of value) you choose will be removed from all selected playlists, continue?", QMessageBox::Yes, QMessageBox::No);
        if (ret != QMessageBox::Yes) {
            return;
        }
        Rule r(Rule::FILENAME_CONTAINS, "tmp"); //keep, unless you get error about empty value in RuleDialog
        RuleDialog rd(r, 0);
        rd.hideValueFrame(); //dont show valueFrame, just choose rule type
        if (rd.exec() != QDialog::Accepted) {
            return;
        }
        r = rd.getRule();

        for (int i = 0; i < selected.size(); i++) {
            PlayList* p = static_cast<PlayList*>(selected[i]);
            QVector<Rule> rules = p->rules();
            QVector<Rule> newRules;
            for (int j = 0; j < rules.size(); j++) {
                qDebug() << Rule::getRuleName(rules[j].type()) << Rule::getRuleName(r.type());
                if (rules[j].type() != r.type()) {
                    newRules.append(rules[j]);
                }
            }
            p->setRules(newRules);
        }
        showRulesAndFolders();
        return;
    }

    PlayList* p = currentPlayList();
    int rind = rulesTable->currentRow();
    if (rind == -1 || !p) {
        QMessageBox::information(this, "",
                                 "Please select a Rule first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    delete rulesTable->takeItem(rind);
    QVector<Rule> rules = p->rules();
    rules.remove(rind);
    p->setRules(rules);

}



/*!
 \brief

*/
void PlaylistManager::showSettings() {

    guiSettings->sync();

    SettingsDialog s(this);
    if (s.exec() == QDialog::Accepted) {

        updateUseScript();

        if (!guiSettings->value("keepTags").toBool()) {
            tags_.clear();
        }
    }


}





