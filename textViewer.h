#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H
 
#include <QtGui>
#include "ui_textViewer.h"
 
 
class TextViewer : public QDialog, private Ui::TextViewer
{
    Q_OBJECT
 
public:

    TextViewer(QWidget *parent = 0, QString *text=0);
	QTextEdit *getTextEdit();
	
public slots:

	void setText( QString text );
	/*void resize( QSize size );
	void setFont( QFont font );
	*/
protected:
	
	
};
 
 
#endif






