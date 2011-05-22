#ifndef RULEDIALOG_H
#define RULEDIALOG_H
 
#include <QtGui>
#include "rule.h"
#include "playList.h"
#include "globalFunctions.h"
#include "ui_ruleDialog.h"
 
 
class ruleDialog : public QDialog, private Ui::ruleDialog{
    Q_OBJECT
 
public:

    //ruleDialog(QWidget *parent = 0, int index=0, QString val="");
        ruleDialog( rule *r=0, QWidget *parent = 0 );
	
	
public slots:

	void setSettings( );
	rule getSettings();
	void indexChanged(int ind);
	void disableRuleChange();
	void finito();

protected:
	
};
 
 
#endif






