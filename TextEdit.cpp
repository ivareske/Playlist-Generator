#include "TextEdit.h"

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent){

}

void TextEdit::focusOutEvent ( QFocusEvent * event ){
    emit editingFinished();
    QTextEdit::focusOutEvent(event);
}
