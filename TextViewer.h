#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QtGui>
#include "GlobalFunctions.h"
#include "ui_TextViewer.h"
#include <CodeEditor.h>

class TextViewer : public QDialog, private Ui::TextViewer {
    Q_OBJECT

public:

    TextViewer(QWidget* parent = 0, const QString& text = "");
    ~TextViewer();

public slots:

    void setLabelText(const QString &text);
    bool readOnly() const;
    void setText(const QString& text);
    QString text();
    void setReadOnly( bool enable );
    void enableCancelButton( bool enable );
    bool cancelButtonenabled() const;
    CodeEditor* codeEditor();
    void setExample(const QString &example);

protected:

private slots:

private:
    CodeEditor *textEdit;
    QSettings *s;

};


#endif






