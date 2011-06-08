#include "Tag.h"


/*!
 \brief

 \param fullfile
*/
Tag::Tag(const QString& fullfile) {
    artist_ = "";
    title_ = "";
    album_ = "";
    comment_ = "";
    genre_ = "";
    //year_=0;
    //track_=0;
    filename_ = fullfile;
    tagIsRead_ = false;
    tagOk_ = false;
    audioPropertiesOk_ = false;

    length_ = -1;
    bitRate_ = -1;
    sampleRate_ = -1;
    channels_ = -1;
}


/*!
 \brief

 \return QString
*/
QString Tag::artist() const {
    return artist_;
}

/*!
 \brief

 \return QString
*/
QString Tag::title() const {
    return title_;
}

/*!
 \brief

 \return QString
*/
QString Tag::album() const {
    return album_;
}

/*!
 \brief

 \return QString
*/
QString Tag::comment() const {
    return comment_;
}

/*!
 \brief

 \return QString
*/
QString Tag::genre() const {
    return genre_;
}

/*!
 \brief

 \return uint
*/
uint Tag::year() const {
    return year_;
}

/*!
 \brief

 \return uint
*/
uint Tag::track() const {
    return track_;
}

/*!
 \brief

 \return uint
*/
uint Tag::length() const {
    return length_;
}

/*!
 \brief

 \return uint
*/
uint Tag::bitRate() const {
    return bitRate_;
}

/*!
 \brief

 \return uint
*/
uint Tag::sampleRate() const {
    return sampleRate_;
}

/*!
 \brief

 \return uint
*/
uint Tag::channels() const {
    return channels_;
}



/*!
 \brief

 \return QString
*/
QString Tag::fileName() const {
    return filename_;
}

/*!
 \brief

 \return bool
*/
bool Tag::tagOk() const {
    return tagOk_;
}

/*!
 \brief

 \return bool
*/
bool Tag::tagIsRead() const {
    return tagIsRead_;
}

/*!
 \brief

 \return bool
*/
bool Tag::audioPropertiesOk() const {
    return audioPropertiesOk_;
}

/*
QVariant Tag::getTag( Tag::TagField field ){

    if(!tagIsRead_){
        readTags();
    }

    QVariant result;
    if(field==Tag::ARTIST){
        result = QVariant(artist_);
    }else if(field==Tag::ALBUM){
        result = QVariant(album_);
    }else if(field==Tag::TITLE){
        result = QVariant(title_);
    }else if(field==Tag::GENRE){
        result = QVariant(genre_);
    }else if(field==Tag::COMMENT){
        result = QVariant(artist_);
    }else if(field==Tag::YEAR){
        result = QVariant(year_);
    }else if(field==Tag::TRACK){
        result = QVariant(track_);
    }else if(field==Tag::LENGTH){
        result = QVariant(length_);
    }else if(field==Tag::BITRATE){
        result = QVariant(bitRate_);
    }else if(field==Tag::SAMPLERATE){
        result = QVariant(sampleRate_);
    }else if(field==Tag::CHANNELS){
        result = QVariant(channels_);
    }
    return result;
}
*/

/*!
 \brief

*/
void Tag::readTags() {

    tagIsRead_ = true;
    TagLib::FileRef f(filename_.toStdString().c_str());
    if (!f.isNull() && f.tag() != 0) {
        artist_ = f.tag()->artist().toCString();
        album_ = f.tag()->album().toCString();
        title_ = f.tag()->title().toCString();
        genre_ = f.tag()->genre().toCString();
        comment_ = f.tag()->comment().toCString();
        year_ = f.tag()->year();
        track_ = f.tag()->track();
        tagOk_ = true;
    }
    if (!f.isNull() && f.audioProperties() != 0) {
        length_ = f.audioProperties()->length();
        bitRate_ = f.audioProperties()->bitrate();
        sampleRate_ = f.audioProperties()->sampleRate();
        channels_ = f.audioProperties()->channels();
        audioPropertiesOk_ = true;
    }

}


/*!
 \brief

*/
void Tag::clearTags() {
    artist_ = "";
    title_ = "";
    album_ = "";
    comment_ = "";
    genre_ = "";
    //year_=0;
    //track_=0;
    tagIsRead_ = false;
    audioPropertiesOk_ = false;
    tagOk_ = false;
    length_ = -1;
    bitRate_ = -1;
    sampleRate_ = -1;
    channels_ = -1;
}
