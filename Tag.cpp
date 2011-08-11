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

    length_ = 0;
    bitRate_ = 0;
    sampleRate_ = 0;
    channels_ = 0;
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


/*!
 \brief
 \param field
 \return QVariant
*/
QVariant Tag::getTag( Tag::TagField field ){

    /*
    if(!tagIsRead_){
        readTags();
    }
    */

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


/*!
 \brief

*/

QHash<QString,QString> Tag::frames() const{
    return frames_;
}


/*!
 \brief

*/
void Tag::clearFrames(){
    frames_.clear();
}

/*!
 \brief

*/
void Tag::readFrames() {
    //read all ID3v2 frames and APE items
    //http://kid3.sourcearchive.com/documentation/0.8.1/taglibframelist_8cpp-source.html

    //qDebug()<<filename_;
    frames_.clear();


    //https://code.launchpad.net/~mixxxdevelopers/mixxx/features_recording2/+merge/55016

    TagLib::FileRef file(filename_.toStdString().c_str(),false);

    //MPEG ID3v2 tags
    TagLib::MPEG::File *mpegFile = dynamic_cast<TagLib::MPEG::File*>(file.file());
    if( mpegFile && mpegFile->isValid() ){
        readID3V2Frames( mpegFile->ID3v2Tag(), "ID3V2" );
    }

    //true audio ID3v2 tags
    TagLib::TrueAudio::File *trueAudioFile = dynamic_cast<TagLib::TrueAudio::File*>(file.file());
    if( trueAudioFile && trueAudioFile->isValid() ){
        readID3V2Frames( trueAudioFile->ID3v2Tag(false), "ID3V2" );
    }

    //wav ID3v2 tags
    TagLib::RIFF::WAV::File *wavFile = dynamic_cast<TagLib::RIFF::WAV::File*>(file.file());
    if( wavFile && wavFile->isValid() ){
        readID3V2Frames( wavFile->tag(), "ID3V2"  );
    }


    //aiff ID3v2 tags
    TagLib::RIFF::AIFF::File *aifFile = dynamic_cast<TagLib::RIFF::AIFF::File*>(file.file());
    if( aifFile && aifFile->isValid() ){
        readID3V2Frames( aifFile->tag(), "AIF"  );
    }


    //APE items
    TagLib::APE::File *apeFile = dynamic_cast<TagLib::APE::File*>(file.file());
    if( apeFile && apeFile->isValid() ){
        readAPEItems( apeFile->APETag(), "APE"  );
    }


    //just like asf/mp4 tags, one vorbis item can have multiple values. How to handle this?
    //vorbis
    TagLib::Ogg::Vorbis::File *vorbisFile = dynamic_cast<TagLib::Ogg::Vorbis::File*>(file.file());
    if( vorbisFile && vorbisFile->isValid() ){
        readXiphComment( vorbisFile->tag(), "XIPH" );
    }

    //speex
    TagLib::Ogg::Speex::File *speexFile = dynamic_cast<TagLib::Ogg::Speex::File*>(file.file());
    if( speexFile && speexFile->isValid() ){
        readXiphComment( speexFile->tag(), "XIPH" );
    }

    //flac
    TagLib::Ogg::FLAC::File *flacFile = dynamic_cast<TagLib::Ogg::FLAC::File*>(file.file());
    if( flacFile && flacFile->isValid() ){
        readXiphComment( flacFile->tag(), "XIPH" );
    }


    //just like asf tags, one mp4 item can have multiple values. How to handle this?
    //MP4 tags
    TagLib::MP4::File *mp4File = dynamic_cast<TagLib::MP4::File*>(file.file());
    if( mp4File && mp4File->isValid() ){
        readMP4Items( mp4File->tag(), "MP4" );
    }


    //ASF
    //one asf tag/id can contain  list of values. how to let the user use these values?
    TagLib::ASF::File *asfFile = dynamic_cast<TagLib::ASF::File*>(file.file());
    if( asfFile && asfFile->isValid() ){
        readASFAttributes( asfFile->tag(), "ASF" );
    }

    //wavpack
    TagLib::WavPack::File *wavPackFile = dynamic_cast<TagLib::WavPack::File*>(file.file());
    if( wavPackFile && wavPackFile->isValid() ){
        readAPEItems( wavPackFile->APETag(false), "APE" );
    }


}

/*!
 \brief Read asf attributes
 \param asfTag
 \return bool
*/
bool Tag::readASFAttributes( TagLib::ASF::Tag *asfTag, const QString &type ){
    if(asfTag){
        qDebug()<<"found ASF tag";
        //ASF items
        for(TagLib::ASF::AttributeListMap::ConstIterator it = asfTag->attributeListMap().begin(); it != asfTag->attributeListMap().end(); ++it){

            TagLib::List<TagLib::ASF::Attribute> attributes = (*it).second;
            QString data;
            QString id = TStringToQString((*it).first);
            for(uint i=0;i<attributes.size();i++){
                data = TStringToQString(attributes[i].toString());
                qDebug()<<"ASF data: "<<id<<data;
            }
            frames_.insert(type+"::"+id,data);
        }
        return true;
    }else{
        return false;
        qDebug()<<"NO ASF TAG";
    }
}

/*!
 \brief Read mp4 items
 \param mp4Tag
 \return bool
*/
bool Tag::readMP4Items( TagLib::MP4::Tag *mp4Tag, const QString &type ){
    if(mp4Tag){
        qDebug()<<"found mp4 tag";

        for(TagLib::MP4::ItemListMap::ConstIterator it = mp4Tag->itemListMap().begin(); it != mp4Tag->itemListMap().end(); ++it){
            QString data;
            QString id = TStringToQString((*it).first);
            TagLib::StringList stringList = (*it).second.toStringList();
            for(uint i=0;i<stringList.size();i++){
                data = TStringToQString(stringList[i]);
                qDebug()<<"MP4 data: "<<id<<data;
            }
            frames_.insert(type+"::"+id,data);
        }
        return true;
    }else{
        return false;
        qDebug()<<"NO MP4 TAG";
    }
}


/*!
 \brief Read either vorbis, speex or flac XiphComment items/fields
 \param tag type
 \return bool
*/
bool Tag::readXiphComment( TagLib::Ogg::XiphComment *tag, const QString &type ) {

    if(tag) {
        qDebug()<<"found "<<type<<" tag";
        TagLib::Ogg::FieldListMap map=tag->fieldListMap();
        for (TagLib::Ogg::FieldListMap::ConstIterator it = map.begin(); it != map.end(); ++it){
            QString id = TStringToQString(TagLib::String((*it).first));
            TagLib::StringList list = (*it).second;
            for(uint i=0;i<list.size();i++){
                QString data = TStringToQString(list[i]);
                qDebug()<<type<<id<<data;
                frames_.insert(type+"::"+id,data);
            }
        }
        return true;
    }else{
        return false;
        qDebug()<<"NO "<<type<<" TAG";
    }

}

/*!
  \brief Read frames from id3v2 tag
  \param id3v2tag
  \return bool
*/
bool Tag::readID3V2Frames( TagLib::ID3v2::Tag *id3v2tag, const QString &type ){
    if(id3v2tag) {
        TagLib::ID3v2::FrameList::ConstIterator it = id3v2tag->frameList().begin();
        for(; it != id3v2tag->frameList().end(); it++){
            //cout << (*it)->frameID() << " - \"" << (*it)->toString() << "\"" << endl;
            QString id = TStringToQString(TagLib::String((*it)->frameID()));
            QString content = TStringToQString((*it)->toString());
            //qDebug()<<"ID3v2"<<id<<content;
            frames_.insert(type+"::"+id,content);
        }
        return true;
    }else{
        qDebug()<<"no id3v2 tag in mpeg file";
        return false;
    }
}

/*!
  \brief Read frames from ape tag
  \param ape
  \return bool
*/

bool Tag::readAPEItems( TagLib::APE::Tag *ape, const QString &type ){
    if(ape) {
        for(TagLib::APE::ItemListMap::ConstIterator it = ape->itemListMap().begin(); it != ape->itemListMap().end(); ++it){

            QString data = TStringToQString((*it).second.toString());
            QString id = TStringToQString((*it).first);
            //qDebug()<<"APE: "<<id<<item.toString().toCString();
            frames_.insert(type+"::"+id,data);
        }
        return true;
    }else{
        qDebug()<<"no ape tag";
        return false;
    }
}


/*!
 \brief

*/

void Tag::readTags() {

    tagIsRead_ = true;
    //qDebug()<<"reading tags for "<<filename_;
    TagLib::FileRef f(filename_.toStdString().c_str());
    if(f.isNull()){
        return;
    }
    if (f.tag()!= 0) {
        artist_ = f.tag()->artist().toCString();
        album_ = f.tag()->album().toCString();
        title_ = f.tag()->title().toCString();
        genre_ = f.tag()->genre().toCString();
        comment_ = f.tag()->comment().toCString();
        year_ = f.tag()->year();
        track_ = f.tag()->track();
        tagOk_ = true;
    }
    if (f.audioProperties() != 0) {
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
