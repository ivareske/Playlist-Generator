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

    SettingsDialog( QWidget *parent = 0);
    ~SettingsDialog();


protected:

private:
    QSettings *settings;
    void setSettings();
    QString chooseDir();    
private slots:
    void onFinish( int result );
    void setOutPutDir();

};


#endif






