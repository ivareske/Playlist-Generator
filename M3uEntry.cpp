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
