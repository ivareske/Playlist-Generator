#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui>
#include "CodeEditor.h"
#include "GlobalFunctions.h"

class TabWidget : public QTabWidget{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = 0);
    CodeEditor *codeEditor(int ind=-1) const;
    QString text(int ind=-1) const;
    void removeAllTabs();
    QString tabText(int ind=-1) const;
    void setText(const QString &text, int ind=-1);
    void setIsDeletable(int ind , bool isDeletable);
    bool isDeletable(int ind, bool default_=true) const;
    QList< QPair<QString,QString> > scripts() const;    
    QString commonScript() const;
public slots:
    void removeCurrentTab();
    int addTab(const QString &label="New script");

signals:
    void textChanged();

protected:

private slots:
    void tabTextChanged(int index,const QString &text);
    void closeTab(int ind);
    void contextMenu(const QPoint &p);
    
};

class IndependentLineEdit: public QLineEdit {
    Q_OBJECT
public:
    IndependentLineEdit(QWidget* parent) :
        QLineEdit(parent) {
        setupWidget();
    }

    IndependentLineEdit(const QString & contents, QWidget* parent) :
        QLineEdit(contents, parent) {
        setupWidget();
    }
public slots:
    void setText(const QString &text) {
        QLineEdit::setText(text);
        setFocus();
        selectAll();
        show();
    }
signals:
    void newTextEntered(const QString &text);
protected:
    void keyPressEvent(QKeyEvent* event) {
        QLineEdit::keyPressEvent(event);
        if (event->key() == Qt::Key_Escape) {
            setText("");
            hide();
        }
    }

    void focusOutEvent(QFocusEvent* event) {
        QLineEdit::focusOutEvent(event);
        emit editingFinished();
    }
private slots:
    void publishText() {
        if (text() != "") {
            emit newTextEntered(text());
        }
        hide();
    }

    void setupWidget() {
        connect(this, SIGNAL(editingFinished()), this, SLOT(publishText()));
        setWindowFlags(Qt::CustomizeWindowHint);
    }
};



class EditableTabBar: public QTabBar {
    Q_OBJECT
public:
    EditableTabBar(QWidget* parent=0) : QTabBar(parent), _edit("", 0) {
        connect(&_edit, SIGNAL(newTextEntered(const QString &)), this,
                SLOT( setCurrentTabText(const QString &)));
    }
signals:
    void tabTextChanged(int index, const QString &text);
protected:
    void mouseDoubleClickEvent(QMouseEvent* event) {
        _edit.setText(tabText(currentIndex()));
        _edit.move(mapToGlobal(event->pos()));
    }
private slots:
    void setCurrentTabText(const QString &text) {
        if(this->tabText(currentIndex())=="Common script variables"){
            return;
        }
        setTabText(currentIndex(), text);
        emit tabTextChanged(currentIndex(), text);
    }
private:
    IndependentLineEdit _edit;
};







#endif // TABWIDGET_H
