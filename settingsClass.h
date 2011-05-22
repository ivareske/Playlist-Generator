#ifndef SETTINGS_CLASS_H
#define SETTINGS_CLASS_H

#include <QtGui>

class SettingsClass{

public:
    SettingsClass();

    QString style() const;
    bool artistEmpty() const;
    bool titleEmpty() const;
    bool albumEmpty() const;
    bool commentEmpty() const;
    bool genreEmpty() const;
    bool trackEmpty() const;
    bool yearEmpty() const;
    QString defaultOutputFolder() const;
    QStringList defaultExtensions() const;
    QString format() const;
    bool useScript() const;
    bool showLog() const;
    bool useCopyFilesToPath() const;
    bool keepTags() const;
    QString copyFilesToDir() const;
    QSize textViewerSize() const;

    void setArtistEmpty( bool artistEmpty );
    void setTitleEmpty( bool titleEmpty );
    void setAlbumEmpty( bool albumEmpty );
    void setCommentEmpty( bool commentEmpty );
    void setGenreEmpty( bool genreEmpty );
    void setTrackEmpty( bool trackEmpty );
    void setYearEmpty( bool yearEmpty );
    void setUseScript( bool useScript );
    void setShowLog( bool showLog );
    void setUseCopyFilesToPath( bool useCopyFilesToPath );
    void setKeepTags( bool keepTags );
    void setDefaultOutputFolder( const QString &defaultOutputFolder );
    void setDefaultExtensions( const QStringList &defaultExtensions );
    void setFormat( const QString &format );
    void setStyle( const QString &style );
    void setCopyFilesToDir( const QString &copyFilesToDir );
    void setTextViewerSize( const QSize &textViewerSize );
    operator QVariant () const;
private:

    QString style_;
    bool artistEmpty_;
    bool titleEmpty_;
    bool albumEmpty_;
    bool commentEmpty_;
    bool genreEmpty_;
    bool trackEmpty_;
    bool yearEmpty_;
    QString defaultOutputFolder_;
    QStringList defaultExtensions_;
    QString format_;
    bool useScript_;
    bool showLog_;
    bool useCopyFilesToPath_;
    bool keepTags_;
    QString copyFilesToDir_;
    QSize textViewerSize_;

    //bool showTagLibDebug_;

};

Q_DECLARE_METATYPE(SettingsClass);


QDataStream &operator>>(QDataStream &in, SettingsClass &s);
QDataStream &operator<<(QDataStream &out, const SettingsClass &s);

#endif

