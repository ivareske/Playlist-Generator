#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui>

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0);
        QString text() const;
signals:
    void editingFinished();
    void entered();
public slots:

protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent ( QFocusEvent * event );
};

#endif // TEXTEDIT_H
