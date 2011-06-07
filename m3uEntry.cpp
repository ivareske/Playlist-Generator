#include "M3uEntry.h"


M3uEntry::M3uEntry(){
    

}


bool M3uEntry::operator==(const M3uEntry &e1) const{
    return originalFile_ == e1.originalFile();
}

QString M3uEntry::extInf() const{
    return extInf_;
}


QFileInfo M3uEntry::originalFile() const{
    return originalFile_;
}

void M3uEntry::setExtInf( const QString &extInf ){
    extInf_ = extInf;
}


void M3uEntry::setOriginalFile( const QFileInfo &originalFile ){
    originalFile_ = originalFile;
}

uint qHash( const M3uEntry &key ){
    //QString tmp = key.file;
    uint val = qHash( key.originalFile().absoluteFilePath() );
    return val;
}
