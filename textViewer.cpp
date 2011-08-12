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

    resize(Global::guiSettings()->value("textViewer/size",QSize(400,400)).toSize());
    move(Global::guiSettings()->value("textViewer/position",this->pos()).toPoint());

    connect(editFontButton,SIGNAL(clicked()),this,SLOT(editFont()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(this,SIGNAL(finished(int)),this,SLOT(onFinish(int)));
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

/*!
 \brief Store size and position before exit

 \param event
*/
void TextViewer::onFinish(int result){

    Global::guiSettings()->setValue("textViewer/position",this->pos());
    Global::guiSettings()->setValue("textViewer/size",this->size());
    Global::guiSettings()->sync();
}
