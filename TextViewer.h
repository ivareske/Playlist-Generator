#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QtGui>
#include "ui_textViewer.h"
#include "GlobalFunctions.h"
#include "CodeEditor.h"

class TextViewer : public QDialog, private Ui::TextViewer {
    Q_OBJECT

public:

    TextViewer(QWidget* parent = 0, const QString& text = "");
    ~TextViewer();

public slots:

    bool readOnly() const;
    void setText(const QString& text);
    void setLabelText(const QString& text);
    QString text();
    void setReadOnly( bool enable );
    void enableCancelButton( bool enable );
    bool cancelButtonenabled() const;

protected:

private slots:
    void fontDialog();

private:
    CodeEditor *textEdit;
    PMSettings *s;

};


#endif






