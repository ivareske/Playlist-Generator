#ifndef REPLACEINFILENAMESDIALOG_H
#define REPLACEINFILENAMESDIALOG_H

#include <QtGui>
#include "GlobalFunctions.h"
#include "ui_ReplaceInFileNameDialog.h"


class ReplaceInFileNameDialog : public QDialog, private Ui::ReplaceInFileNameDialog {
    Q_OBJECT

public:

    ReplaceInFileNameDialog(QWidget* parent = 0);
    ~ReplaceInFileNameDialog();

private slots:
    void addEntry(const QString &from="", const QString &to="", QWidget *parent=0);
    void onFinish(int result);

private:
    PMSettings* settings;
    QFrame *frame;

};

class ReplacementBox : public QFrame{

public:
    ReplacementBox(const QString &from="", const QString &to="", QWidget *parent=0) : QFrame(parent) {
        this->setLayout(new QHBoxLayout());
        from_ = new QLineEdit(this);
        to_ = new QLineEdit(this);
        this->layout()->addWidget(from_);
        this->layout()->addItem(new QSpacerItem(25,10));
        this->layout()->addWidget(to_);
        this->layout()->addItem(new QSpacerItem(25,10));
        button_ = new QToolButton;button_->setText("");button_->setIcon(QIcon(":/red_cross.png"));
        connect(button_,SIGNAL(clicked()),this,SLOT(deleteLater()));
        this->layout()->addWidget(button_);
        setFrom(from);setTo(to);
    }
    QString from() const{
        return from_->text();
    }
    QString to() const{
        return to_->text();
    }
    void setFrom( const QString &from ){
        from_->setText(from);
    }
    void setTo( const QString &to ){
        to_->setText(to);
    }

private:
    QLineEdit *from_,*to_;
    QToolButton *button_;

};


#endif






