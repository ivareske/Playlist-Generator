#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>
#include "PlayList.h"
#include "GlobalFunctions.h"
#include "TextViewer.h"
#include "ReplaceInFileNameDialog.h"
#include "ui_SettingsDialog.h"


class SettingsDialog : public QDialog, private Ui::SettingsDialog {
    Q_OBJECT

public:

    SettingsDialog(QWidget* parent = 0);
    ~SettingsDialog();

protected:

private slots:
    void showRelaceFileNameDialog();
    void editFrameFields();
    void onFinish(int result);
    void setOutPutDir();

private:
    PMSettings* settings;
    void setSettings();
    QString chooseDir();

};


#endif






