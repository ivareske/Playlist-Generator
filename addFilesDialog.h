#ifndef ADDFILESDIALOG_H
#define ADDFILESDIALOG_H
 
#include <QtGui>
#include "SettingsClass.h"
#include "ui_addFilesDialog.h"
 
 
class AddFilesDialog : public QDialog, private Ui::AddFilesDialog
{
    Q_OBJECT
 
public:

    AddFilesDialog( const QList<QFileInfo> &files,  QWidget *parent = 0 );
    QList<QFileInfo> getFiles() const;
	
private slots:

	void chooseFiles();
	void removeFiles();

	
	
};
 
 
#endif






