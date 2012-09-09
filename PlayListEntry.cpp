#include "PlayListEntry.h"


/*!
 \brief

*/
PlayListEntry::PlayListEntry(const Tag *tag, const QString &format) {

    if(tag){
        extInf_ = createExtInfString(tag,format);
        file_ = QFileInfo(tag->fileName());
    }else{
        qWarning()<<"PlayListEntry null tag";
    }
}

PlayListEntry::PlayListEntry( const QFileInfo &file ){
    file_=file;
}


/*!
 \brief

 \param e1
 \return bool M3uEntry::operator
*/
bool PlayListEntry::operator==(const PlayListEntry& e1) const {
    return file_ == e1.file();
}

/*!
 \brief

 \return QString
*/
QString PlayListEntry::extInf() const {
    return extInf_;
}


/*!
 \brief

 \return QFileInfo
*/
QFileInfo PlayListEntry::file() const {
    return file_;
}

/*!
 \brief

 \param extInf
*
/
void M3uEntry::setExtInf(const QString& extInf) {
    extInf_ = extInf;
}
*/

/*!
 \brief

 \param originalFile
*
/
void PlayListEntry::setFile(const QFileInfo& originalFile) {
    file_ = originalFile;
}
*/

/*!
 \brief

 \param key
 \return uint
*/
uint qHash(const PlayListEntry& key) {
    //QString tmp = key.file;
    uint val = qHash(key.file().absoluteFilePath());
    return val;
}


/*!
 \brief

 \param tag
 \param file
 \param format_
 \return QString
*/
QString PlayListEntry::createExtInfString(const Tag* tag, const QString& format_) const {

    QString format = format_;
    QString file = tag->fileName();

    bool ok=false;
    if(tag!=0){
        if (tag->tagOk()) {
            ok = true;
            QString artist = tag->artist();
            QString title = tag->title();
            QString album = tag->album();
            QString comment = tag->comment();
            QString genre = tag->genre();
            QString year = QString::number(tag->year());
            QString track = QString::number(tag->track());

            format.replace(QString("%artist%"), artist);
            format.replace(QString("%title%"), title);
            format.replace(QString("%album%"), album);
            format.replace(QString("%comment%"), comment);
            format.replace(QString("%genre%"), genre);
            format.replace(QString("%year%"), year);
            format.replace(QString("%track%"), track);
            format.replace(QString("%filename%"), file);
            /*
        int sum = artistEmpty+titleEmpty+albumEmpty+commentEmpty+genreEmpty+trackEmpty+yearEmpty;
        int sum2 = 0;
        if (artist.isEmpty() && artistEmpty) {
            sum2++;
        }
        if (title.isEmpty() && titleEmpty) {
            sum2++;
        }
        if (album.isEmpty() && albumEmpty) {
            sum2++;
        }
        if (comment.isEmpty() && commentEmpty) {
            sum2++;
        }
        if (genre.isEmpty() && genreEmpty) {
            sum2++;
        }
        if (track.isEmpty() && trackEmpty) {
            sum2++;
        }
        if (year.isEmpty() && yearEmpty) {
            sum2++;
        }
        if (sum2 == sum) {
            format = file;
        }
*/
        }
    }
    if(!ok){
        format = file;
    }


    QString result = QString("#EXTINF:") + QString::number(tag->length()) + "," + format;

    return result;
}


