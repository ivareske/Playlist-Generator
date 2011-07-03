#include "TextViewer.h"

 
/*!
 \brief

 \param parent
 \param text
*/
TextViewer::TextViewer(QWidget* parent, QString* text) : QDialog(parent) {
    setupUi(this); // this sets up GUI

    if (text!=0) {
        textEdit->setText(*text);
    }
    QFont f = Global::guiSettings()->value("textViewer/font",textEdit->font()).value<QFont>();
    textEdit->setFont(f);

    connect(editFontButton,SIGNAL(clicked()),this,SLOT(editFont()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
}

/*!
 \brief
*/
void TextViewer::editFont() {
    textEdit->setFont(QFontDialog::getFont(0, textEdit->font()));
    Global::guiSettings()->setValue("textViewer/font",textEdit->font());
}

/*!
 \brief

 \return QTextEdit *
*/
QTextEdit* TextViewer::getTextEdit() {
    return textEdit;
}

/*!
 \brief

 \param text
*/
QLabel *TextViewer::label(){
    return label_;
}

/*!
 \brief

 \param text
*/
void TextViewer::setText(QString text) {
    textEdit->setText(text);
}

/*
void TextViewer::resize( QSize size ){
  this->setSize(size);
}

void TextViewer::setFont( QFont font ){
  textEdit->setFont(font);
}
*/
