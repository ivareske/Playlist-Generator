#include "settingsDialog.h"



settingsDialog::settingsDialog( settingsClass *settings, QWidget *parent) : QDialog(parent){
    setupUi(this); // this sets up GUI

    settings_ = settings;

    ShowTagLibDebug->hide(); // not implemented anymore

    // signals/slots mechanism in action
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) ); 
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) ); 
    connect( fileDialog, SIGNAL( clicked() ), this, SLOT( setOutPutDir() ) ); 
    connect( this, SIGNAL(finished(int)), this, SLOT(onFinish(int)) );

    setSettings();
    UseScript->setChecked(false);
    UseScript->hide();
}

void settingsDialog::onFinish( int result ){

    if(result==QDialog::Accepted){
        QString out = outputPath->text();
        QString last = out.right(1);
        if( last!="\\" && last!="/"){
            out.append("/");
        }
        settings_->setDefaultOutputFolder(out);
        settings_->setDefaultExtensions(DefaultExtensions->text().split(";"));
        settings_->setUseScript( UseScript->isChecked() );
        settings_->setFormat( Format->text() );
        settings_->setShowLog( ShowLog->isChecked() );
        settings_->setArtistEmpty( Artist->isChecked() );
        settings_->setTitleEmpty( Title->isChecked() );
        settings_->setAlbumEmpty( Album->isChecked() );
        settings_->setCommentEmpty( Comment->isChecked() );
        settings_->setGenreEmpty( Genre->isChecked() );
        settings_->setTrackEmpty( Track->isChecked() );
        settings_->setYearEmpty( Year->isChecked() );
        settings_->setUseCopyFilesToPath( useCopyFilesToPathCheckBox->isChecked() );
        settings_->setKeepTags( keepTagsCheckBox->isChecked() );
    }
}


void settingsDialog::setSettings(){

    DefaultExtensions->setText( settings_->defaultExtensions().join(";") );
    Format->setText( settings_->format() );
    UseScript->setChecked( settings_->useScript() );
    ShowLog->setChecked( settings_->showLog() );
    Artist->setChecked( settings_->artistEmpty() );
    Title->setChecked( settings_->titleEmpty() );
    Album->setChecked( settings_->albumEmpty() );
    Comment->setChecked( settings_->commentEmpty() );
    Genre->setChecked( settings_->genreEmpty() );
    Track->setChecked( settings_->trackEmpty() );
    Year->setChecked( settings_->yearEmpty() );
    useCopyFilesToPathCheckBox->setChecked( settings_->useCopyFilesToPath() );
    keepTagsCheckBox->setChecked( settings_->keepTags() );

    /*
    settings = new QSettings( "playListGenerator"+ext, QSettings::IniFormat, this );

    outputPath->setText( settings->value("defaultOutputFolder",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString() );
    DefaultExtensions->setText( settings->value("defaultExtensions",defaultExt.split(";")).toStringList().join(";") );
    Format->setText( settings->value("format","%artist% - %title%").toString() );
    UseScript->setChecked( settings->value("useScript",false).toBool() );
    ShowLog->setChecked( settings->value("showLog",true).toBool() );
    ShowTagLibDebug->setChecked( settings->value("ShowTagLibDebug",true).toBool() );
    Artist->setChecked( settings->value("artistEmpty",true).toBool() );
    Title->setChecked( settings->value("titleEmpty",true).toBool() );
    Album->setChecked( settings->value("albumEmpty",false).toBool() );
    Comment->setChecked( settings->value("commentEmpty",false).toBool() );
    Genre->setChecked( settings->value("genreEmpty",false).toBool() );
    Track->setChecked( settings->value("trackEmpty",false).toBool() );
    Year->setChecked( settings->value("yearEmpty",false).toBool() );
    useCopyFilesToPathCheckBox->setChecked( settings->value("useCopyFilesToPath",true).toBool() );
    keepTagsCheckBox->setChecked( settings->value("keepTags",true).toBool() );
    settings->sync();
    */
}

/*
void settingsDialog::getSettings(){

    settings = new QSettings( "playListGenerator"+ext, QSettings::IniFormat, this );
    QString out = outputPath->text();
    QString last = out.right(1);
    if( last!="\\" && last!="/"){
        out.append("/");
    }
    settings->setValue( "defaultOutputFolder", out );
    settings->setValue( "defaultExtensions", DefaultExtensions->text().split(";") );
    settings->setValue( "useScript", UseScript->isChecked() );
    settings->setValue( "format", Format->text() );
    settings->setValue( "showLog", ShowLog->isChecked() );
    settings->setValue( "ShowTagLibDebug", ShowTagLibDebug->isChecked() );
    settings->setValue( "artistEmpty", Artist->isChecked() );
    settings->setValue( "titleEmpty", Title->isChecked() );
    settings->setValue( "albumEmpty", Album->isChecked() );
    settings->setValue( "commentEmpty", Comment->isChecked() );
    settings->setValue( "genreEmpty", Genre->isChecked() );
    settings->setValue( "trackEmpty", Track->isChecked() );
    settings->setValue( "yearEmpty", Year->isChecked() );
    settings->setValue( "useCopyFilesToPath", useCopyFilesToPathCheckBox->isChecked() );
    settings->setValue( "keepTags", keepTagsCheckBox->isChecked() );
    settings->sync();
}
*/

QString settingsDialog::chooseDir(){

    QString dir = outputPath->text();
    if(dir.isEmpty()){
        dir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    }
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setDirectory( dir );
    if( dialog.exec() ){
        QStringList d = dialog.selectedFiles();
        dir = d[0];
        return dir;
    }else{
        return QString("");
    }

}

void settingsDialog::setOutPutDir(){

    QString dir = chooseDir();
    if(!dir.isEmpty()){
        outputPath->setText( dir );
    }

}
