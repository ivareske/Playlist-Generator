
#include "playlistManager.h"


playlistManager::playlistManager(QWidget *parent) : QMainWindow(parent){
    setupUi(this); // this sets up GUI			

    qRegisterMetaType<playList>("playList");
    qRegisterMetaTypeStreamOperators<playList>("playList");
    qRegisterMetaType<rule>("rule");
    qRegisterMetaTypeStreamOperators<rule>("rule");
    qRegisterMetaType<settingsClass>("settingsClass");
    qRegisterMetaTypeStreamOperators<settingsClass>("settingsClass");    


    createActions();
    /*qApp->addLibraryPath("U:/Programmering/c++/QT/qt-labs-qtscriptgenerator/plugins");
	qApp->addLibraryPath("U:/Programmering/c++/QT/qt-labs-qtscriptgenerator/plugins/script");
	qApp->addLibraryPath("U:/Programmering/c++/QT/qt-labs-qtscriptgenerator/");
	QStringList paths = qApp->libraryPaths();
    paths <<  QString("U:/Programmering/c++/QT/qt-labs-qtscriptgenerator-4.6.3/plugins");
    qApp->setLibraryPaths(paths);
	*/
    guiSettings = new QSettings("playListGenerator"+Global::ext,QSettings::IniFormat,this);
    readGUISettings();
    playListSettings = new QSettings( settingsFile.fileName(), QSettings::IniFormat, this );
    initialize( settingsFile );
    

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


void playlistManager::runscript(){

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

void playlistManager::openStyleSheet(){

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
    settings_.setStyle(fileName);
    //lastStyleSheetFolder = f.absolutePath();

}

bool playlistManager::loadStyleSheet( QString file ){
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

void playlistManager::createActions(){

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
    connect( actionSave, SIGNAL( triggered() ), this, SLOT( saveCurrentSettings() ) );
    connect( actionSaveAs, SIGNAL( triggered() ), this, SLOT( saveSettingsAs() ) );
    connect( actionOpen, SIGNAL( triggered() ), this, SLOT( open() ) );
    connect( actionNew, SIGNAL( triggered() ), this, SLOT( newCollection() ) );
    connect( actionClearTags, SIGNAL( triggered() ), this, SLOT( clearTags() ) );

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

void playlistManager::clearTags(){

    tags_.clear();
    statusBar()->showMessage("Tags cleared", 8000);
}

void playlistManager::getCopyDir(){

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
        playList *p = currentPlayList();
        if(p){
            p->setCopyFilesToDir(d[0]);
        }
    }
}

void playlistManager::updateCopyFiles( const QString &text ){

    playList *p = currentPlayList();
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

void playlistManager::updateCopyTo( int state ){

    playList *p = currentPlayList();
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

void playlistManager::newCollection(){

    QString dir = settingsFile.absoluteFilePath();
    if(dir.isEmpty()){
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Name of new playlist collection"),dir, "*"+Global::ext);

    if( fileName.isNull() || fileName.isEmpty() ){
        return;
    }

    qDebug()<<"New collection: "<<fileName;
    QFileInfo file = QFileInfo(fileName);
    playListSettings = new QSettings(file.absoluteFilePath(),QSettings::IniFormat,this);
    playListSettings->clear();
    playListSettings->setValue( "playListGeneratorFile", true ); //identifier to be accepted in initialize/readSettings as a valid file
    playListSettings->sync();
    //readGUISettings(); //no need
    playListTable->blockSignals(true);
    initialize( file );
    saveSettings( file );
    playListTable->blockSignals(false);
    showRulesAndFolders();
}


void playlistManager::uncheckStyleActions(){

    QList<QAction *> actions = menuStyle->actions();
    for(int i=0;i<actions.size();i++){
        actions[i]->setChecked(false);
    }
}

void playlistManager::checkStyleAction( QString actionText, bool state ){

    QList<QAction *> actions = menuStyle->actions();
    for(int i=0;i<actions.size();i++){
        if( actions[i]->text()==actionText ){
            actions[i]->setChecked(state);
        }
    }
}

void playlistManager::setGUIStyle( const QString &s ){

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
                settings_.setStyle(s);
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
        settings_.setStyle(s);
    }

}

void playlistManager::writeGUISettings(){

    guiSettings->clear();
    guiSettings->beginGroup("MainWindow");
    guiSettings->setValue("size", this->size());
    guiSettings->setValue("pos", this->pos());
    guiSettings->endGroup();
    guiSettings->setValue("settingsFile", settingsFile.absoluteFilePath() );
    guiSettings->setValue("settingsClass", settings_ );

    guiSettings->sync();
    //delete guiSettings;



}

void playlistManager::readGUISettings(){

    guiSettings->beginGroup("MainWindow");
    this->resize(guiSettings->value("size", QSize(400, 400)).toSize());
    this->move(guiSettings->value("pos", QPoint(200, 200)).toPoint());
    guiSettings->endGroup();
    settingsFile = guiSettings->value("settingsFile","examples"+Global::ext).toString();
    settings_ = guiSettings->value("settingsClass",settingsClass()).value<settingsClass>();

}


void playlistManager::open(){

    QString dir = settingsFile.absoluteFilePath();
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

void playlistManager::initialize( QFileInfo file ){

    qDebug()<<"Initializing";
    //playLists_.clear();
    Global::fileReadResult ok = readSettings( file );
    if(ok==Global::NOTVALID){
        QMessageBox::critical(this, "",
                              file.absoluteFilePath()+" is not a valid playListGenerator file",
                              QMessageBox::Ok, QMessageBox::Ok);
        //settingsFile = QFileInfo("New Collection"+ext);
        //return;
    }else if(ok==Global::DOESNOTEXIST){
        QMessageBox::critical(this, "",
                              file.absoluteFilePath()+" does not exist",
                              QMessageBox::Ok, QMessageBox::Ok);
        //settingsFile = QFileInfo("New Collection"+ext);
        //return;
    }
    enableOptions( false );
    //sortPlayLists();
    setGUIStyle( settings_.style() );
    updateUseScript();
    settingsFile = file;
    this->setWindowTitle( "Current playlist collection: "+settingsFile.absoluteFilePath() );
}

void playlistManager::updateUseScript(){
    if( settings_.useScript() ){
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
void playlistManager::giveUniqueIds(){

    for(int i=0;i<playLists_.size();i++){
        playLists_[i].setUniqueId(i);
    }

}
*/

/*
void playlistManager::sortPlayLists(){

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

void playlistManager::addIndividualFiles(){

    playList *p = currentPlayList();
    if(!p){
        return;
    }

    addFilesDialog d(p->individualFiles());
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


void playlistManager::enableOptions( bool state ){

    foldersGroupBox->setEnabled( state );
    rulesGroupBox->setEnabled( state );
    optionsGroupBox->setEnabled( state );
    addFilesButton->setEnabled( state );
    copyFilesFrame->setEnabled( state );
}

void playlistManager::updateRandomize( int state ){	

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setRandomize(randomize->isChecked());

}

playList* playlistManager::currentPlayList(){
    playList *p = static_cast<playList*>(playListTable->currentItem());
    return p;
}


void playlistManager::updateAllRulesTrue( int state ){	

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setAllRulesTrue(allRulesTrue->isChecked());

}

void playlistManager::updateSearchSubfolders( int state ){	

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setIncludeSubFolders(searchSubFolders->isChecked());

}

void playlistManager::updateIncludeExtInf( int state ){	

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setIncludeExtInf(includeExtInf->isChecked());

}

void playlistManager::updateUseRelativePath( int state ){	

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setRelativePath(relativePath->isChecked());

}


void playlistManager::updateMakeUnique( int state ){	

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setMakeUnique(makeUnique->isChecked());

}

void playlistManager::updateScript(){

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the script",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setScript(RuleScript->toPlainText());

}

void playlistManager::updateExtensions( const QString & exts ){

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the extensions",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    p->setExtensions(exts.split(";"));
}

void playlistManager::addFolder(){

    playList *p = currentPlayList();
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

void playlistManager::renameFolder(QListWidgetItem *item){

    playList *p = currentPlayList();
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

void playlistManager::changeFolder(QListWidgetItem *item){

    playList *p = currentPlayList();
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

void playlistManager::removeFolder(){

    playList *p = currentPlayList();
    int ind = folderTable->currentRow();    
    if(ind==-1 || !p){
        return;
    }
    QList<QDir> folders = p->folders();
    folders.removeAt( ind );
    delete folderTable->takeItem( ind );
    p->setFolders(folders);
}

void playlistManager::closeEvent( QCloseEvent *event ){

    if( !settingsFile.exists() ){
        saveSettingsAs();
    }else{
        saveSettings( settingsFile );
    }
    writeGUISettings();
}

void playlistManager::clearRulesAndFolders(){

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

void playlistManager::showRulesAndFolders(){

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
    playList *p = currentPlayList();
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
    QVector<rule> rules = p->rules();
    for(int i=0;i<rules.size();i++){
        rule::ruleType t = rules[i].type();
        QListWidgetItem *item = new QListWidgetItem();
        if(!rules[i].shouldBeTrue()){
            QFont f = item->font(); f.setItalic(true);
            item->setFont(f);
        }
        item->setText( Global::getRuleName(t)+": "+ rules[i].value() );
        rulesTable->addItem( item );
    }

}

/*
void playlistManager::renamePlayList(QListWidgetItem *item){

    playList *p = static_cast<playList*>(item);
    QString oldName = p->name();
    QString newName = item->text();
    if(newName.isEmpty()){
        QMessageBox::critical(this, "",
                              "A playlist cannot have an empty name",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        item->setText( oldName );
        return;
    }
    playLists_[ind].name() = newName;
    qDebug()<<"renamed from "<<oldName<<" to "<<item->text();

    sortPlayLists();

}
*/
Global::fileReadResult playlistManager::readSettings( QFileInfo file ){
    qDebug()<<"Reading settings...";

    if( !file.exists() ){
        return Global::DOESNOTEXIST;
    }

    playListSettings = new QSettings( file.absoluteFilePath(), QSettings::IniFormat, this );
    /*bool valid = playListSettings->value( "playListGeneratorFile", false ).toBool(); //identifier
    if(!valid){
        return NOTVALID;
    }
    */

    QStringList playListNames = playListSettings->allKeys();
    for(int i=0;i<playListNames.size();i++){
        playList p = playListSettings->value(playListNames[i]).value< playList >();
        playList *p2 = new playList(p);
        p2->setFlags(p2->flags () | Qt::ItemIsEditable);
        playListTable->addItem( p2 );
    }


    return Global::EXISTS;

}

void playlistManager::saveSettingsAs(){

    QString dir = settingsFile.absoluteFilePath();
    if(dir.isEmpty()){
        dir = qApp->applicationDirPath();//QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), dir, "*"+Global::ext);

    if( fileName.isNull() || fileName.isEmpty() ){
        return;
    }
    qDebug()<<"New filename: "<<fileName;
    QFileInfo file = QFileInfo( fileName );
    //settings.saveSettings( settingsFile.absoluteFilePath(), playLists_ );
    saveSettings( file );
    initialize( file );
    //statusBar()->showMessage("'"+settingsFile.baseName()+"' saved!", 4000);

}

void playlistManager::saveCurrentSettings(){		
    saveSettings( settingsFile );
}

void playlistManager::saveSettings( QFileInfo file ){

    if( !file.absoluteDir().exists() || file.fileName().isEmpty() ){
        saveSettingsAs();
        return;
    }

    playListSettings->clear();
    for(int i=0;i<playListTable->count();i++){
        playList *p = static_cast<playList*>(playListTable->item(i));
        playListSettings->setValue(p->name(),*p);
    }


    playListSettings->sync();

    statusBar()->showMessage("'"+file.baseName()+"' saved!", 8000);

}


void playlistManager::generateSelectedPlayLists(){

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

void playlistManager::generateAllPlayLists(){

    QList<int> inds;
    for(int i=0;i<playListTable->count();i++){
        inds.append(i);
    }

    generatePlayLists(inds);

}


void playlistManager::generatePlayLists( QList<int> inds ){

    if( inds.size()==0 ){
        return;
    }

    QDir d(settings_.defaultOutputFolder());
    if( settings_.defaultOutputFolder().isEmpty() || !d.exists() ){
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

        playList *p = static_cast<playList*>(playListTable->item(i));
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

        QList<m3uEntry> songs;
        bool wasCanceled = p->generate( &songs, statusBar(), &log, &tags_, settings_ );
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
    if(settings_.showLog()){
        textViewer t(this, &log);
        t.resize(settings_.textViewerSize());
        t.exec();
        settings_.setTextViewerSize(t.size());
    }
}

/*
void playlistManager::copyFiles( playList p, QList<m3uEntry> songs, QString *log ){

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

void playlistManager::addPlayList(){

    QString name = "New playlist";

    playList *p = new playList( settings_.defaultExtensions() );
    p->setFlags(p->flags () | Qt::ItemIsEditable);
    playListTable->addItem( p );
    playListTable->setCurrentItem(p);


}

void playlistManager::removePlayList(){

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

void playlistManager::newRule(){

    playList *p = currentPlayList();
    if(!p){
        QMessageBox::information(this, "",
                                 "Please select a playlist first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    ruleDialog rd;
    if( rd.exec()!=QDialog::Accepted ){
        return;
    }
    rule r = rd.getSettings();
    QVector<rule> rules = p->rules();
    rules.append( r );
    p->setRules(rules);

    showRulesAndFolders();


}

void playlistManager::editRule(){

    playList *p = currentPlayList();
    int rind = rulesTable->currentRow();
    if(rind==-1 || !p){
        QMessageBox::information(this, "",
                                 "Please select a rule first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    QVector<rule> rules = p->rules();
    rule r = rules[rind];
    ruleDialog rd( &r, this );

    if( rd.exec()!=QDialog::Accepted ){
        return;
    }
    rule newr = rd.getSettings();
    rules.append(newr);
    p->setRules(rules);
    delete rulesTable->takeItem(rind);
    rulesTable->insertItem( rind, Global::getRuleName( newr.type() )+": "+newr.value() );
    showRulesAndFolders();

}

void playlistManager::removeRule(){

    playList *p = currentPlayList();
    int rind = rulesTable->currentRow();
    if(rind==-1 || !p){
        QMessageBox::information(this, "",
                                 "Please select a rule first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    delete rulesTable->takeItem(rind);
    QVector<rule> rules = p->rules();
    rules.remove(rind);
    p->setRules(rules);

}



void playlistManager::showSettings(){

    settingsDialog s(&settings_);
    //guiSettings->sync();
    if( s.exec()==QDialog::Accepted ){
        updateUseScript();
        if(!settings_.keepTags()){
            tags_.clear();
        }
    }


}





