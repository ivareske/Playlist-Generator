#include "Tag.h"


/*!
 \brief

 \param fullfile
*/
Tag::Tag(const QString& fullfile, QObject *parent) : QObject(parent){
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
    //qDebug()<<"Tag::fileName()";
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
*
/
QVariant Tag::getTag( Tag::TagField field ){


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

QHash< QString, QHash<QString,QStringList> > Tag::frames() const{
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
    //read all frames/items/attributes and store them in qhash frames_

    //http://kid3.sourcearchive.com/documentation/0.8.1/taglibframelist_8cpp-source.html

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

    //flac
    TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File*>(file.file());
    if( flacFile && flacFile->isValid() ){
        readID3V2Frames( flacFile->ID3v2Tag(false), "ID3V2" );
        readXiphComment(flacFile->xiphComment(false), "XIPH" );
    }

    #ifdef HAVE_TAGLIB_APEFILE_H
        //APE items
        TagLib::APE::File *apeFile = dynamic_cast<TagLib::APE::File*>(file.file());
        if( apeFile && apeFile->isValid() ){
            readAPEItems( apeFile->APETag(), "APE"  );
        }
    #endif


    //wavpack
    TagLib::WavPack::File *wavPackFile = dynamic_cast<TagLib::WavPack::File*>(file.file());
    if( wavPackFile && wavPackFile->isValid() ){
        readAPEItems( wavPackFile->APETag(false), "APE" );
    }

    //wavpack
    TagLib::MPC::File *mpcFile = dynamic_cast<TagLib::MPC::File*>(file.file());
    if( mpcFile && mpcFile->isValid() ){
        readAPEItems( mpcFile->APETag(false), "APE" );
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

    //ogg flac
    TagLib::Ogg::FLAC::File *oggFlacFile = dynamic_cast<TagLib::Ogg::FLAC::File*>(file.file());
    if( oggFlacFile && oggFlacFile->isValid() ){
        readXiphComment( oggFlacFile->tag(), "XIPH" );
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
        QHash<QString,QStringList> list = frames_[type];
        for(TagLib::ASF::AttributeListMap::ConstIterator it = asfTag->attributeListMap().begin(); it != asfTag->attributeListMap().end(); ++it){

            TagLib::List<TagLib::ASF::Attribute> attributes = (*it).second;
            QStringList data;
            QString id = TStringToQString((*it).first);
            for(uint i=0;i<attributes.size();i++){
                data << TStringToQString(attributes[i].toString());
            }
            //qDebug()<<type<<id<<data;
            list.insert(id,data);
        }
        frames_[type] = list;
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

        QHash<QString,QStringList> list = frames_[type];
        for(TagLib::MP4::ItemListMap::ConstIterator it = mp4Tag->itemListMap().begin(); it != mp4Tag->itemListMap().end(); ++it){
            QStringList data;
            QString id = TStringToQString((*it).first);
            TagLib::StringList stringList = (*it).second.toStringList();
            for(uint i=0;i<stringList.size();i++){
                data << TStringToQString(stringList[i]);
            }
            //qDebug()<<type<<id<<data;
            list.insert(id,data);
        }
        frames_[type]=list;
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
        QHash<QString,QStringList> list = frames_[type];
        TagLib::Ogg::FieldListMap map=tag->fieldListMap();
        for (TagLib::Ogg::FieldListMap::ConstIterator it = map.begin(); it != map.end(); ++it){
            QString id = TStringToQString(TagLib::String((*it).first));
            TagLib::StringList list2 = (*it).second;
            QStringList data;
            for(uint i=0;i<list2.size();i++){
                data << TStringToQString(list2[i]);
            }
            //qDebug()<<type<<id<<data;
            list.insert(id,data);
        }
        frames_[type] = list;
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
        QHash<QString,QStringList> list = frames_[type];
        TagLib::ID3v2::FrameList::ConstIterator it = id3v2tag->frameList().begin();
        for(; it != id3v2tag->frameList().end(); it++){
            QString id = TStringToQString(TagLib::String((*it)->frameID()));
            TagLib::ID3v2::FrameList l = id3v2tag->frameListMap()[id.toUtf8().data()];
            QStringList data;
            for(uint i=0;i<l.size();i++){
                data << TStringToQString(l[i]->toString());
            }
            list.insert(id,data);
            //qDebug()<<"Tag::readID3V2Frames: "<<type<<id<<data;
        }        
        frames_[type]=list;
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
        QHash<QString,QStringList> list = frames_[type];
        QHash<QString,QStringList> items;
        for(TagLib::APE::ItemListMap::ConstIterator it = ape->itemListMap().begin(); it != ape->itemListMap().end(); ++it){
            QString data = TStringToQString((*it).second.toString());
            QString id = TStringToQString((*it).first);
            items[id]<<data;
        }
        QStringList keys = items.keys();
        for(int i=0;i<keys.size();i++){
            list.insert(keys[i],items[keys[i]]);
            //qDebug()<<type<<keys[i]<<items[keys[i]];
        }        
        frames_[type]=list;
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

QHash<QString,QStringList> Tag::ID3v2Frames() const{
    return frames_["ID3V2"];
}
QHash<QString,QStringList> Tag::APEItems() const{
    return frames_["APE"];
}
QHash<QString,QStringList> Tag::MP4Items() const{
    return frames_["MP4"];
}
QHash<QString,QStringList> Tag::ASFAttributes() const{
    return frames_["ASF"];
}

QHash<QString, QStringList> Tag::xiphFrames() const{
    return frames_["XIPH"];
}
