#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0);

signals:
    void editingFinished();
public slots:

protected:
    void focusOutEvent ( QFocusEvent * event );
};

#endif // TEXTEDIT_H
