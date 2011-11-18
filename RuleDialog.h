#ifndef RULEDIALOG_H
#define RULEDIALOG_H
 
#include <QtGui>
#include "Rule.h"
#include "PlayList.h"
#include "GlobalFunctions.h"
#include "ui_RuleDialog.h"
 
 
class RuleDialog : public QDialog, private Ui::RuleDialog {
        Q_OBJECT

    public:

        RuleDialog(const Rule& r = Rule(), QWidget* parent = 0);
        void hideValueFrame();

    public slots:

        void setSettings();
        Rule getRule();
        void indexChanged(int ind);
        void disableRuleChange();
        void finito();

    protected:

    private:
        QString getRuleExample(const Rule::RuleType& type);

};
 
 
#endif






