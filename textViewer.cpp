#include "textViewer.h"

 
textViewer::textViewer(QWidget *parent, QString *text){
    setupUi(this); // this sets up GUI 	       	
	if(!text->isNull()){
		textEdit->setText( *text );
	}
}

QTextEdit* textViewer::getTextEdit(){
	return textEdit;
}

void textViewer::setText( QString text ){
	textEdit->setText( text );
}

/*
void textViewer::resize( QSize size ){
	this->setSize(size);
}

void textViewer::setFont( QFont font ){
	textEdit->setFont(font);
}
*/