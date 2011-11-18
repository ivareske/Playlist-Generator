#ifndef PMSETTINGS_H
#define PMSETTINGS_H

#include<QtGui>

class PMSettings : public QSettings{
    Q_OBJECT
public:
    explicit PMSettings(const QString & fileName, Format format, QObject * parent = 0);
    bool checkForEmptyValue() const;
    void setCheckForEmptyValue( bool check );
    QVariant value(const QString &key, const QVariant &defaultValue=QVariant()) const;
signals:

public slots:

private:
    bool checkForEmptyValue_;
};

#endif // PMSETTINGS_H
