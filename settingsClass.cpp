#include "settingsClass.h"


settingsClass::settingsClass(){


    style_="";
    artistEmpty_=true;
    titleEmpty_=true;
    albumEmpty_=false;
    commentEmpty_=false;
    genreEmpty_=false;
    trackEmpty_=false;
    yearEmpty_=false;
    defaultOutputFolder_= QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
    defaultExtensions_ = "*.mp3;*.wma;*.wav;*.ogg;*.aac;*.ac3";
    format_ = "%artist% - %title%";
    useScript_ = false;
    showLog_ = true;
    useCopyFilesToPath_ = false;
    keepTags_ = true;
    copyFilesToDir_="";
    textViewerSize_ = QSize(200,400);

}



QString settingsClass::style() const{
    return style_;
}

bool settingsClass::artistEmpty() const{
    return artistEmpty_;
}

bool settingsClass::titleEmpty() const{
    return titleEmpty_;
}

bool settingsClass::albumEmpty() const{
    return albumEmpty_;
}

bool settingsClass::commentEmpty() const{
    return commentEmpty_;
}

bool settingsClass::genreEmpty() const{
    return genreEmpty_;
}

bool settingsClass::trackEmpty() const{
    return trackEmpty_;
}

bool settingsClass::yearEmpty() const{
    return yearEmpty_;
}

QString settingsClass::defaultOutputFolder() const{
    return defaultOutputFolder_;
}

QStringList settingsClass::defaultExtensions() const{
    return defaultExtensions_;
}

QString settingsClass::format() const{
    return format_;
}

bool settingsClass::useScript() const{
    return useScript_;
}

bool settingsClass::showLog() const{
    return showLog_;
}

bool settingsClass::useCopyFilesToPath() const{
    return useCopyFilesToPath_;
}

bool settingsClass::keepTags() const{
    return keepTags_;
}

QString settingsClass::copyFilesToDir() const{
    return copyFilesToDir_;
}

QSize settingsClass::textViewerSize() const{
    return textViewerSize_;
}


void settingsClass::setArtistEmpty( bool artistEmpty ){
    artistEmpty_ = artistEmpty;
}

void settingsClass::setTitleEmpty( bool titleEmpty ){
    titleEmpty_ = titleEmpty;
}

void settingsClass::setAlbumEmpty( bool albumEmpty ){
    albumEmpty_ = albumEmpty;
}

void settingsClass::setCommentEmpty( bool commentEmpty ){
    commentEmpty_ = commentEmpty;
}

void settingsClass::setGenreEmpty( bool genreEmpty ){
    genreEmpty_ = genreEmpty;
}

void settingsClass::setTrackEmpty( bool trackEmpty ){
    trackEmpty_ = trackEmpty;
}

void settingsClass::setYearEmpty( bool yearEmpty ){
    yearEmpty_ = yearEmpty;
}

void settingsClass::setUseScript( bool useScript ){
    useScript_ = useScript;
}

void settingsClass::setShowLog( bool showLog ){
    showLog_ = showLog;
}

bool settingsClass::setUseCopyFilesToPath( bool useCopyFilesToPath ){
    useCopyFilesToPath_ = useCopyFilesToPath;
}

bool settingsClass::setKeepTags( bool keepTags ){
    keepTags_ = keepTags;
}

void settingsClass::setDefaultOutputFolder( const QString &defaultOutputFolder ){
    defaultOutputFolder_ = defaultOutputFolder;
}

void settingsClass::setDefaultExtensions( const QStringList &defaultExtensions ){
    defaultExtensions_ = defaultExtensions;
}

void settingsClass::setFormat( const QString &format ){
    format_ = format;
}

void settingsClass::setStyle( const QString &style ){
    style_ = style;
}

void settingsClass::setCopyFilesToDir( const QString &copyFilesToDir ){
    copyFilesToDir_ = copyFilesToDir;
}

void settingsClass::setTextViewerSize( const QSize &textViewerSize ){
    textViewerSize_ = textViewerSize;
}





QDataStream &operator>>(QDataStream &in, settingsClass &s);
QDataStream &operator<<(QDataStream &out, const settingsClass &s);






