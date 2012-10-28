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

    QPair<QString,QString> p("New script","");
    scripts_.append(p);

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

QString PlayListCollection::commonScript() const{
    return commonScript_;
}
void PlayListCollection::setCommonScript( const QString &commonScript ){
    commonScript_=commonScript;
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
    res &= this->sortedScripts() == other.sortedScripts();
    res &= commonScript_ == other.commonScript();
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
    QString outPutPath,commonScript;
    QList< QPair<QString,QString> > scripts;
    in >> name >> playLists >> outPutPath >> scripts >> commonScript;
    p = PlayListCollection(name);
    p.setPlayLists(playLists);
    p.setOutPutPath(QDir(outPutPath));
    p.setScripts(scripts);
    p.setCommonScript(commonScript);
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

    out << p.name() << p.playLists() << p.outPutPath().absolutePath() << p.scripts()<<p.commonScript();
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

QList<QPair<QString, QString> > PlayListCollection::sortedScripts() const{
    QList<QPair<QString, QString> > tmpScripts=scripts_;
    qSort(tmpScripts.begin(),tmpScripts.end(),Global::QPairFirstComparer());
    return tmpScripts;
}


