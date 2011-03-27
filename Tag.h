#ifndef TAG_H
#define TAG_H

#include <QtGui>
#include <tag.h>
#include <taglib.h>
#include <fileref.h>

class Tag{
	
	public:
	Tag(QString fullfile="");
	void clearTags();
	void readTags();
        QVariant getTag( QString field );
        bool tagIsRead() const;
        bool tagOk() const;
        bool audiopropertiesOk() const;
        QString fileName() const;
	
private:
        QString filename_;
	QString artist;
	QString title;
	QString album;
	QString comment;
	QString genre;
	uint year;
	uint track;
	bool tagisread;
	bool tagok;
	bool audiopropertiesok;
	
	int length;
	int bitrate;
	int samplerate;
	int channels;
	
};



#endif
