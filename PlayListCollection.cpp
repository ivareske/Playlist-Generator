#include "PlayListCollection.h"

PlayListCollection::PlayListCollection(const QString& name) {

    name_ = name;
    if (name_.isEmpty()) {
        name_ = defaultCollectionName();
    }

}

void PlayListCollection::setName(const QString& name) {
    name_ = name;
}

QString PlayListCollection::name() const {
    return name_;
}

void PlayListCollection::setPlayLists(const QList<PlayList> &playLists) {
    playLists_ = playLists;
}

QList<PlayList> PlayListCollection::playLists() const {
    return playLists_;
}

bool PlayListCollection::operator ==(const PlayListCollection& other) const {
    bool res = name_ == other.name();
    res &= playLists_ == other.playLists();
    return res;
}

PlayListCollection::operator QVariant() const {
    return QVariant::fromValue(*this);
}

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
    in >> name >> playLists;
    p = PlayListCollection(name);
    p.setPlayLists(playLists);
    return in;
}

QDataStream& operator<<(QDataStream& out, const PlayListCollection& p) {

    out << out.version();
    out << qApp->applicationVersion();

    out << p.name() << p.playLists();
    return out;
}

QString PlayListCollection::defaultCollectionName() {
    return QDesktopServices::storageLocation(QDesktopServices::MusicLocation) + "/New collection" + Global::ext;
}
