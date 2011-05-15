#ifndef RULE_H
#define RULE_H

#include "structs.h"


class rule{

public:
    rule();
    enum ruleType{ UNKNOWN, FILENAME_CONTAINS,FILENAME_EQUALS, TAG_TITLE_CONTAINS, TAG_TITLE_EQUALS, TAG_ARTIST_CONTAINS, TAG_ARTIST_EQUALS,\
            TAG_ALBUM_CONTAINS, TAG_ALBUM_EQUALS, TAG_YEAR_IS, TAG_COMMENT_CONTAINS, TAG_COMMENT_EQUALS, TAG_TRACK_IS, TAG_GENRE_CONTAINS, TAG_GENRE_EQUALS, AUDIO_BITRATE_IS, AUDIO_SAMPLERATE_IS,\
            AUDIO_CHANNELS_IS, AUDIO_LENGTH_IS, NUMBEROFRULES };

    ruleType type() const;
    QString value() const;
    bool shouldBeTrue() const;
    bool caseSensitive() const;

    void setType( ruleType );
    void setValue( const QString &value );
    void setShouldBeTrue( bool shouldBeTrue );
    void setCaseSensitive( bool caseSensitive );

    operator QVariant () const;

private:
    ruleType type_;
    QString value_;
    bool shouldBeTrue_;
    bool caseSensitive_;


};

Q_DECLARE_METATYPE(rule);


QDataStream &operator>>(QDataStream &in, rule &r);
QDataStream &operator<<(QDataStream &out, const rule &r);

#endif

