#include "Rule.h"


Rule::Rule(){
    
    type_ = UNKNOWN;
    shouldBeTrue_ = true;
    caseSensitive_ = true;
}

bool Rule::operator==(const Rule &other) const{
    bool res = type_==other.type() && value_==other.value();
    res &= shouldBeTrue_==other.shouldBeTrue() && caseSensitive_==other.caseSensitive();
    return res;
}

Rule::RuleType Rule::type() const{
    return type_;
}

QString Rule::value() const{
    return value_;
}

bool Rule::shouldBeTrue() const{
    return shouldBeTrue_;
}

bool Rule::caseSensitive() const{
    return caseSensitive_;
}


void Rule::setType( RuleType type ){
    type_ = type;
}

void Rule::setValue( const QString &value ){
    value_ = value;
}

void Rule::setShouldBeTrue( bool shouldBeTrue ){
    shouldBeTrue_ = shouldBeTrue;
}

void Rule::setCaseSensitive( bool caseSensitive ){
    caseSensitive_ = caseSensitive;
}

QString Rule::getRuleName( const Rule::RuleType &type ){

    QString ans="";
    if( type==Rule::FILENAME_CONTAINS ){
        ans = "File name contains";
    }else if( type==Rule::FILENAME_EQUALS ){
        ans = "File name equals";
    }else if( type==Rule::TAG_TITLE_CONTAINS ){
        ans = "Title tag contains";
    }else if( type==Rule::TAG_TITLE_EQUALS ){
        ans = "Title tag equals";
    }else if( type==Rule::TAG_ARTIST_CONTAINS ){
        ans = "Artist tag contains";
    }else if( type==Rule::TAG_ARTIST_EQUALS ){
        ans = "Artist tag equals";
    }else if( type==Rule::TAG_ALBUM_CONTAINS ){
        ans = "Album tag contains";
    }else if( type==Rule::TAG_ALBUM_EQUALS ){
        ans = "Album tag equals";
    }else if( type==Rule::TAG_YEAR_IS ){
        ans = "Year tag is";
    }else if( type==Rule::TAG_COMMENT_CONTAINS ){
        ans = "Comment tag contains";
    }else if( type==Rule::TAG_COMMENT_EQUALS ){
        ans = "Comment tag equals";
    }else if( type==Rule::TAG_TRACK_IS ){
        ans = "Track tag is";
    }else if( type==Rule::TAG_GENRE_CONTAINS ){
        ans = "Genre tag contains";
    }else if( type==Rule::TAG_GENRE_EQUALS ){
        ans = "Genre tag equals";
    }else if( type==Rule::AUDIO_BITRATE_IS ){
        ans = "Audio bitrate is";
    }else if( type==Rule::AUDIO_SAMPLERATE_IS ){
        ans = "Audio samplerate is";
    }else if( type==Rule::AUDIO_CHANNELS_IS ){
        ans = "Audio channels is";
    }else if( type==Rule::AUDIO_LENGTH_IS ){
        ans = "Audio lenght is";
    }else{
        qDebug()<<"UNKOWN RULE TYPE IN Rule::getRuleName "<<type;
    }

    return ans;

}


Rule::operator QVariant () const{
    return QVariant::fromValue(*this);
}

QDataStream &operator>>(QDataStream &in, Rule &r){

    bool ok; QString log;
    QString version = Global::versionCheck( &in, &ok, &log );
    if(!ok){
        QMessageBox::critical(0,"",log);
    }

    if(version!="1.0"){
        in.setStatus(QDataStream::ReadCorruptData);
        return in;
    }

    quint32 type;
    QString value;
    bool shouldBeTrue;
    bool caseSensitive;
    in >> type >> value >> shouldBeTrue >> caseSensitive;
    r = Rule();
    r.setType(static_cast<Rule::RuleType>(type)); r.setValue(value); r.setCaseSensitive(caseSensitive); r.setShouldBeTrue(shouldBeTrue);
    return in;
}

QDataStream &operator<<(QDataStream &out, const Rule &r){

    out << out.version();
    out << qApp->applicationVersion();

    out << static_cast<quint32>(r.type()) << r.value() << r.caseSensitive() << r.shouldBeTrue();
    return out;
}



