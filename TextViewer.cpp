#include "TextViewer.h"


/*!
 \brief General dialog with a texteditor. Used to show log messages/write comments

 \param parent
 \param text
*/
TextViewer::TextViewer(QWidget* parent, const QString& text) : QDialog(parent) {
    setupUi(this); // this sets up GUI

    QVBoxLayout *layout = new QVBoxLayout(frame);
    textEdit = new CodeEditor(this);
    layout->addWidget(textEdit);
    frame->setLayout(layout);
    this->setWindowTitle(qApp->applicationName());
    s = Global::guiSettings(this);
    s->beginGroup("TextViewer");
    QSize size = s->value("size",QSize(200,200)).toSize();
    QFont font = s->value("font",textEdit->font()).value<QFont>();
    textEdit->setFont(font);
    resize(size);
    s->endGroup();

    if (!text.isNull()) {
        textEdit->setPlainText(text);
    }

    connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(editFontButton,SIGNAL(clicked()),this,SLOT(fontDialog()));

    setReadOnly(true);
}

/*!
 \brief Show a dialog to edit the font of the texteditor

*/
void TextViewer::fontDialog(){

    bool ok;
    QFont font = QFontDialog::getFont(&ok, textEdit->font(), this);

    if( ok ){
        textEdit->setFont(font);
        s->beginGroup("TextViewer");
        s->setValue("font",font);
        s->endGroup();
        s->sync();
    }
}

/*!
 \brief Destructor

*/
TextViewer::~TextViewer(){

    s->beginGroup("TextViewer");
    s->setValue("size",size());
    s->setValue("font",textEdit->font());
    s->endGroup();
    s->sync();
    delete s;
}

/*!
 \brief Returns if the texteditor is read-only or not

 \return bool
*/
bool TextViewer::readOnly() const{
    return textEdit->isReadOnly();
}

/*!
 \brief Hide or show the cancelbutton in the dialog

 \param enable
*/
void TextViewer::enableCancelButton( bool enable ){
    cancelButton->setHidden(!enable);
}

/*!
 \brief Return if the cancel button is enabled

 \return bool
*/
bool TextViewer::cancelButtonenabled() const{
    return cancelButton->isVisible();
}

/*!
 \brief Set the rad-only status of the texteditor

 \param enable
*/
void TextViewer::setReadOnly( bool enable ){
    textEdit->setReadOnly(enable);
}

/*!
 \brief Set specified text in the texteditor

 \param text
*/
void TextViewer::setText(const QString& text) {
    textEdit->setPlainText(text);
}

void TextViewer::setLabelText(const QString& text) {
    label_->setText(text);
}

/*!
 \brief return the text in the texteditor

 \return QString
*/
QString TextViewer::text() {
    return textEdit->toPlainText();
}
