#include "PlayListCollection.h"

/*!
 \brief

 \param name
*/
PlayListCollection::PlayListCollection(const QString& name) {

    name_ = name;
    if (name_.isEmpty()) {
        name_ = defaultCollectionName();
    }

}

/*!
 \brief

 \param name
*/
void PlayListCollection::setName(const QString& name) {
    name_ = name;
}

/*!
 \brief

 \return QString
*/
QString PlayListCollection::name() const {
    return name_;
}

/*!
 \brief

 \param playLists
*/
void PlayListCollection::setPlayLists(const QList<PlayList> &playLists) {
    playLists_ = playLists;
}

/*!
 \brief

 \return QList<PlayList>
*/
QList<PlayList> PlayListCollection::playLists() const {
    return playLists_;
}

/*!
 \brief

 \return QDir
*/
QDir PlayListCollection::outPutPath() const{
    return outPutPath_;
}

/*!
 \brief

 \param outPutPath
*/
void PlayListCollection::setOutPutPath( const QDir &outPutPath ){
    outPutPath_ = outPutPath;
}

/*!
 \brief

 \param other
 \return bool PlayListCollection::operator
*/
bool PlayListCollection::operator ==(const PlayListCollection& other) const {
    bool res = name_ == other.name();
    res &= playLists_ == other.playLists();
    res &= outPutPath_ == other.outPutPath();
    return res;
}

/*!
 \brief

 \return PlayListCollection::operator
*/
PlayListCollection::operator QVariant() const {
    return QVariant::fromValue(*this);
}

/*!
 \brief

 \param in
 \param p
 \return QDataStream & operator >>
*/
QDataStream& operator>>(QDataStream& in, PlayListCollection& p) {

    bool ok;
    QString log;
    QString version = Global::versionCheck(&in, &ok, &log);
    if (!ok) {
        QMessageBox::critical(0, "", log);
    }

    if (version != "1.0") {
        in.setStatus(QDataStream::ReadCorruptData);
        return in;
    }

    QString name;
    QList<PlayList> playLists;
    QString outPutPath;
    in >> name >> playLists >> outPutPath ;
    p = PlayListCollection(name);
    p.setPlayLists(playLists);
    p.setOutPutPath(QDir(outPutPath));
    return in;
}

/*!
 \brief

 \param out
 \param p
 \return QDataStream & operator
*/
QDataStream& operator<<(QDataStream& out, const PlayListCollection& p) {

    out << out.version();
    out << qApp->applicationVersion();

    out << p.name() << p.playLists() << p.outPutPath().absolutePath();
    return out;
}

/*!
 \brief

 \return QString
*/
QString PlayListCollection::defaultCollectionName() {
    return QDesktopServices::storageLocation(QDesktopServices::MusicLocation) + "/New collection" + Global::ext;
}
