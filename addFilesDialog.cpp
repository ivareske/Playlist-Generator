#include "AddFilesDialog.h"


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


QList<QFileInfo> AddFilesDialog::getFiles() const {

    QList<QFileInfo> files;
    for (int i = 0; i < fileList->count(); i++) {
        files.append(QFileInfo(fileList->item(i)->text()));
    }
    return files;
}

void AddFilesDialog::removeFiles() {

    qDeleteAll(fileList->selectedItems());

}


void AddFilesDialog::chooseFiles() {


    static QString lastUsedDir;

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


}
