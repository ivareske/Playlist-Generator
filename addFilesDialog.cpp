#include "addFilesDialog.h"

 
addFilesDialog::addFilesDialog(QWidget *parent){
    setupUi(this); // this sets up GUI
 	
    // signals/slots mechanism in action
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) ); 
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) ); 
    connect( addFilesButton, SIGNAL( clicked() ), this, SLOT( chooseFiles() ) ); 
	connect( removeFilesButton, SIGNAL( clicked() ), this, SLOT( removeFiles() ) ); 
	
}



void addFilesDialog::setFiles( QStringList files ){
	fileList->addItems( files );	
}

QStringList addFilesDialog::getFiles() const{
	
	QStringList files;
	for(int i=0;i<fileList->count();i++){
		files.append( fileList->item(i)->text() );
	}
	return files;
}

void addFilesDialog::removeFiles(){			
	
	qDeleteAll(fileList->selectedItems());
/*
	QModelIndexList indexes = fileList->selectionModel()->selectedIndexes();	
	if(indexes.size()==0){		
		return;
	}
	qDebug()<<indexes.size();
	for(int i=indexes.size()-1;i>=0;i--){
		qDebug()<<"deleting at row "<<i;
		delete fileList->takeItem( indexes[i].row() );		
	}	
	*/
}


void addFilesDialog::chooseFiles(){
	
	
	static QString lastUsedDir;	
	
	QStringList files = QFileDialog::getOpenFileNames(
		this,
		"Select files",
		lastUsedDir,
		"*.*");
	
	if(files.size()==0){
		return;
	}
	
	for(int i=0;i<files.size();i++){
		QFileInfo f = QFileInfo(files[i]);
		files[i] = f.fileName();
	}
	
	fileList->addItems( files );

}
