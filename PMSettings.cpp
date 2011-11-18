#include "PMSettings.h"


/*!
 \brief
 \param fileName
 \param format
 \param parent
*/
PMSettings::PMSettings(const QString & fileName, Format format, QObject * parent ) : QSettings(fileName,format,parent){
    checkForEmptyValue_=true;
}

/*!
 \brief
 \param check
*/
void PMSettings::setCheckForEmptyValue(bool check){
    checkForEmptyValue_=check;
}

/*!
 \brief
 \return bool checkForEmptyValue_
*/
bool PMSettings::checkForEmptyValue() const{
    return checkForEmptyValue_;
}

/*!
 \brief If requested value and default value is empty and checkForEmptyValue_ is true, ASSERT.
 \param key
 \param defaultValue
 \return QVariant val
*/
QVariant PMSettings::value( const QString & key, const QVariant & defaultValue ) const{
    QVariant val = QSettings::value(key,defaultValue);
    Q_ASSERT(!(checkForEmptyValue_ && val==defaultValue && defaultValue==QVariant() ));
    return val;
}

