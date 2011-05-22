#include "TextViewer.h"

 
TextViewer::TextViewer(QWidget *parent, QString *text){
    setupUi(this); // this sets up GUI 	       	
	if(!text->isNull()){
		textEdit->setText( *text );
	}
}

QTextEdit* TextViewer::getTextEdit(){
	return textEdit;
}

void TextViewer::setText( QString text ){
	textEdit->setText( text );
}

/*
void TextViewer::resize( QSize size ){
	this->setSize(size);
}

void TextViewer::setFont( QFont font ){
	textEdit->setFont(font);
}
*/