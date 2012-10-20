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
    res &= scripts_ == other.scripts();
    return res;
}

bool PlayListCollection::operator !=(const PlayListCollection& other) const {
    return !this->operator ==(other);
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
    QList< QPair<QString,QString> > scripts;
    in >> name >> playLists >> outPutPath >> scripts;
    p = PlayListCollection(name);
    p.setPlayLists(playLists);
    p.setOutPutPath(QDir(outPutPath));
    p.setScripts(scripts);
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

    out << p.name() << p.playLists() << p.outPutPath().absolutePath() << p.scripts();
    return out;
}

/*!
 \brief

 \return QString
*/
QString PlayListCollection::defaultCollectionName() {
    return QDesktopServices::storageLocation(QDesktopServices::MusicLocation) + "/New collection" + Global::ext;
}

/*!
 \brief

 \param script
*/
void PlayListCollection::setScripts(const QList< QPair<QString,QString> > &scripts){
    scripts_=scripts;
}

/*!
 \brief

 return QHash<QString,QString>
*/
QList<QPair<QString, QString> > PlayListCollection::scripts() const{
    return scripts_;
}
