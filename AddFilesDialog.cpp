#include "AddFilesDialog.h"


/*!
 \brief

 \param files
 \param parent
*/
AddFilesDialog::AddFilesDialog(const QList<QFileInfo> &files, QWidget* parent) : QDialog(parent) {
    setupUi(this); // this sets up GUI

    // signals/slots mechanism in action
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(addFilesButton, SIGNAL(clicked()), this, SLOT(chooseFiles()));
    connect(removeFilesButton, SIGNAL(clicked()), this, SLOT(removeFiles()));

    for (int i = 0; i < files.size(); i++) {
        fileList->addItem(files[i].absoluteFilePath());
    }

}


/*!
 \brief

 \return QList<QFileInfo>
*/
QList<QFileInfo> AddFilesDialog::getFiles() const {

    QList<QFileInfo> files;
    for (int i = 0; i < fileList->count(); i++) {
        files.append(QFileInfo(fileList->item(i)->text()));
    }
    return files;
}

/*!
 \brief

*/
void AddFilesDialog::removeFiles() {

    qDeleteAll(fileList->selectedItems());

}


/*!
 \brief

*/
void AddFilesDialog::chooseFiles() {

    PMSettings *settings = Global::guiSettings();
    QString lastUsedDir = settings->value("AddFilesDialog/lastUsedDir",QDesktopServices::MusicLocation).toString();    
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Select files",
                            lastUsedDir,
                            "*.*");    

    if (files.size() == 0) {
        return;
    }

    for (int i = 0; i < files.size(); i++) {
        //QFileInfo f = QFileInfo(files[i]);
        //files[i] = f.fileName();
        fileList->addItem(files[i]);
    }

    QFileInfo f(files[0]);
    settings->setValue("AddFilesDialog/lastUsedDir",f.absolutePath());
    delete settings;settings=0;

}
