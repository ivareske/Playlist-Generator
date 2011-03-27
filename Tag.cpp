#include "Tag.h"


Tag::Tag(QString fullfile){
    artist="";
	title="";
	album="";
	comment="";
	genre="";
	//year=0;
	//track=0;
        filename_ = fullfile;
	tagisread=false;
	tagok=false;
	audiopropertiesok=false;
	
	length=-1;
	bitrate=-1;
	samplerate=-1;
	channels=-1;
}

QString Tag::fileName() const{
        return filename_;
}

bool Tag::tagOk() const{
	return tagok;
}

bool Tag::tagIsRead() const{
	return tagisread;
}

bool Tag::audiopropertiesOk() const{
	return audiopropertiesok;
}

QVariant Tag::getTag( QString field ){
	
	if(!tagisread){
		readTags();
	}
	
	QVariant result;
	if(field=="artist"){
		result = QVariant(artist);
	}else if(field=="album"){
		result = QVariant(album);
	}else if(field=="title"){
		result = QVariant(title);
	}else if(field=="genre"){
		result = QVariant(genre);
	}else if(field=="comment"){
		result = QVariant(artist);
	}else if(field=="year"){
		result = QVariant(year);
	}else if(field=="track"){
		result = QVariant(track);
	}else if(field=="length"){
		result = QVariant(length);
	}else if(field=="bitrate"){
		result = QVariant(bitrate);
	}else if(field=="samplerate"){
		result = QVariant(samplerate);
	}else if(field=="channels"){
		result = QVariant(channels);
	}
	return result;
}


void Tag::readTags(){
	
	tagisread=true;
        TagLib::FileRef f( filename_.toStdString().c_str() );
	if( !f.isNull() && f.tag()!=NULL  ){
		artist = f.tag()->artist().toCString();
		album = f.tag()->album().toCString();
		title = f.tag()->title().toCString();
		genre = f.tag()->genre().toCString();
		comment = f.tag()->comment().toCString();
		year = f.tag()->year();
		track = f.tag()->track();
		tagok=true;
	}
	if( !f.isNull() && f.audioProperties()!=NULL ){				
		length = f.audioProperties()->length();
		bitrate = f.audioProperties()->bitrate();
		samplerate = f.audioProperties()->sampleRate();
		channels = f.audioProperties()->channels();
		audiopropertiesok=true;
	}

}


void Tag::clearTags(){
	artist="";
	title="";
	album="";
	comment="";
	genre="";
	//year=0;
	//track=0;
	tagisread=false;
	audiopropertiesok=false;
	tagok=false;
	length=-1;
	bitrate=-1;
	samplerate=-1;
	channels=-1;
}
