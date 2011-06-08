#ifndef TAG_H
#define TAG_H

#include <QtGui>
#include <tag.h>
#include <taglib.h>
#include <fileref.h>

class Tag {

    public:
        enum TagField { ARTIST, ALBUM, TITLE, GENRE, TRACK, YEAR, COMMENT, LENGTH, BITRATE, SAMPLERATE, CHANNELS };
        Tag(const QString& fullfile = "");
        void clearTags();
        void readTags();
        //QVariant getTag( Tag::TagField field );
        bool tagIsRead() const;
        bool tagOk() const;
        bool audioPropertiesOk() const;
        QString fileName() const;

        QString filename() const;
        QString artist() const;
        QString title() const;
        QString album() const;
        QString comment() const;
        QString genre() const;
        uint year() const;
        uint track() const;

        uint length() const;
        uint bitRate() const;
        uint sampleRate() const;
        uint channels() const;

    private:
        QString filename_;
        QString artist_;
        QString title_;
        QString album_;
        QString comment_;
        QString genre_;
        uint year_;
        uint track_;
        bool tagIsRead_;
        bool tagOk_;
        bool audioPropertiesOk_;

        uint length_;
        uint bitRate_;
        uint sampleRate_;
        uint channels_;

};



#endif
