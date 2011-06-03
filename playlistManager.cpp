
#include "PlaylistManager.h"


PlaylistManager::PlaylistManager(QWidget *parent) : QMainWindow(parent){
    setupUi(this); // this sets up GUI			

    playListCollection=0;
    guiSettings = Global::guiSettings();
    initGuiSettings();

    qRegisterMetaType<PlayList>("PlayList");
    qRegisterMetaTypeStreamOperators<PlayList>("PlayList");
    qRegisterMetaType<Rule>("Rule");
    qRegisterMetaTypeStreamOperators<Rule>("Rule");


    createActions();


    readGUISettings();

    initialize( playListCollectionFile );
    

    /*
	QScriptEngine e;
	//e.globalObject().setProperty("QString(artist)","1");
	QScriptValue res = e.evaluate(" QString(""artist"")==""1"" ");	
	if( e.hasUncaughtException() ){
		QString err = e.uncaughtExceptionBacktrace().join("\n");
		qDebug()<<err;
	}else{
		bool res2 = res.toBool();
		qDebug()<<"res "<<res2;
	}	
	*/
}

void PlaylistManager::initGuiSettings(){
    if(guiSettings==0){
        guiSettings = Global::guiSettings();
    }

    guiSettings->setValue("style","Plastique");
    guiSettings->setValue("artistEmpty",true);
    guiSettings->setValue("titleEmpty",true);
    guiSettings->setValue("albumEmpty",false);
    guiSettings->setValue("commentEmpty",false);
    guiSettings->setValue("genreEmpty",false);
    guiSettings->setValue("trackEmpty",false);
    guiSettings->setValue("outputPath",QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    QStringList defExts; defExts<< "*.mp3"<<"*.wma"<<"*.wav"<<"*.ogg"<<"*.aac"<<"*.ac3";
    guiSettings->setValue("defaultExtensions",defExts);
    guiSettings->setValue("format","%artist% - %title%");
    guiSettings->setValue("useScript",false);
    guiSettings->setValue("showLog",true);
    guiSettings->setValue("useCopyFilesToPath",false);
    guiSettings->setValue("keepTags",true);
    guiSettings->setValue("copyFilesToDir","");

}

void PlaylistManager::runscript(){

    /*
	QScriptEngine *engine = new QScriptEngine(this);    

	QString libpaths = qApp->libraryPaths().join("\n");
	qDebug()<<libpaths;	
	QString exts = engine->availableExtensions().join("\n");
	qDebug()<<exts;
	qDebug()<<engine->importedExtensions().join("\n");	
	QScriptValue tmp = engine->importExtension("qt.core"); //"qt.gui"
	qDebug()<<tmp.toString();
	*/
    /*	struct QtMetaObject : private QObject{
	public:
	 static const QMetaObject *get()
		 { return &static_cast<QtMetaObject*>(0)->staticQtMetaObject; }
	};	
	QScriptValue Qt = engine.newQMetaObject(QtMetaObject::get());	
    Qt.setProperty("App", engine.newQObject(qApp));
    engine.globalObject().setProperty("Qt", Qt);
	*/
    /*
	bool ok;
	static QString script;
	script = QInputDialog::getText(this, tr("QInputDialog::getText()"),	
	tr("Script:"), QLineEdit::Normal,script,&ok);	
	//QString script = " Qt::QString(\"artist\")==\"1\" " ;
	qDebug()<<script;
	QScriptValue res = engine->evaluate(script);	
	if( engine->hasUncaughtException() ){
		QString err = engine->uncaughtException().toString();
		qDebug()<<err;
	}else{		
		qDebug()<<"res "<<res.toVariant();
	}	
	*/
}

void PlaylistManager::openStyleSheet(){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open style sheet file"),
                                                    guiSettings->value("lastStyleSheetFolder",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString(), tr("*.qss"));
    if( fileName.isEmpty() ){
        checkStyleAction( "Custom...", false );
        return;
    }

    QFileInfo f(fileName);
    guiSettings->setValue("lastStyleSheetFolder",f.absolutePath());
    guiSettings->sync();
    bool ok = loadStyleSheet( fileName );
    if(!ok){
        QMessageBox::critical(this, "",
                              "Could not open "+fileName,
                              QMessageBox::Ok, QMessageBox::Ok);
        checkStyleAction( "Custom...", false );
        return;
    }
    uncheckStyleActions();
    checkStyleAction( "Custom...", true );
    guiSettings->setValue("style",fileName);
    guiSettings->sync();
    //lastStyleSheetFolder = f.absolutePath();

}

bool PlaylistManager::loadStyleSheet( const QString &file ){
    //http://zhoushanfeng.spaces.live.com/blog/cns!E046ED05325D3173!336.entry

    QFile data(file);
    QString s;

    if(!data.open(QFile::ReadOnly)){
        return false;
    }

    s = QLatin1String(data.readAll());
    //QTextStream styleIn(&data);
    //s = styleIn.readAll();
    data.close();
    qApp->setStyleSheet(s);

    return true;
}

void PlaylistManager::createActions(){

    //connect( actionRunScript, SIGNAL( triggered() ), this, SLOT( runscript() ) );


    // signals/slots mechanism in action
    connect( generateButton, SIGNAL( clicked() ), this, SLOT( generateSelectedPlayLists() ) ); 
    connect( generateAllButton, SIGNAL( clicked() ), this, SLOT( generateAllPlayLists() ) ); 
    connect( addPlayListButton, SIGNAL( clicked() ), this, SLOT( addPlayList() ) ); 
    connect( removePlayListButton, SIGNAL( clicked() ), this, SLOT( removePlayList() ) ); 
    connect( newRuleButton, SIGNAL( clicked() ), this, SLOT( newRule() ) );
    connect( editRuleButton, SIGNAL( clicked() ), this, SLOT( editRule() ) );
    connect( removeRuleButton, SIGNAL( clicked() ), this, SLOT( removeRule() ) );
    connect( addFolderButton, SIGNAL( clicked() ), this, SLOT( addFolder() ) );
    connect( removeFolderButton, SIGNAL( clicked() ), this, SLOT( removeFolder() ) );
    connect( addFilesButton, SIGNAL( clicked() ), this, SLOT( addIndividualFiles() ) );
    //connect( folderTable, SIGNAL( itemChanged ( QListWidgetItem * ) ), this, SLOT( renameFolder(QListWidgetItem *) ) );
    connect( folderTable, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ), this, SLOT( changeFolder(QListWidgetItem *) ) );

    //connect( playListTable, SIGNAL( itemChanged ( QListWidgetItem *) ), this, SLOT( renamePlayList(QListWidgetItem *) ) );
    //connect( playListTable, SIGNAL( currentRowChanged ( int ) ), this, SLOT( showRulesAndFolders(int) ) );
    connect( playListTable, SIGNAL( itemSelectionChanged() ), this, SLOT( showRulesAndFolders() ) );
    connect( rulesTable, SIGNAL( itemDoubleClicked ( QListWidgetItem * ) ), this, SLOT( editRule() ) );

    //playlist settings
    connect( extensions, SIGNAL( textEdited( const QString & )   ), this, SLOT( updateExtensions(const QString &) ) );
    connect( RuleScript, SIGNAL( textChanged()   ), this, SLOT( updateScript() ) );
    connect( randomize, SIGNAL( stateChanged ( int ) ), this, SLOT( updateRandomize( int ) ) );
    connect( allRulesTrue, SIGNAL( stateChanged ( int ) ), this, SLOT( updateAllRulesTrue( int ) ) );
    connect( searchSubFolders, SIGNAL( stateChanged ( int ) ), this, SLOT( updateSearchSubfolders( int ) ) );
    connect( includeExtInf, SIGNAL( stateChanged ( int ) ), this, SLOT( updateIncludeExtInf( int ) ) );
    connect( relativePath, SIGNAL( stateChanged ( int ) ), this, SLOT( updateUseRelativePath( int ) ) );
    connect( makeUnique, SIGNAL( stateChanged ( int ) ), this, SLOT( updateMakeUnique( int ) ) );

    connect( actionSettings, SIGNAL( triggered() ), this, SLOT( showSettings() ) );
    connect( actionSave, SIGNAL( triggered() ), this, SLOT( saveCurrentCollection() ) );
    connect( actionSaveAs, SIGNAL( triggered() ), this, SLOT( saveSettingsAs() ) );
    connect( actionOpen, SIGNAL( triggered() ), this, SLOT( open() ) );
    connect( actionNew, SIGNAL( triggered() ), this, SLOT( newCollection() ) );
    connect( actionClearTags, SIGNAL( triggered() ), this, SLOT( clearTags() ) );
    connect( actionAbout, SIGNAL( triggered() ), this, SLOT( showAbout() ) );

    connect( copyFilesToButton, SIGNAL( clicked() ), this, SLOT( getCopyDir() ) );
    connect( copyFilesText, SIGNAL( textEdited( const QString & ) ), this, SLOT( updateCopyFiles( const QString & ) ) );
    connect( copyFilesCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( updateCopyTo( int ) ) );

    //styles

    QSignalMapper *styleMapper = new QSignalMapper(this);
    QStringList styles = QStyleFactory::keys();
    for(int i=0;i<styles.size();i++){
        QAction *a = new QAction(styles[i],menuStyle);
        a->setCheckable(true);
        connect(a, SIGNAL(triggered()), styleMapper, SLOT(map()));
        styleMapper->setMapping(a, styles[i]);
        menuStyle->addAction(a);
    }
    menuStyle->addSeparator();
    QAction *actionCustomStyleSheet = new QAction("Custom...",menuStyle);
    actionCustomStyleSheet->setCheckable(true);
    connect(actionCustomStyleSheet, SIGNAL(triggered()), this, SLOT(openStyleSheet()));
    menuStyle->addAction( actionCustomStyleSheet );
    connect(styleMapper, SIGNAL(mapped(const QString &)), this, SLOT(setGUIStyle(const QString &)));

}

void PlaylistManager::showAbout(){
    QString text = qApp->applicationName()+" version "+qApp->applicationVersion();
    text += "\nAuthor: Ivar Eskerud Smith / ivar.eskerud@gmail.com";
    QMessageBox::information(this,"About",text);
}


void PlaylistManager::clearTags(){

    tags_.clear();
    statusBar()->showMessage("Tags cleared", 8000);
}

void PlaylistManager::getCopyDir(){

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    QString dir = copyFilesText->text();
    if(dir.isEmpty()){
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    dialog.setDirectory( dir );
    if( dialog.exec() ){
        QStringList d = dialog.selectedFiles();
        copyFilesText->setText( d[0] );
        PlayList *p = currentPlayList();
        if(p){
            p->setCopyFilesToDir(d[0]);
        }
    }
}

void PlaylistManager::updateCopyFiles( const QString &text ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the extensions",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    qDebug()<<"copy files dir set to "<<text<<" for "<<p->name();
    p->setCopyFilesToDir(copyFilesText->text());
}

void PlaylistManager::updateCopyTo( int state ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the extensions",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    qDebug()<<"copy files set to "<<state<<" for "<<p->name();
    p->setCopyFiles(copyFilesCheckBox->isChecked());
}

void PlaylistManager::newCollection(){

    QString dir = playListCollectionFile.absoluteFilePath();
    if(dir.isEmpty()){
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Name of new playlist collection"),dir, "*"+Global::ext);

    if( fileName.isNull() || fileName.isEmpty() ){
        return;
    }

    qDebug()<<"New collection: "<<fileName;
    QFileInfo file(fileName);
    if(playListCollection!=0){
        delete playListCollection; playListCollection=0;
    }

    playListCollection = new QSettings(file.absoluteFilePath(),QSettings::IniFormat,this);
    playListCollection->clear();
    playListCollection->setValue( "playListGeneratorFile", true ); //identifier to be accepted in initialize/readSettings as a valid file
    playListCollection->sync();
    //readGUISettings(); //no need
    playListTable->blockSignals(true);
    initialize( file );
    saveCollection( file );
    playListTable->blockSignals(false);
    showRulesAndFolders();
}


void PlaylistManager::uncheckStyleActions(){

    QList<QAction *> actions = menuStyle->actions();
    for(int i=0;i<actions.size();i++){
        actions[i]->setChecked(false);
    }
}

void PlaylistManager::checkStyleAction( const QString &actionText, bool state ){

    QList<QAction *> actions = menuStyle->actions();
    for(int i=0;i<actions.size();i++){
        if( actions[i]->text()==actionText ){
            actions[i]->setChecked(state);
        }
    }
}

void PlaylistManager::setGUIStyle( const QString &s ){

    qDebug()<<"style: "<<s;
    QStringList styles = QStyleFactory::keys();
    if( styles.contains( s ) ){
        //build in style
        QList<QAction *> actions = menuStyle->actions();
        for(int i=0;i<actions.size();i++){
            qDebug()<<actions[i]->text();
            if( actions[i]->text()==s ){
                QApplication::setStyle( QStyleFactory::create ( s ) );
                uncheckStyleActions();
                guiSettings->setValue("style",s); guiSettings->sync();
                actions[i]->setChecked(true);
                return;
            }
        }
    }

    //if function has not returned yet, assume qss file
    qDebug()<<"Custom style!";
    bool ok = loadStyleSheet( s );
    if(!ok){
        QMessageBox::critical(this, "",
                              "Could not open style "+s,
                              QMessageBox::Ok, QMessageBox::Ok);
        //uncheck currently clicked action
        checkStyleAction( "Custom...", false );
    }else{
        uncheckStyleActions();
        checkStyleAction( "Custom...", true );
        guiSettings->setValue("style",s); guiSettings->sync();
    }

}

void PlaylistManager::writeGUISettings(){

    guiSettings->clear();
    guiSettings->beginGroup("MainWindow");
    guiSettings->setValue("size", this->size());
    guiSettings->setValue("pos", this->pos());
    guiSettings->endGroup();
    guiSettings->setValue("settingsFile", playListCollectionFile.absoluteFilePath() );

    guiSettings->sync();
    //delete guiSettings;



}

void PlaylistManager::readGUISettings(){

    guiSettings->beginGroup("MainWindow");
    this->resize(guiSettings->value("size", QSize(400, 400)).toSize());
    this->move(guiSettings->value("pos", QPoint(200, 200)).toPoint());
    guiSettings->endGroup();
    playListCollectionFile = QFileInfo(guiSettings->value("settingsFile",QDesktopServices::storageLocation(QDesktopServices::MusicLocation)+"/New collection"+Global::ext ).toString());

}


void PlaylistManager::open(){

    QString dir = playListCollectionFile.absoluteFilePath();
    if(dir.isEmpty()){
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),dir, "*"+Global::ext);
    if( fileName.isEmpty() ){
        return;
    }
    QFileInfo file = QFileInfo( fileName );
    playListTable->blockSignals(true);
    initialize( file );
    playListTable->blockSignals(false);
    showRulesAndFolders();
}

void PlaylistManager::initialize( const QFileInfo &file ){

    playListCollectionFile = file;


    QString newCollection = QDesktopServices::storageLocation(QDesktopServices::MusicLocation)+"/New collection"+Global::ext;
    newCollection=newCollection.replace("\\","/");

    if(playListCollectionFile.absoluteFilePath().isEmpty()){
        playListCollectionFile = QFileInfo( newCollection );
    }


    if(playListCollection!=0){
        delete playListCollection;
        playListCollection=0;
    }

    playListCollection = new QSettings( playListCollectionFile.fileName(), QSettings::IniFormat, this );

    qDebug()<<"Initializing";
    Global::fileReadResult ok = readCollection( playListCollectionFile );
    qDebug()<<file.absoluteFilePath()<<newCollection;
    if(ok==Global::NOTVALID && file.absoluteFilePath()!=newCollection ){
        QMessageBox::critical(this, "",
                              file.absoluteFilePath()+" is not a valid " + qApp->applicationName() + " file",
                              QMessageBox::Ok, QMessageBox::Ok);
    }else if(ok==Global::DOESNOTEXIST && file.absoluteFilePath()!=newCollection ){
        QMessageBox::critical(this, "",
                              file.absoluteFilePath()+" does not exist",
                              QMessageBox::Ok, QMessageBox::Ok);        
    }
    enableOptions( false );    
    setGUIStyle( guiSettings->value("style").toString() );
    updateUseScript();    
    this->setWindowTitle( "Current playlist collection: "+playListCollectionFile.absoluteFilePath() );
}

void PlaylistManager::updateUseScript(){
    if( guiSettings->value("useScript").toBool() ){
        rulesFrame->hide();
        RuleScript->show();
        allRulesTrue->setEnabled(false);
    }else{
        rulesFrame->show();
        RuleScript->hide();
        allRulesTrue->setEnabled(true);
    }
}

/*
void PlaylistManager::giveUniqueIds(){

    for(int i=0;i<playLists_.size();i++){
        playLists_[i].setUniqueId(i);
    }

}
*/

/*
void PlaylistManager::sortPlayLists(){

    //give unique ids
    giveUniqueIds();
    //get currently selected row
    int nowRow = playListTable->currentRow();
    qDebug()<<"nowRow "<<nowRow;

    int id=-1;
    if( nowRow>=0 && nowRow<playLists_.size() ){
        id = playLists_[nowRow].uniqueId();
    }

    //do sorting
    qSort( playLists_.begin(), playLists_.end(), Global::caseInsensitiveLessThan );
    playListTable->clear();
    for(int i=0;i<playLists_.size();i++){
        QListWidgetItem *item = new QListWidgetItem();
        item->setText( playLists_[i].name() );
        item->setFlags(item->flags () | Qt::ItemIsEditable);
        playListTable->addItem( item );
    }

    //find and select previously selected playlist
    if( nowRow>=0 && nowRow<playLists_.size() ){
        qDebug()<<"unique id of playlist is "<<id;
        int row=-1;
        for(int i=0;i<playLists_.size();i++){
            if(playLists_[i].uniqueId()==id){
                row = i;
                qDebug()<<"found playlist with id "<<id<<" at row "<<row<<": "<<playLists_[row].name();
                break;
            }
        }
        if(row>=0){
            playListTable->setCurrentRow(row);
        }
    }

    showRulesAndFolders();

}
*/

void PlaylistManager::addIndividualFiles(){

    PlayList *p = currentPlayList();
    if(!p){
        return;
    }

    AddFilesDialog d(p->individualFiles());
    if( d.exec()!=QDialog::Accepted ){
        return;
    }    

    p->setIndividualFiles(d.getFiles());

    //show bold text on "add individual files" if individual files exist
    QFont f = addFilesButton->font();
    if( p->individualFiles().size()>0 ){
        f.setBold(true);
    }else{
        f.setBold(false);
    }
    addFilesButton->setFont(f);

}


void PlaylistManager::enableOptions( bool state ){

    foldersGroupBox->setEnabled( state );
    rulesGroupBox->setEnabled( state );
    optionsGroupBox->setEnabled( state );
    addFilesButton->setEnabled( state );
    copyFilesFrame->setEnabled( state );
}

void PlaylistManager::updateRandomize( int state ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setRandomize(randomize->isChecked());

}

PlayList* PlaylistManager::currentPlayList(){
    PlayList *p = static_cast<PlayList*>(playListTable->currentItem());
    return p;
}


void PlaylistManager::updateAllRulesTrue( int state ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setAllRulesTrue(allRulesTrue->isChecked());

}

void PlaylistManager::updateSearchSubfolders( int state ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setIncludeSubFolders(searchSubFolders->isChecked());

}

void PlaylistManager::updateIncludeExtInf( int state ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setIncludeExtInf(includeExtInf->isChecked());

}

void PlaylistManager::updateUseRelativePath( int state ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setRelativePath(relativePath->isChecked());

}


void PlaylistManager::updateMakeUnique( int state ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setMakeUnique(makeUnique->isChecked());

}

void PlaylistManager::updateScript(){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the script",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setScript(RuleScript->toPlainText());

}

void PlaylistManager::updateExtensions( const QString & exts ){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the extensions",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setExtensions(exts.split(";"));
}

void PlaylistManager::addFolder(){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    QString dir;
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    //dialog.setOption(QFileDialog::ShowDirsOnly, true);
    QString lastFolder = guiSettings->value("lastFolder",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString();
    if(!lastFolder.isEmpty()){
        dialog.setDirectory( lastFolder );
    }
    QListView *l = dialog.findChild<QListView*>("listView");
    if (l) {
        l->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    if( dialog.exec()!=QDialog::Accepted ){
        return;
    }
    QStringList d = dialog.selectedFiles();

    QList<QDir> folders = p->folders();
    for(int i=0;i<d.size();i++){
        if(i==0){
            QFileInfo f(d[i]);
            guiSettings->setValue("lastFolder",f.absolutePath());
        }        
        QListWidgetItem *item = new QListWidgetItem();
        item->setText( d[i] );
        item->setFlags(item->flags () & ~Qt::ItemIsEditable);
        folderTable->addItem( item );
        folders.append(QDir(d[i]));
    }
    p->setFolders(folders);
}

void PlaylistManager::renameFolder(QListWidgetItem *item){

    PlayList *p = currentPlayList();
    if( !p || !folderTable->currentItem() ){
        return;
    }
    int ind = folderTable->currentRow();
    QList<QDir> folders = p->folders();
    QString oldName = folders[ind].absolutePath();
    QString newName = item->text();
    QDir d( newName );
    if( !d.exists() ){
        QMessageBox::critical(this, "",
                              "Folder "+newName+" does not exist",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        item->setText( oldName );
        return;
    }
    if( !d.isAbsolute() ){
        QMessageBox::critical(0, "",
                              "Folder "+newName+" is not an absolute path",
                              QMessageBox::Ok, QMessageBox::Ok);
        item->setText( oldName );
        return;
    }
    folders[ind] = QDir(newName);
    p->setFolders(folders);
    qDebug()<<"renamed from "<<oldName<<" to "<<item->text();

}

void PlaylistManager::changeFolder(QListWidgetItem *item){

    PlayList *p = currentPlayList();
    if(!p){
        return;
    }
    int ind = folderTable->currentRow();
    QList<QDir> folders = p->folders();
    QString oldName = folders[ind].absolutePath();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    oldName,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if( dir.isEmpty() ){
        return;
    }
    item->setText( dir );
    folders[ind] = QDir(dir);
    p->setFolders(folders);

}

void PlaylistManager::removeFolder(){

    PlayList *p = currentPlayList();
    int ind = folderTable->currentRow();    
    if(ind==-1 || !p){
        return;
    }
    QList<QDir> folders = p->folders();
    folders.removeAt( ind );
    delete folderTable->takeItem( ind );
    p->setFolders(folders);
}

void PlaylistManager::closeEvent( QCloseEvent *event ){

    if( !playListCollectionFile.exists() ){
        saveSettingsAs();
    }else{
        saveCollection( playListCollectionFile );
    }
    writeGUISettings();
}

void PlaylistManager::clearRulesAndFolders(){

    QList<QWidget*> children = optionsFrame->findChildren<QWidget*>();
    for(int i=0;i<children.size();i++){
        children[i]->blockSignals(true);
    }

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

    for(int i=0;i<children.size();i++){
        children[i]->blockSignals(false);
    }

}

void PlaylistManager::showRulesAndFolders(){

    folderTable->clear();
    rulesTable->clear();
    extensions->setText("");
    qDebug()<<"a";
    //don`t show rules & folders if more than one playlist is selected
    QList<QListWidgetItem*>  tmp = playListTable->selectedItems();
    if(tmp.size()>1){ //for some reason 0 is given when 1 item is selected
        enableOptions( false );
        return;
    }
    qDebug()<<"b";
    PlayList *p = currentPlayList();
    if( !p ){
        enableOptions( false );
        return;
    }

    enableOptions( true );

    //folders    
    QList<QDir> folders = p->folders();
    for(int i=0;i<folders.size();i++){
        QString t = folders[i].absolutePath();
        QListWidgetItem *item = new QListWidgetItem();
        item->setFlags(item->flags () & ~Qt::ItemIsEditable);
        item->setText( t );
        folderTable->addItem( item );
    }

    //extensions & settings
    extensions->setText( p->extensions().join(";") );
    randomize->setChecked( p->randomize() );
    searchSubFolders->setChecked( p->includeSubFolders() );
    allRulesTrue->setChecked( p->allRulesTrue() );
    includeExtInf->setChecked( p->includeExtInf() );
    relativePath->setChecked( p->relativePath() );
    makeUnique->setChecked( p->makeUnique() );
    copyFilesCheckBox->setChecked( p->copyFiles() );
    copyFilesText->setText( p->copyFilesToDir().absolutePath() );
    qDebug()<<"f";
    //show bold text on "add individual files" if individual files exist
    QFont f = addFilesButton->font();
    if( p->individualFiles().size()>0 ){
        f.setBold(true);
    }else{
        f.setBold(false);
    }
    addFilesButton->setFont(f);

    //rules
    RuleScript->setText( p->script() );
    QVector<Rule> rules = p->rules();
    for(int i=0;i<rules.size();i++){
        Rule::RuleType t = rules[i].type();
        QListWidgetItem *item = new QListWidgetItem();
        if(!rules[i].shouldBeTrue()){
            QFont f = item->font(); f.setItalic(true);
            item->setFont(f);
        }
        item->setText( Rule::getRuleName(t)+": "+ rules[i].value() );
        rulesTable->addItem( item );
    }

}


Global::fileReadResult PlaylistManager::readCollection( const QFileInfo &file ){
    qDebug()<<"Reading settings...";

    if( !file.exists() ){
        return Global::DOESNOTEXIST;
    }

    playListCollection = new QSettings( file.absoluteFilePath(), QSettings::IniFormat, this );
    /*bool valid = playListSettings->value( "playListGeneratorFile", false ).toBool(); //identifier
    if(!valid){
        return NOTVALID;
    }
    */

    QStringList playListNames = playListCollection->allKeys();
    for(int i=0;i<playListNames.size();i++){
        PlayList p = playListCollection->value(playListNames[i]).value< PlayList >();
        PlayList *p2 = new PlayList(p);
        p2->setFlags(p2->flags () | Qt::ItemIsEditable);
        playListTable->addItem( p2 );
    }


    return Global::EXISTS;

}

void PlaylistManager::saveSettingsAs(){

    QString dir = playListCollectionFile.absoluteFilePath();
    if(dir.isEmpty()){
        dir = qApp->applicationDirPath();//QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), dir, "*"+Global::ext);

    if( fileName.isNull() || fileName.isEmpty() ){
        return;
    }
    qDebug()<<"New filename: "<<fileName;
    QFileInfo file = QFileInfo( fileName );
    saveCollection( file );
    initialize( file );

}

void PlaylistManager::saveCurrentCollection(){
    saveCollection( playListCollectionFile );
}

void PlaylistManager::saveCollection( const QFileInfo &file ){

    if( !file.absoluteDir().exists() || file.fileName().isEmpty() ){
        saveSettingsAs();
        return;
    }

    playListCollection->clear();
    for(int i=0;i<playListTable->count();i++){
        PlayList *p = static_cast<PlayList*>(playListTable->item(i));
        playListCollection->setValue(p->name(),*p);
    }


    playListCollection->sync();

    statusBar()->showMessage("'"+file.baseName()+"' saved!", 8000);

}


void PlaylistManager::generateSelectedPlayLists(){

    //int ind = playListTable->currentRow();
    QModelIndexList indexes = playListTable->selectionModel()->selectedIndexes();
    if(indexes.size()==0){
        QMessageBox::information(this, "",
                                 "Please select a playlist first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QList<int> inds;
    for(int i=0;i<indexes.size();i++){
        inds.append(indexes[i].row());
    }

    generatePlayLists(inds);

}

void PlaylistManager::generateAllPlayLists(){

    QList<int> inds;
    for(int i=0;i<playListTable->count();i++){
        inds.append(i);
    }

    generatePlayLists(inds);

}


void PlaylistManager::generatePlayLists( const QList<int> &inds ){

    if( inds.size()==0 ){
        return;
    }

    guiSettings->sync();

    QString defOut = guiSettings->value("defaultOutputFolder").toString();
    QDir d(defOut);
    if( defOut.isEmpty() || !d.exists() ){
        QMessageBox::critical(this, "",
                              "Output folder is not a valid path",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //writeGUISettings();

    QString log;
    time_t Start_t, End_t;
    int totaltime; QStringList names;
    Start_t = time(0);

    for(int i=0;i<inds.size();i++){

        PlayList *p = static_cast<PlayList*>(playListTable->item(i));
        names.append( p->name() );
        log.append("Info for generation of playlist '"+p->name()+"'\n");

        bool ok=true;
        if( p->copyFiles() ){
            QDir c(p->copyFilesToDir());
            if( !c.exists() ){
                QDir newDir;
                ok = newDir.mkpath( p->copyFilesToDir().absolutePath() );
                if(ok){
                    log.append("Created directory "+p->copyFilesToDir().absolutePath()+"\n");
                }else{
                    log.append("Could not create directory "+p->copyFilesToDir().absolutePath()+", no copying performed\n");
                }
            }
        }

        QList<M3uEntry> songs;
        bool wasCanceled = p->generate( &songs, &log, &tags_ );
        if(wasCanceled){
            log.append("\n\nAborted by user");
            break;
        }
        if( p->copyFiles() && ok ){
            p->copyFoundFiles( songs, &log );
        }
        log.append("\n----------------------------------------------------------\n");
    }

    End_t = time(0);
    if(names.size()!=names.toSet().toList().size()){
        //if some names are equal
        log.append("Warning, some playlists have identical names!\n");
    }
    if(inds.size()>1){
        totaltime = difftime(End_t, Start_t);
        log.append("\n\n Total time used: "+QString::number(totaltime)+" seconds\n");
    }
    if(guiSettings->value("showLog").toBool()){
        TextViewer t(this, &log);     
        t.exec();
    }
}

/*
void PlaylistManager::copyFiles( PlayList p, QList<M3uEntry> songs, QString *log ){

    log->append("\nResult from file copy:\n");
    int nCopied=0;

    qDebug()<<"starting to copy "<<songs.size()<<" files!";
    QProgressDialog pr("Copying files for playlist "+p.name()+" to "+p.copyFilesToDir(), "Abort", 0, songs.size(), this);
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
        QString newname = p.copyFilesToDir() + "/" + songs[j].file();
        bool okf = f.copy( newname );
        //qDebug()<<"copy result of "<<songs[j].originalfile<<" -> "<<newname<<": "<<okf;
        if(!okf){
            QFile f2(newname);
            if( f2.exists() ){
                log->append( songs[j].file() + " was not copied as it already exists in "+p.copyFilesToDir()+"\n" );
            }else{
                log->append( songs[j].file() + " could not be copied to "+p.copyFilesToDir()+"\n" );
            }
        }else{
            nCopied++;
        }
    }
    pr.setValue(songs.size());
    pr.close();
    log->append(QString::number(nCopied)+" of "+QString::number(songs.size())+" files copied to "+p.copyFilesToDir()+"\n");

}
*/

QString PlaylistManager::newUniqePlayListName(){

    QString name = "New Playlist";
    int k=0;
    while (1){
        if(k>0){
            name = "New Playlist"+QString::number(k);
        }
        bool ok = true;
        for(int i=0;i<playListTable->count();i++){
            if( static_cast<PlayList*>(playListTable->item(i))->name()==name ){
                ok = false;
                break;
            }
        }
        if(ok){
            break;
        }
        k++;
    }

    return name;
}


void PlaylistManager::addPlayList(){

    QString name = newUniqePlayListName();
    PlayList *p = new PlayList(name);
    p->setFlags(p->flags () | Qt::ItemIsEditable);
    playListTable->addItem( p );
    playListTable->setCurrentItem(p);


}

void PlaylistManager::removePlayList(){

    QModelIndexList indexes = playListTable->selectionModel()->selectedIndexes();

    if(indexes.size()==0){
        return;
    }

    qDebug()<<"indexes.size() "<<indexes.size();
    enableOptions( false );
    clearRulesAndFolders();
    playListTable->blockSignals(true);
    for(int i=indexes.size()-1;i>=0;i--){                        
        delete playListTable->takeItem( indexes[i].row() );                        
    }
    playListTable->blockSignals(false);
    if( playListTable->count()>0 ){
        playListTable->setCurrentRow(0);
    }


}

void PlaylistManager::newRule(){

    PlayList *p = currentPlayList();
    if(!p){
        QMessageBox::information(this, "",
                                 "Please select a playlist first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    RuleDialog rd;
    if( rd.exec()!=QDialog::Accepted ){
        return;
    }
    Rule r = rd.getSettings();
    QVector<Rule> rules = p->rules();
    rules.append( r );
    p->setRules(rules);

    showRulesAndFolders();


}

void PlaylistManager::editRule(){

    PlayList *p = currentPlayList();
    int rind = rulesTable->currentRow();
    if(rind==-1 || !p){
        QMessageBox::information(this, "",
                                 "Please select a Rule first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    QVector<Rule> rules = p->rules();
    Rule r = rules[rind];
    RuleDialog rd( &r, this );

    if( rd.exec()!=QDialog::Accepted ){
        return;
    }
    Rule newr = rd.getSettings();
    rules.append(newr);
    p->setRules(rules);
    delete rulesTable->takeItem(rind);
    rulesTable->insertItem( rind, Rule::getRuleName( newr.type() )+": "+newr.value() );
    showRulesAndFolders();

}

void PlaylistManager::removeRule(){

    PlayList *p = currentPlayList();
    int rind = rulesTable->currentRow();
    if(rind==-1 || !p){
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



void PlaylistManager::showSettings(){

    guiSettings->sync();

    SettingsDialog s(this);
    if( s.exec()==QDialog::Accepted ){

        updateUseScript();

        if(guiSettings->value("keepTags").toBool()){
            tags_.clear();
        }
    }


}





