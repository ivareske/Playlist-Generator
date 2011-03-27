#ifndef ADDFILESDIALOG_H
#define ADDFILESDIALOG_H
 
#include <QtGui>
#include "settingsClass.h"
#include "ui_addFilesDialog.h"
 
 
class addFilesDialog : public QDialog, private Ui::addFilesDialog
{
    Q_OBJECT
 
public:

    addFilesDialog(QWidget *parent = 0);
	
	
public slots:

	void setFiles( QStringList files );
        QStringList getFiles() const;
	void chooseFiles();
	void removeFiles();
protected:
	
	
};
 
 
#endif






