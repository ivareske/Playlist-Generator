#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>
#include "structs.h"
#include "rule.h"
#include "playList.h"
#include "globalFunctions.h"
#include "ui_settingsDialog.h"


class settingsDialog : public QDialog, private Ui::settingsDialog
{
    Q_OBJECT

public:

    settingsDialog( settingsClass *settings, QWidget *parent = 0);


    //public slots:



    //void getSettings();

protected:

private:

    void setSettings();
    QString chooseDir();
    //QSettings *settings;
    settingsClass *settings_;
private slots:
    void onFinish( int result );
    void setOutPutDir();

};


#endif






