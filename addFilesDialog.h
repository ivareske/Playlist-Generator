#ifndef ADDFILESDIALOG_H
#define ADDFILESDIALOG_H
 
#include <QtGui>
#include "settingsClass.h"
#include "ui_addFilesDialog.h"
 
 
class addFilesDialog : public QDialog, private Ui::addFilesDialog
{
    Q_OBJECT
 
public:

    addFilesDialog( const QList<QFileInfo> &files,  QWidget *parent = 0 );
    QList<QFileInfo> getFiles() const;
	
private slots:

	void chooseFiles();
	void removeFiles();

	
	
};
 
 
#endif






