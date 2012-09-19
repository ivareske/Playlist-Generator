#include "SettingsDialog.h"



/*!
 \brief

 \param parent
*/
SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setupUi(this); // this sets up GUI

    #ifndef HAVE_TAGLIB_APEFILE_H
        apeItemKeysButton->hide();
    #endif

    settings = Global::guiSettings();    

    // signals/slots mechanism in action

    connect(ID3v2FramesButton, SIGNAL(clicked()), this, SLOT(editFrameFields()));
    connect(apeItemKeysButton, SIGNAL(clicked()), this, SLOT(editFrameFields()));
    connect(mp4KeysButton, SIGNAL(clicked()), this, SLOT(editFrameFields()));
    connect(asfKeysButton, SIGNAL(clicked()), this, SLOT(editFrameFields()));
    connect(xiphKeysButton, SIGNAL(clicked()), this, SLOT(editFrameFields()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(fileDialog, SIGNAL(clicked()), this, SLOT(setOutPutDir()));
    connect(this, SIGNAL(finished(int)), this, SLOT(onFinish(int)));

    setSettings();
    //UseScript->setChecked(false);
    //UseScript->hide();
}

/*!
  \brief
  \param type
*/
void SettingsDialog::editFrameFields(){

    QObject* s = QObject::sender();;
    if( s==0 ){
        qDebug()<<"SettingsDialog::editFrameFields, ERROR, NULL POINTER SENDER!";
        return;
    }
    QString type;
    if(s==ID3v2FramesButton){
        type="ID3V2";
    }else if(s==apeItemKeysButton){
        type="APE";
    }else if(s==mp4KeysButton){
        type="MP4";
    }else if(s==asfKeysButton){
        type="ASF";
    }else if(s==xiphKeysButton){
        type="XIPH";
    }else{
        qDebug()<<"SettingsDialog::editFrameFields, UNKNOWN SENDER!";
        return;
    }

    TextViewer t;
    t.setLabelText("Warning: Do not edit these values unless you know what you are doing!");
    QHash<QString,QVariant> frameFields = settings->value("frameFields").toHash();
    t.setText( frameFields[type].toStringList().join("\n").toUpper() );
    t.setReadOnly(false);
    if(t.exec()!=QDialog::Accepted){
        return;
    }
    QStringList list = t.text().split("\n");
    frameFields[type] = list;
    settings->setValue("frameFields",QVariant::fromValue(frameFields));
}

/*!
 \brief

*/
SettingsDialog::~SettingsDialog() {
    if (settings != 0) {
        delete settings;
        settings = 0;
    }
}

/*!
 \brief

 \param result
*/
void SettingsDialog::onFinish(int result) {

    if (result == QDialog::Accepted) {
        QString out = outPutPath->text();
        QString last = out.right(1);
        if (last != "\\" && last != "/") {
            out.append("/");
        }

        settings->setValue("outPutPath", out);
        settings->setValue("defaultExtensions", DefaultExtensions->text().split(";"));
        //settings->setValue("useScript", UseScript->isChecked());        
        settings->setValue("format", Format->text());
        settings->setValue("showLog", ShowLog->isChecked());
        settings->setValue("artistEmpty", Artist->isChecked());
        settings->setValue("titleEmpty", Title->isChecked());
        settings->setValue("albumEmpty", Album->isChecked());
        settings->setValue("commentEmpty", Comment->isChecked());
        settings->setValue("genreEmpty", Genre->isChecked());
        settings->setValue("trackEmpty", Track->isChecked());
        settings->setValue("yearEmpty", Year->isChecked());
        settings->setValue("useCopyFilesToPath", useCopyFilesToPathCheckBox->isChecked());
        settings->setValue("keepFolderStructure", keepFolderStructureCheckBox->isChecked());
        settings->setValue("overWriteFiles", overWriteFilesCheckBox->isChecked());
        settings->setValue("keepTags", keepTagsCheckBox->isChecked());
        settings->setValue("ShowTagLibDebug", ShowTagLibDebug->isChecked());
        settings->setValue("debugScriptCheckBox", debugScript->isChecked());
        settings->sync();
    }


}


/*!
 \brief

*/
void SettingsDialog::setSettings() {

    outPutPath->setText(settings->value("outPutPath").toString());
    DefaultExtensions->setText(settings->value("defaultExtensions").toStringList().join(";"));
    Format->setText(settings->value("format").toString());
    //UseScript->setChecked(settings->value("useScript").toBool());
    ShowLog->setChecked(settings->value("showLog").toBool());
    Artist->setChecked(settings->value("artistEmpty").toBool());
    Title->setChecked(settings->value("titleEmpty").toBool());
    Album->setChecked(settings->value("albumEmpty").toBool());
    Comment->setChecked(settings->value("commentEmpty").toBool());
    Genre->setChecked(settings->value("genreEmpty").toBool());
    Track->setChecked(settings->value("trackEmpty").toBool());
    Year->setChecked(settings->value("yearEmpty").toBool());
    useCopyFilesToPathCheckBox->setChecked(settings->value("useCopyFilesToPath").toBool());
    keepFolderStructureCheckBox->setChecked(settings->value("keepFolderStructure").toBool());
    overWriteFilesCheckBox->setChecked(settings->value("overWriteFiles").toBool());
    keepTagsCheckBox->setChecked(settings->value("keepTags").toBool());
    ShowTagLibDebug->setChecked(settings->value("ShowTagLibDebug").toBool());
    debugScriptCheckBox->setChecked(settings->value("debugScript").toBool());


}



/*!
 \brief

 \return QString
*/
QString SettingsDialog::chooseDir() {

    QString dir = outPutPath->text();
    if (dir.isEmpty()) {
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setDirectory(dir);
    if (dialog.exec()) {
        QStringList d = dialog.selectedFiles();
        dir = d[0];
        return dir;
    }
    else {
        return QString("");
    }

}

/*!
 \brief

*/
void SettingsDialog::setOutPutDir() {

    QString dir = chooseDir();
    if (!dir.isEmpty()) {
        outPutPath->setText(dir);
    }

}
