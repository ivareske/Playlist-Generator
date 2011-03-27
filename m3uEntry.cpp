#include "m3uEntry.h"


m3uEntry::m3uEntry(){
    
        file_="";
        fullFile_="";
        originalFile_="";
        extInf_="";
}


bool m3uEntry::operator==(const m3uEntry &e1) const{
    return file_ == e1.file();
}

QString m3uEntry::extInf() const{
    return extInf_;
}

QString m3uEntry::file() const{
    return file_;
}

QString m3uEntry::fullFile() const{
    return fullFile_;
}

QString m3uEntry::originalFile() const{
    return originalFile_;
}

void m3uEntry::setExtInf( const QString &extInf ){
    extInf_ = extInf;
}

void m3uEntry::setFile( const QString &file ){
    file_ = file;
}

void m3uEntry::setFullFile( const QString &fullFile ){
    fullFile_ = fullFile;
}

void m3uEntry::setOriginalFile( const QString &originalFile ){
    originalFile_ = originalFile;
}

/*
uint m3uEntry::qHash( const m3uEntry &key ){
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
