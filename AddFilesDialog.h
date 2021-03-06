#ifndef ADDFILESDIALOG_H
#define ADDFILESDIALOG_H

#include <QtGui>
#include "GlobalFunctions.h"
#include "ui_AddFilesDialog.h"


class AddFilesDialog : public QDialog, private Ui::AddFilesDialog {
        Q_OBJECT

    public:

        AddFilesDialog(const QList<QFileInfo> &files = QList<QFileInfo>(),  QWidget* parent = 0);
        QList<QFileInfo> getFiles() const;

    private slots:

        void chooseFiles();
        void removeFiles();



};
 
 
#endif






