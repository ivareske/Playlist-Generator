#include "TextEdit.h"

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent){

}

QString TextEdit::text() const{
    return this->toPlainText();
}


void TextEdit::focusOutEvent ( QFocusEvent * event ){
    QTextEdit::focusOutEvent(event);
    emit editingFinished();
}

void TextEdit::focusInEvent(QFocusEvent *e){
    QTextEdit::focusInEvent(e);
    emit entered();
}
