#ifndef RULEDIALOG_H
#define RULEDIALOG_H
 
#include <QtGui>
#include "Rule.h"
#include "PlayList.h"
#include "globalFunctions.h"
#include "ui_ruleDialog.h"
 
 
class RuleDialog : public QDialog, private Ui::RuleDialog{
    Q_OBJECT
 
public:

        RuleDialog( Rule *r=0, QWidget *parent = 0 );
	
	
public slots:

        void setSettings();
	Rule getSettings();
	void indexChanged(int ind);
	void disableRuleChange();
	void finito();

protected:

private:
        QString getRuleExample( const Rule::RuleType &type );
	
};
 
 
#endif






