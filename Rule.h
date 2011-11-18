#ifndef RULE_H
#define RULE_H

#include <QtGui>
#include "GlobalFunctions.h"

class Rule {

    public:
        enum RuleType { UNKNOWN, FILENAME_CONTAINS, FILENAME_EQUALS, TAG_TITLE_CONTAINS, TAG_TITLE_EQUALS, TAG_ARTIST_CONTAINS, TAG_ARTIST_EQUALS, \
                        TAG_ALBUM_CONTAINS, TAG_ALBUM_EQUALS, TAG_YEAR_IS, TAG_COMMENT_CONTAINS, TAG_COMMENT_EQUALS, TAG_TRACK_IS, TAG_GENRE_CONTAINS, TAG_GENRE_EQUALS, AUDIO_BITRATE_IS, AUDIO_SAMPLERATE_IS, \
                        AUDIO_CHANNELS_IS, AUDIO_LENGTH_IS, NUMBEROFRULES
                      };
        Rule(Rule::RuleType type = Rule::UNKNOWN, const QString& value = "");

        RuleType type() const;
        QString value() const;
        bool shouldBeTrue() const;
        bool caseSensitive() const;

        void setType(RuleType);
        void setValue(const QString& value);
        void setShouldBeTrue(bool shouldBeTrue);
        void setCaseSensitive(bool caseSensitive);
        static QString getRuleName(const Rule::RuleType& type);


        operator QVariant() const;
        bool operator==(const Rule& other) const;

    private:
        RuleType type_;
        QString value_;
        bool shouldBeTrue_;
        bool caseSensitive_;


};

Q_DECLARE_METATYPE(Rule);


QDataStream& operator>>(QDataStream& in, Rule& r);
QDataStream& operator<<(QDataStream& out, const Rule& r);

#endif

