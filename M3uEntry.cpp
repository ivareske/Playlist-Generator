#include "M3uEntry.h"


/*!
 \brief

*/
M3uEntry::M3uEntry() {


}


/*!
 \brief

 \param e1
 \return bool M3uEntry::operator
*/
bool M3uEntry::operator==(const M3uEntry& e1) const {
    return originalFile_ == e1.originalFile();
}

/*!
 \brief

 \return QString
*/
QString M3uEntry::extInf() const {
    return extInf_;
}


/*!
 \brief

 \return QFileInfo
*/
QFileInfo M3uEntry::originalFile() const {
    return originalFile_;
}

/*!
 \brief

 \param extInf
*/
void M3uEntry::setExtInf(const QString& extInf) {
    extInf_ = extInf;
}


/*!
 \brief

 \param originalFile
*/
void M3uEntry::setOriginalFile(const QFileInfo& originalFile) {
    originalFile_ = originalFile;
}

void M3uEntry::setSortBy(const QString &sortBy){
    sortBy_=sortBy;
}

QString M3uEntry::sortBy() const{
    return sortBy_;
}

/*!
 \brief

 \param key
 \return uint
*/
uint qHash(const M3uEntry& key) {
    //QString tmp = key.file;
    uint val = qHash(key.originalFile().absoluteFilePath());
    return val;
}

bool m3uEntryLessThan(const M3uEntry& s1, const M3uEntry& s2) {
    return Global::naturalCompare(s1.sortBy(), s2.sortBy(), Qt::CaseSensitive) < 0 ;
}
