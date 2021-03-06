#ifndef TAG_H
#define TAG_H

#include <iostream>
#include <QtGui>
#include <tag.h>
#include <taglib.h>
#include <fileref.h>

#include <tbytevector.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <id3v2frame.h>
#include <xiphcomment.h>
#include <asffile.h>
#include <id3v2header.h>
#include <id3v1tag.h>
#include <apetag.h>
#include <mp4file.h>
#include <oggfile.h>
#include <vorbisfile.h>
#include <speexfile.h>
#include <oggflacfile.h>
#include <wavfile.h>
#include <rifffile.h>
#include <aifffile.h>
#ifdef HAVE_TAGLIB_APEFILE_H
#include <apefile.h>
#endif
#include <trueaudiofile.h>
#include <wavpackfile.h>
#include <mpcfile.h>
#include <flacfile.h>
#include <id3v2frame.h>


class Tag : public QObject{
    Q_OBJECT
    //Q_PROPERTY( QString fileName READ fileName )
public:
    enum TagField { ARTIST, ALBUM, TITLE, GENRE, TRACK, YEAR, COMMENT, LENGTH, BITRATE, SAMPLERATE, CHANNELS, NTAGFIELDS };
    Tag(const QString& fullfile = "",QObject *parent=0);
public slots:
    void clearTags();
    void readTags();
    QVariant getTag( Tag::TagField field );
    bool tagIsRead() const;
    bool tagOk() const;
    bool audioPropertiesOk() const;
    QString fileName() const;

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

    void readFrames();
    void clearFrames();
    QHash< QString, QHash<QString,QStringList> > frames() const;

private:

    bool readXiphComment(TagLib::Ogg::XiphComment *tag, const QString &type="XIPH");
    bool readID3V2Frames(TagLib::ID3v2::Tag *id3v2tag, const QString &type="ID3V2");
    bool readAPEItems(TagLib::APE::Tag *ape, const QString &type="APE");
    bool readMP4Items(TagLib::MP4::Tag *mp4Tag, const QString &type="MP4");
    bool readASFAttributes(TagLib::ASF::Tag *asfTag, const QString &type="ASF");

    //QHash<QString,QStringList> = frames_["ID3V2"]; //usage
    QHash< QString, QHash<QString,QStringList> > frames_;
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

Q_DECLARE_METATYPE(Tag*)

#endif
