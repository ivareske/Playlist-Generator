#include "M3uEntry.h"


M3uEntry::M3uEntry(){
    
        file_="";
        fullFile_="";
        originalFile_="";
        extInf_="";
}


bool M3uEntry::operator==(const M3uEntry &e1) const{
    return file_ == e1.file();
}

QString M3uEntry::extInf() const{
    return extInf_;
}

QString M3uEntry::file() const{
    return file_;
}

QString M3uEntry::fullFile() const{
    return fullFile_;
}

QString M3uEntry::originalFile() const{
    return originalFile_;
}

void M3uEntry::setExtInf( const QString &extInf ){
    extInf_ = extInf;
}

void M3uEntry::setFile( const QString &file ){
    file_ = file;
}

void M3uEntry::setFullFile( const QString &fullFile ){
    fullFile_ = fullFile;
}

void M3uEntry::setOriginalFile( const QString &originalFile ){
    originalFile_ = originalFile;
}

/*
uint M3uEntry::qHash( const M3uEntry &key ){
	//QString tmp = key.file;
	uint val = qHash( file.unicode() );
	return 1;
	const QChar *ptr = key.file.unicode();
    int n = key.file.size();
    uint h = 0;
    uint g;
    while (n--) {
        h = (h << 4) + (ptr++)->unicode();
        if ((g = (h & 0xf0000000)) != 0)
            h ^= g >> 23;
        h &= ~g;
    }
    return h;

}
*/
