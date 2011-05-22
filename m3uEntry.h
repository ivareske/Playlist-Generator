#ifndef M3UENTRY_H
#define M3UENTRY_H

#include <QtGui>
#include <QHash>

class M3uEntry{
	
	public:
	M3uEntry();

        QString extInf() const;
        QString file() const;
        QString fullFile() const;
        QString originalFile() const;
        void setExtInf( const QString &extInf );
        void setFile( const QString &file );
        void setFullFile( const QString &fullFile );
        void setOriginalFile( const QString &originalFile );

	bool operator==(const M3uEntry &e1) const;	

private:
        QString extInf_;
        QString file_;
        QString fullFile_;
        QString originalFile_;
	
};



#endif
