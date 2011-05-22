#include "SettingsClass.h"


SettingsClass::SettingsClass(){


    style_="Plastique";
    artistEmpty_=true;
    titleEmpty_=true;
    albumEmpty_=false;
    commentEmpty_=false;
    genreEmpty_=false;
    trackEmpty_=false;
    yearEmpty_=false;
    defaultOutputFolder_= QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
    defaultExtensions_ << "*.mp3"<<"*.wma"<<"*.wav"<<"*.ogg"<<"*.aac"<<"*.ac3";
    format_ = "%artist% - %title%";
    useScript_ = false;
    showLog_ = true;
    useCopyFilesToPath_ = false;
    keepTags_ = true;
    copyFilesToDir_="";
    textViewerSize_ = QSize(200,400);

}



QString SettingsClass::style() const{
    return style_;
}

bool SettingsClass::artistEmpty() const{
    return artistEmpty_;
}

bool SettingsClass::titleEmpty() const{
    return titleEmpty_;
}

bool SettingsClass::albumEmpty() const{
    return albumEmpty_;
}

bool SettingsClass::commentEmpty() const{
    return commentEmpty_;
}

bool SettingsClass::genreEmpty() const{
    return genreEmpty_;
}

bool SettingsClass::trackEmpty() const{
    return trackEmpty_;
}

bool SettingsClass::yearEmpty() const{
    return yearEmpty_;
}

QString SettingsClass::defaultOutputFolder() const{
    return defaultOutputFolder_;
}

QStringList SettingsClass::defaultExtensions() const{
    return defaultExtensions_;
}

QString SettingsClass::format() const{
    return format_;
}

bool SettingsClass::useScript() const{
    return useScript_;
}

bool SettingsClass::showLog() const{
    return showLog_;
}

bool SettingsClass::useCopyFilesToPath() const{
    return useCopyFilesToPath_;
}

bool SettingsClass::keepTags() const{
    return keepTags_;
}

QString SettingsClass::copyFilesToDir() const{
    return copyFilesToDir_;
}

QSize SettingsClass::textViewerSize() const{
    return textViewerSize_;
}


void SettingsClass::setArtistEmpty( bool artistEmpty ){
    artistEmpty_ = artistEmpty;
}

void SettingsClass::setTitleEmpty( bool titleEmpty ){
    titleEmpty_ = titleEmpty;
}

void SettingsClass::setAlbumEmpty( bool albumEmpty ){
    albumEmpty_ = albumEmpty;
}

void SettingsClass::setCommentEmpty( bool commentEmpty ){
    commentEmpty_ = commentEmpty;
}

void SettingsClass::setGenreEmpty( bool genreEmpty ){
    genreEmpty_ = genreEmpty;
}

void SettingsClass::setTrackEmpty( bool trackEmpty ){
    trackEmpty_ = trackEmpty;
}

void SettingsClass::setYearEmpty( bool yearEmpty ){
    yearEmpty_ = yearEmpty;
}

void SettingsClass::setUseScript( bool useScript ){
    useScript_ = useScript;
}

void SettingsClass::setShowLog( bool showLog ){
    showLog_ = showLog;
}

void SettingsClass::setUseCopyFilesToPath( bool useCopyFilesToPath ){
    useCopyFilesToPath_ = useCopyFilesToPath;
}

void SettingsClass::setKeepTags( bool keepTags ){
    keepTags_ = keepTags;
}

void SettingsClass::setDefaultOutputFolder( const QString &defaultOutputFolder ){
    defaultOutputFolder_ = defaultOutputFolder;
}

void SettingsClass::setDefaultExtensions( const QStringList &defaultExtensions ){
    defaultExtensions_ = defaultExtensions;
}

void SettingsClass::setFormat( const QString &format ){
    format_ = format;
}

void SettingsClass::setStyle( const QString &style ){
    style_ = style;
}

void SettingsClass::setCopyFilesToDir( const QString &copyFilesToDir ){
    copyFilesToDir_ = copyFilesToDir;
}

void SettingsClass::setTextViewerSize( const QSize &textViewerSize ){
    textViewerSize_ = textViewerSize;
}


SettingsClass::operator QVariant () const{
    return QVariant::fromValue(*this);
}


QDataStream &operator>>(QDataStream &in, SettingsClass &s){
    QString style;
    bool artistEmpty;
    bool titleEmpty;
    bool albumEmpty;
    bool commentEmpty;
    bool genreEmpty;
    bool trackEmpty;
    bool yearEmpty;
    QString defaultOutputFolder;
    QStringList defaultExtensions;
    QString format;
    bool useScript;
    bool showLog;
    bool useCopyFilesToPath;
    bool keepTags;
    QString copyFilesToDir;
    QSize textViewerSize;
    in >> style >> artistEmpty >> titleEmpty >> albumEmpty >> commentEmpty >> genreEmpty >> trackEmpty;
    in >> yearEmpty >> defaultOutputFolder >> defaultExtensions >> format >> useScript >> showLog >> useCopyFilesToPath;
    in >> keepTags >> copyFilesToDir >> textViewerSize;
    s = SettingsClass();
    s.setStyle(style); s.setArtistEmpty(artistEmpty); s.setTitleEmpty(titleEmpty); s.setAlbumEmpty(albumEmpty);
    s.setCommentEmpty(commentEmpty); s.setGenreEmpty(genreEmpty); s.setTrackEmpty(trackEmpty); s.setYearEmpty(yearEmpty);
    s.setDefaultOutputFolder(defaultOutputFolder); s.setDefaultExtensions(defaultExtensions); s.setFormat(format);
    s.setUseScript(useScript); s.setShowLog(showLog); s.setUseCopyFilesToPath(useCopyFilesToPath); s.setKeepTags(keepTags);
    s.setCopyFilesToDir(copyFilesToDir); s.setTextViewerSize(textViewerSize);
    return in;
}

QDataStream &operator<<(QDataStream &out, const SettingsClass &s){
    out << s.style() << s.artistEmpty() << s.titleEmpty() << s.albumEmpty() << s.commentEmpty() << s.genreEmpty();
    out << s.trackEmpty() << s.yearEmpty() << s.defaultOutputFolder() << s.defaultExtensions() << s.format();
    out << s.useScript() << s.showLog() << s.useCopyFilesToPath() << s.keepTags() << s.copyFilesToDir() << s.textViewerSize();
    return out;
}





