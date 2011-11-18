#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QtGui>
#include "ui_TextViewer.h"
#include "GlobalFunctions.h"


class TextViewer : public QDialog, private Ui::TextViewer {
    Q_OBJECT

public:

    TextViewer(QWidget* parent = 0, QString* text = 0);
    QTextEdit* getTextEdit();
    QLabel *label();

public slots:

    void setText(QString text);

protected:

private slots:
    void editFont();
    void onFinish(int result);

};


#endif






