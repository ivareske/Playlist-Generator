#include "TextViewer.h"

 
/*!
 \brief

 \param parent
 \param text
*/
TextViewer::TextViewer(QWidget* parent, QString* text) {
    setupUi(this); // this sets up GUI
    if (!text->isNull()) {
        textEdit->setText(*text);
    }
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
