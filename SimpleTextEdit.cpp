#include "SimpleTextEdit.h"

SimpleTextEdit::SimpleTextEdit(QWidget *parent) : QTextEdit(parent){

}

QString SimpleTextEdit::text() const{
    return this->toPlainText();
}


void SimpleTextEdit::focusOutEvent ( QFocusEvent * event ){
    QTextEdit::focusOutEvent(event);
    emit editingFinished();
}

void SimpleTextEdit::focusInEvent(QFocusEvent *e){
    QTextEdit::focusInEvent(e);
    emit entered();
}
