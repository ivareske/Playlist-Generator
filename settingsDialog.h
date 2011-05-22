#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>
#include "Rule.h"
#include "PlayList.h"
#include "globalFunctions.h"
#include "ui_settingsDialog.h"


class SettingsDialog : public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

public:

    SettingsDialog( SettingsClass *settings, QWidget *parent = 0);


    //public slots:



    //void getSettings();

protected:

private:

    void setSettings();
    QString chooseDir();
    //QSettings *settings;
    SettingsClass *settings_;
private slots:
    void onFinish( int result );
    void setOutPutDir();

};


#endif






