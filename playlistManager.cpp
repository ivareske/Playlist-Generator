
#include "playlistManager.h"


playlistManager::playlistManager(QWidget *parent){
    setupUi(this); // this sets up GUI			
    createActions();
    /*qApp->addLibraryPath("U:/Programmering/c++/QT/qt-labs-qtscriptgenerator/plugins");
	qApp->addLibraryPath("U:/Programmering/c++/QT/qt-labs-qtscriptgenerator/plugins/script");
	qApp->addLibraryPath("U:/Programmering/c++/QT/qt-labs-qtscriptgenerator/");
	QStringList paths = qApp->libraryPaths();
    paths <<  QString("U:/Programmering/c++/QT/qt-labs-qtscriptgenerator-4.6.3/plugins");
    qApp->setLibraryPaths(paths);
	*/
    guiSettings = new QSettings("playListGenerator"+ext,QSettings::IniFormat,this);
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
    style = fileName;
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

    connect( playListTable, SIGNAL( itemChanged ( QListWidgetItem *) ), this, SLOT( renamePlayList(QListWidgetItem *) ) );
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

    tags.clear();
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
        int ind = playListTable->currentRow();
        if(ind!=-1){
            playLists[ind].copyFilesToDir = d[0];
        }
    }
}

void playlistManager::updateCopyFiles( const QString &text ){

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the extensions",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    qDebug()<<"copy files dir set to "<<text<<" for "<<playLists[ind].name;
    playLists[ind].copyFilesToDir = copyFilesText->text();
}

void playlistManager::updateCopyTo( int state ){

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the extensions",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    qDebug()<<"copy files set to "<<state<<" for "<<playLists[ind].name;
    playLists[ind].copyFiles = copyFilesCheckBox->isChecked();
}

void playlistManager::newCollection(){

    QString dir = settingsFile.absoluteFilePath();
    if(dir.isEmpty()){
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Name of new playlist collection"),dir, "*"+ext);

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
                uncheckStyleActions(); style = s;
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
        style = s;
    }

}

void playlistManager::writeGUISettings(){

    guiSettings->clear();
    guiSettings->beginGroup("MainWindow");
    guiSettings->setValue("size", this->size());
    guiSettings->setValue("pos", this->pos());
    guiSettings->endGroup();
    guiSettings->setValue("settingsFile", settingsFile.absoluteFilePath() );
    guiSettings->setValue("style", style );
    guiSettings->setValue("artistEmpty",artistEmpty);
    guiSettings->setValue("titleEmpty",titleEmpty);
    guiSettings->setValue("albumEmpty",albumEmpty);
    guiSettings->setValue("commentEmpty",commentEmpty);
    guiSettings->setValue("genreEmpty",genreEmpty);
    guiSettings->setValue("trackEmpty",trackEmpty);
    guiSettings->setValue("yearEmpty",yearEmpty);
    guiSettings->setValue("defaultOutputFolder",defaultOutputFolder);
    guiSettings->setValue("defaultExtensions",defaultExtensions);
    guiSettings->setValue("format",format);
    guiSettings->setValue("useCopyFilesToPath",useCopyFilesToPath);
    guiSettings->setValue("showLog",showLog);
    guiSettings->setValue("ShowTagLibDebug",ShowTagLibDebug);
    guiSettings->setValue("useScript",useScript);
    guiSettings->setValue("keepTags",keepTags);
    guiSettings->setValue("textViewerSize",textViewerSize);
    guiSettings->sync();
    //delete guiSettings;



}

void playlistManager::readGUISettings(){

    guiSettings->beginGroup("MainWindow");
    this->resize(guiSettings->value("size", QSize(400, 400)).toSize());
    this->move(guiSettings->value("pos", QPoint(200, 200)).toPoint());
    guiSettings->endGroup();
    settingsFile = guiSettings->value("settingsFile","examples"+ext).toString();
    style = guiSettings->value("style","WindowsXP").toString();
    artistEmpty = guiSettings->value("artistEmpty",true).toBool();
    titleEmpty = guiSettings->value("titleEmpty",true).toBool();
    albumEmpty = guiSettings->value("albumEmpty",false).toBool();
    commentEmpty = guiSettings->value("commentEmpty",false).toBool();
    genreEmpty = guiSettings->value("genreEmpty",false).toBool();
    trackEmpty = guiSettings->value("trackEmpty",false).toBool();
    yearEmpty = guiSettings->value("yearEmpty",false).toBool();
    defaultOutputFolder = guiSettings->value("defaultOutputFolder",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString();
    defaultExtensions = guiSettings->value("defaultExtensions",defaultExt.split(";")).toStringList();
    format = guiSettings->value("format","%artist% - %title%").toString();
    useScript = guiSettings->value("useScript",false).toBool();
    showLog = guiSettings->value("showLog",true).toBool();
    ShowTagLibDebug = guiSettings->value("ShowTagLibDebug",true).toBool();
    useCopyFilesToPath = guiSettings->value("useCopyFilesToPath",true).toBool();
    keepTags = guiSettings->value("keepTags",true).toBool();
    textViewerSize = guiSettings->value("textViewerSize",QSize(760,350)).toSize();

}


void playlistManager::open(){

    QString dir = settingsFile.absoluteFilePath();
    if(dir.isEmpty()){
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),dir, "*"+ext);
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
    //playLists.clear();
    fileReadResult ok = readSettings( file );
    if(ok==NOTVALID){
        QMessageBox::critical(this, "",
                              file.absoluteFilePath()+" is not a valid playListGenerator file",
                              QMessageBox::Ok, QMessageBox::Ok);
        //settingsFile = QFileInfo("New Collection"+ext);
        //return;
    }else if(ok==DOESNOTEXIST){
        QMessageBox::critical(this, "",
                              file.absoluteFilePath()+" does not exist",
                              QMessageBox::Ok, QMessageBox::Ok);
        //settingsFile = QFileInfo("New Collection"+ext);
        //return;
    }
    enableOptions( false );
    sortPlayLists();
    setGUIStyle( style );
    if( useScript ){
        rulesFrame->hide();
        RuleScript->show();
        allRulesTrue->setEnabled(false);
    }else{
        rulesFrame->show();
        RuleScript->hide();
        allRulesTrue->setEnabled(true);
    }
    settingsFile = file;
    this->setWindowTitle( "Current playlist collection: "+settingsFile.absoluteFilePath() );
}

void playlistManager::giveUniqueIds(){

    for(int i=0;i<playLists.size();i++){
        playLists[i].uniqueId = i;
    }

}



void playlistManager::sortPlayLists(){

    //give unique ids
    giveUniqueIds();
    //get currently selected row
    int nowRow = playListTable->currentRow();
    qDebug()<<"nowRow "<<nowRow;

    int id=-1;
    if( nowRow>=0 && nowRow<playLists.size() ){
        id = playLists[nowRow].uniqueId;
    }

    //do sorting
    qSort( playLists.begin(), playLists.end(), Global::caseInsensitiveLessThan );
    playListTable->clear();
    for(int i=0;i<playLists.size();i++){
        QListWidgetItem *item = new QListWidgetItem();
        item->setText( playLists[i].name );
        item->setFlags(item->flags () | Qt::ItemIsEditable);
        playListTable->addItem( item );
    }

    //find and select previously selected playlist
    if( nowRow>=0 && nowRow<playLists.size() ){
        qDebug()<<"unique id of playlist is "<<id;
        int row=-1;
        for(int i=0;i<playLists.size();i++){
            if(playLists[i].uniqueId==id){
                row = i;
                qDebug()<<"found playlist with id "<<id<<" at row "<<row<<": "<<playLists[row].name;
                break;
            }
        }
        if(row>=0){
            playListTable->setCurrentRow(row);
        }
    }

    showRulesAndFolders();

}

void playlistManager::addIndividualFiles(){

    int ind = playListTable->currentRow();
    if(ind==-1){
        return;
    }

    addFilesDialog d;
    d.setFiles( playLists[ind].individualFiles );
    if( d.exec()!=QDialog::Accepted ){
        return;
    }
    QStringList files = d.getFiles();

    playLists[ind].individualFiles = files;

    //show bold text on "add individual files" if individual files exist
    QFont f = addFilesButton->font();
    if( playLists[ind].individualFiles.size()>0 ){
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

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    playLists[ind].randomize = randomize->isChecked();

}

void playlistManager::updateAllRulesTrue( int state ){	

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    playLists[ind].allRulesTrue = allRulesTrue->isChecked();

}

void playlistManager::updateSearchSubfolders( int state ){	

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    playLists[ind].includeSubFolders = searchSubFolders->isChecked();

}

void playlistManager::updateIncludeExtInf( int state ){	

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    playLists[ind].includeExtInf = includeExtInf->isChecked();

}

void playlistManager::updateUseRelativePath( int state ){	

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    playLists[ind].relativePath = relativePath->isChecked();

}

void playlistManager::updateMakeUnique( int state ){	

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist first",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    playLists[ind].makeUnique = makeUnique->isChecked();

}

void playlistManager::updateScript(){

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the script",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    playLists[ind].script = RuleScript->toPlainText();

}

void playlistManager::updateExtensions( const QString & exts ){

    int ind = playListTable->currentRow();
    if(ind==-1){
        QMessageBox::critical(this, "",
                              "Please choose a playlist before editing the extensions",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    playLists[ind].extensions = exts.split(";");
    qDebug()<<"setiings playlist"<<ind<<" extensions to "<<exts;
}

void playlistManager::addFolder(){

    int ind = playListTable->currentRow();
    if(ind==-1){
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
    if( !dialog.exec() ){
        return;
    }
    QStringList d = dialog.selectedFiles();


    for(int i=0;i<d.size();i++){
        if(i==0){
            QFileInfo f(d[i]);
            guiSettings->setValue("lastFolder",f.absolutePath());
        }
        qDebug()<<d[i];
        QListWidgetItem *item = new QListWidgetItem();
        item->setText( d[i] );
        item->setFlags(item->flags () & ~Qt::ItemIsEditable);
        folderTable->addItem( item );
        playLists[ind].folders.append(d[i]);
    }
}

void playlistManager::renameFolder(QListWidgetItem *item){

    int pind = playListTable->currentRow();
    int ind = folderTable->currentRow();
    QString oldName = playLists[pind].folders[ind];
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
    playLists[pind].folders[ind] = newName;
    qDebug()<<"renamed from "<<oldName<<" to "<<item->text();

}

void playlistManager::changeFolder(QListWidgetItem *item){

    int pind = playListTable->currentRow();
    int ind = folderTable->currentRow();
    QString oldName = playLists[pind].folders[ind];
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    oldName,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if( dir.isEmpty() ){
        return;
    }
    item->setText( dir );
    playLists[pind].folders[ind] = dir;

}

void playlistManager::removeFolder(){
    int pind = playListTable->currentRow();
    int ind = folderTable->currentRow();
    if(ind==-1){
        return;
    }
    playLists[pind].folders.removeAt( ind );
    delete folderTable->takeItem( ind );
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
    int ind = playListTable->currentRow();
    if( ind==-1 ){
        enableOptions( false );
        return;
    }
    qDebug()<<"c";
    enableOptions( true );
    qDebug()<<"d";
    //folders
    qDebug()<<"ind "<<ind<<"playLists.size() "<<playLists.size();
    for(int i=0;i<playLists[ind].folders.size();i++){
        QString t = playLists[ind].folders[i];
        QListWidgetItem *item = new QListWidgetItem();
        item->setFlags(item->flags () & ~Qt::ItemIsEditable);
        item->setText( t );
        folderTable->addItem( item );
    }
    qDebug()<<"e";
    //extensions & settings
    extensions->setText( playLists[ind].extensions.join(";") );
    randomize->setChecked( playLists[ind].randomize );
    searchSubFolders->setChecked( playLists[ind].includeSubFolders );
    allRulesTrue->setChecked( playLists[ind].allRulesTrue );
    includeExtInf->setChecked( playLists[ind].includeExtInf );
    relativePath->setChecked( playLists[ind].relativePath );
    makeUnique->setChecked( playLists[ind].makeUnique );
    copyFilesCheckBox->setChecked( playLists[ind].copyFiles );
    copyFilesText->setText( playLists[ind].copyFilesToDir );
    qDebug()<<"f";
    //show bold text on "add individual files" if individual files exist
    QFont f = addFilesButton->font();
    if( playLists[ind].individualFiles.size()>0 ){
        f.setBold(true);
    }else{
        f.setBold(false);
    }
    addFilesButton->setFont(f);

    //rules
    RuleScript->setText( playLists[ind].script );
    for(int i=0;i<playLists[ind].rules.size();i++){
        ruleType t = playLists[ind].rules[i].type;
        QListWidgetItem *item = new QListWidgetItem();
        if(!playLists[ind].rules[i].shouldBeTrue){
            QFont f = item->font(); f.setItalic(true);
            item->setFont(f);
        }
        item->setText( Global::getRuleName(t)+": "+playLists[ind].rules[i].value );
        rulesTable->addItem( item );
    }

}

void playlistManager::renamePlayList(QListWidgetItem *item){

    int ind = playListTable->currentRow(); //item->data( Qt::UserRole ).toInt();
    qDebug()<<"index: "<<ind<<" playLists.size(): "<<playLists.size();
    QString oldName = playLists[ind].name;
    QString newName = item->text();
    if(newName.isEmpty()){
        QMessageBox::critical(this, "",
                              "A playlist cannot have an empty name",
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        item->setText( oldName );
        return;
    }
    playLists[ind].name = newName;
    qDebug()<<"renamed from "<<oldName<<" to "<<item->text();

    sortPlayLists();

}

fileReadResult playlistManager::readSettings( QFileInfo file ){
    qDebug()<<"Reading settings...";

    if( !file.exists() ){
        return DOESNOTEXIST;
    }

    playListSettings = new QSettings( file.absoluteFilePath(), QSettings::IniFormat, this );
    bool valid = playListSettings->value( "playListGeneratorFile", false ).toBool(); //identifier
    if(!valid){
        return NOTVALID;
    }

    QStringList plists = playListSettings->childGroups();
    playLists.clear();
    for(int i=0;i<plists.size();i++){
        playList p( defaultExtensions );
        p.name = playListSettings->value( plists[i]+"/name", p.name ).toString();
        p.extensions = playListSettings->value( plists[i]+"/extensions", p.extensions ).toStringList();
        p.randomize = playListSettings->value( plists[i]+"/randomize", p.randomize ).toBool();
        p.includeSubFolders = playListSettings->value( plists[i]+"/includeSubFolders", p.includeSubFolders ).toBool();
        p.relativePath = playListSettings->value( plists[i]+"/relativePath", p.relativePath ).toBool();
        p.copyFilesToDir = playListSettings->value( plists[i]+"/copyFilesToDir", p.copyFilesToDir ).toString();
        p.copyFiles = playListSettings->value( plists[i]+"/copyFiles", p.copyFiles ).toBool();
        p.allRulesTrue = playListSettings->value( plists[i]+"/allRulesTrue", p.allRulesTrue ).toBool();
        p.includeExtInf = playListSettings->value( plists[i]+"/includeExtInf", p.includeExtInf ).toBool();
        p.makeUnique = playListSettings->value( plists[i]+"/makeUnique", p.makeUnique ).toBool();
        p.script = playListSettings->value( plists[i]+"/script", p.script ).toString();
        p.folders = playListSettings->value( plists[i]+"/folders", p.folders ).toStringList();
        p.folders.removeAll("");
        for(int j=0;j<(int)NUMBEROFRULES;j++){
            QString iniRule = plists[i]+"/rule"+QString::number(j);
            if( playListSettings->contains( iniRule ) ){
                rule r;
                QString tmp = playListSettings->value( iniRule, "" ).toString();
                QStringList split = tmp.split(";");
                if(split.size()==4){
                    r.type = (ruleType)split[0].toInt();
                    r.value = split[1];
                    r.shouldBeTrue = (bool)split[2].toInt();
                    r.caseSensitive = (bool)split[3].toInt();
                }
                qDebug()<<"found rule "<<Global::getRuleName( r.type );
                p.rules.append(r);
            }else{
                break;
            }
        }
        p.individualFiles = playListSettings->value( plists[i]+"/individualFiles", p.individualFiles ).toStringList();
        p.individualFiles.removeAll("");

        playLists.append(p);
    }


    for(int i=0;i<playLists.size();i++){
        //add playlists to table
        QListWidgetItem *item = new QListWidgetItem();
        item->setText( playLists[i].name );
        item->setFlags(item->flags () | Qt::ItemIsEditable);
        playListTable->addItem( item );
    }

    return EXISTS;

}

void playlistManager::saveSettingsAs(){

    QString dir = settingsFile.absoluteFilePath();
    if(dir.isEmpty()){
        dir = qApp->applicationDirPath();//QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), dir, "*"+ext);

    if( fileName.isNull() || fileName.isEmpty() ){
        return;
    }
    qDebug()<<"New filename: "<<fileName;
    QFileInfo file = QFileInfo( fileName );
    //settings.saveSettings( settingsFile.absoluteFilePath(), playLists );
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

    qDebug()<<"saving :"<<file.absoluteFilePath();
    playListSettings = new QSettings( file.absoluteFilePath(), QSettings::IniFormat, this );
    playListSettings->clear();
    playListSettings->setValue( "playListGeneratorFile", true ); //identifier
    for(int i=0;i<playLists.size();i++){
        playList p = playLists[i];
        playListSettings->beginGroup(QString::number(i));
        playListSettings->setValue( "name", p.name );
        playListSettings->setValue( "extensions", p.extensions );
        playListSettings->setValue( "randomize", p.randomize );
        playListSettings->setValue( "includeSubFolders", p.includeSubFolders );
        playListSettings->setValue( "relativePath", p.relativePath );
        playListSettings->setValue( "copyFilesToDir", p.copyFilesToDir );
        playListSettings->setValue( "allRulesTrue", p.allRulesTrue );
        playListSettings->setValue( "includeExtInf", p.includeExtInf );
        playListSettings->setValue( "copyFiles", p.copyFiles );
        playListSettings->setValue( "makeUnique", p.makeUnique );
        playListSettings->setValue( "folders", p.folders );
        playListSettings->setValue( "script", p.script );
        for(int j=0;j<p.rules.size();j++){
            QString ruleStr = QString::number( (ruleType)p.rules[j].type );
            playListSettings->setValue( "rule"+QString::number(j), ruleStr+";"+p.rules[j].value+";"+QString::number((int)p.rules[j].shouldBeTrue)+";"+QString::number((int)p.rules[j].caseSensitive) );
        }
        playListSettings->setValue( "individualFiles", p.individualFiles );
        playListSettings->endGroup();
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

    if( playLists.size()==0 ){
        QMessageBox::information(this, "",
                                 "Please select a playlist first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    QList<int> inds;
    for(int i=0;i<playLists.size();i++){
        inds.append(i);
    }

    generatePlayLists(inds);

}


void playlistManager::generatePlayLists( QList<int> inds ){

    QDir d(defaultOutputFolder);
    if( defaultOutputFolder.isEmpty() || !d.exists() ){
        QMessageBox::critical(this, "",
                              "Output folder is not a valid path",
                              QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    writeGUISettings();

    QString log;
    time_t Start_t, End_t;
    int totaltime; QStringList names;
    Start_t = time(NULL);
    for(int i=0;i<inds.size();i++){
        names.append( playLists[inds[i]].name );
        log.append("Info for generation of playlist '"+playLists[inds[i]].name+"'\n");

        bool ok=true;
        if( playLists[inds[i]].copyFiles ){
            QDir c(playLists[inds[i]].copyFilesToDir);
            if( !c.exists() ){
                QDir newDir;
                ok = newDir.mkpath( playLists[inds[i]].copyFilesToDir );
                if(ok){
                    log.append("Created directory "+playLists[inds[i]].copyFilesToDir+"\n");
                }else{
                    log.append("Could not create directory "+playLists[inds[i]].copyFilesToDir+", no copying performed\n");
                }
            }
        }

        QList<m3uEntry> songs;
        bool wasCanceled = playLists[inds[i]].generate( &songs, statusBar(), &log, &tags );
        if(wasCanceled){
            log.append("\n\nAborted by user");
            break;
        }
        if( playLists[inds[i]].copyFiles && ok ){
            copyFiles( playLists[inds[i]], songs, &log );
        }
        log.append("\n----------------------------------------------------------\n");
    }
    End_t = time(NULL);
    if(names.size()!=names.toSet().toList().size()){
        //if some names are equal
        log.append("Warning, some playlists have identical names!\n");
    }
    if(inds.size()>1){
        totaltime = difftime(End_t, Start_t);
        log.append("\n\n Total time used: "+QString::number(totaltime)+" seconds\n");
    }
    if(showLog){
        textViewer t(this, &log);
        t.resize(textViewerSize);
        t.exec();
        textViewerSize=t.size();
    }
}

void playlistManager::copyFiles( playList p, QList<m3uEntry> songs, QString *log ){

    log->append("\nResult from file copy:\n");
    int nCopied=0;

    qDebug()<<"starting to copy "<<songs.size()<<" files!";
    QProgressDialog pr("Copying files for playlist "+p.name+" to "+p.copyFilesToDir, "Abort", 0, songs.size(), this);
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
        QString newname = p.copyFilesToDir + "/" + songs[j].file();
        bool okf = f.copy( newname );
        //qDebug()<<"copy result of "<<songs[j].originalfile<<" -> "<<newname<<": "<<okf;
        if(!okf){
            QFile f2(newname);
            if( f2.exists() ){
                log->append( songs[j].file() + " was not copied as it already exists in "+p.copyFilesToDir+"\n" );
            }else{
                log->append( songs[j].file() + " could not be copied to "+p.copyFilesToDir+"\n" );
            }
        }else{
            nCopied++;
        }
    }
    pr.setValue(songs.size());
    pr.close();
    log->append(QString::number(nCopied)+" of "+QString::number(songs.size())+" files copied to "+p.copyFilesToDir+"\n");

}

void playlistManager::addPlayList(){

    QString name = "New playlist";

    playList tmp( defaultExtensions );
    tmp.name = name;
    playLists.append(tmp);
    QListWidgetItem *item = new QListWidgetItem();
    item->setText( name );
    item->setFlags(item->flags () | Qt::ItemIsEditable);
    playListTable->addItem( item );
    playListTable->setCurrentRow( playListTable->count()-1 );

    sortPlayLists();

}

void playlistManager::removePlayList(){

    QModelIndexList indexes = playListTable->selectionModel()->selectedIndexes();

    if(indexes.size()==0){
        return;
    }

    qDebug()<<"indexes.size() "<<indexes.size();
    for(int i=indexes.size()-1;i>=0;i--){
        qDebug()<<"playLists.size() "<<playLists.size();
        qDebug()<<"removing playlist "<<playLists[indexes[i].row()].name;
        qDebug()<<"deleting row "<<indexes[i].row();
        qDebug()<<"rowcount "<<playListTable->count();
        playLists.removeAt( indexes[i].row() );
        playListTable->blockSignals(true);
        delete playListTable->takeItem( indexes[i].row() );
        playListTable->blockSignals(false);
        enableOptions( false );
        clearRulesAndFolders();
        if( playListTable->count()>0 ){
            playListTable->setCurrentRow(0);
        }
    }


}

void playlistManager::newRule(){

    int ind = playListTable->currentRow();
    if(ind==-1){
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

    playLists[ind].rules.append( r );
    showRulesAndFolders();
    //rulesTable->addItem( getRuleName( (ruleType)r.type )+": "+r.value );
    /*
	QListWidgetItem *item = new QListWidgetItem();	
	if(!r.shouldBeTrue){
		qDebug()<<"setting font!";
		QFont f = item->font(); f.setItalic(true);
		item->setFont(f);
	}
	item->setText( getRuleName(r.type)+": "+r.value );
	rulesTable->addItem( item );		 	
	//rulesTable->addItem( getRuleName( (ruleType)r.type )+": "+r.value );
	*/

}

void playlistManager::editRule(){

    int pind = playListTable->currentRow();
    int rind = rulesTable->currentRow();
    if(rind==-1){
        QMessageBox::information(this, "",
                                 "Please select a rule first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    rule r = playLists[pind].rules[rind];
    ruleDialog rd( 0, &r );

    if( rd.exec()!=QDialog::Accepted ){
        return;
    }
    rule newr = rd.getSettings();

    playLists[pind].rules[rind] = newr;
    delete rulesTable->takeItem(rind);
    rulesTable->insertItem( rind, Global::getRuleName( (ruleType)newr.type )+": "+newr.value );
    showRulesAndFolders();

}

void playlistManager::removeRule(){

    int pind = playListTable->currentRow();
    int rind = rulesTable->currentRow();
    if(rind==-1){
        QMessageBox::information(this, "",
                                 "Please select a rule first",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    delete rulesTable->takeItem(rind);
    playLists[pind].rules.remove(rind);
}



void playlistManager::showSettings(){

    settingsDialog s;
    guiSettings->sync();

    if( s.exec()==QDialog::Accepted ){
        s.getSettings();

        artistEmpty = guiSettings->value("artistEmpty",true).toBool();
        titleEmpty = guiSettings->value("titleEmpty",true).toBool();
        albumEmpty = guiSettings->value("albumEmpty",false).toBool();
        commentEmpty = guiSettings->value("commentEmpty",false).toBool();
        genreEmpty = guiSettings->value("genreEmpty",false).toBool();
        trackEmpty = guiSettings->value("trackEmpty",false).toBool();
        yearEmpty = guiSettings->value("yearEmpty",false).toBool();
        defaultOutputFolder = guiSettings->value("defaultOutputFolder",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString();
        defaultExtensions = guiSettings->value("defaultExtensions",defaultExt.split(";")).toStringList();
        format = guiSettings->value("format","%artist% - %title%").toString();
        useScript = guiSettings->value("useScript",false).toBool();
        if( useScript ){
            rulesFrame->hide();
            RuleScript->show();
            allRulesTrue->setEnabled(false);
        }else{
            rulesFrame->show();
            RuleScript->hide();
            allRulesTrue->setEnabled(true);
        }
        showLog = guiSettings->value("showLog",true).toBool();
        ShowTagLibDebug = guiSettings->value("ShowTagLibDebug",true).toBool();
        useCopyFilesToPath = guiSettings->value("useCopyFilesToPath",true).toBool();
        keepTags = guiSettings->value("keepTags",true).toBool();
        if(!keepTags){
            tags.clear();
        }
    }



}














