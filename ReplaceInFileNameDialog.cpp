#include "ReplaceInFileNameDialog.h"


/*!
 \brief

 \param files
 \param parent
*/
ReplaceInFileNameDialog::ReplaceInFileNameDialog(QWidget* parent) : QDialog(parent) {
    setupUi(this); // this sets up GUI

    frame = new QFrame(this);
    frame->setLayout(new QVBoxLayout);
    scrollArea->setWidget(frame);


    // signals/slots mechanism in action
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(AddButton, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(this, SIGNAL(finished(int)), this, SLOT(onFinish(int)));


    settings = Global::guiSettings();
    QHash<QString,QVariant> replace = settings->value("ReplaceInFileName").toHash();
    QStringList keys = replace.keys();
    for(int i=0;i<keys.size();i++){
        addEntry(keys[i],replace[keys[i]].toString());
    }

}

ReplaceInFileNameDialog::~ReplaceInFileNameDialog(){
    delete frame;
}

void ReplaceInFileNameDialog::addEntry( const QString &from, const QString &to, QWidget *parent ){
    ReplacementBox *box = new ReplacementBox(from,to,parent);
    frame->layout()->addWidget(box);
}


void ReplaceInFileNameDialog::onFinish( int result ){


    if(result==QDialog::Accepted){
        QHash<QString,QVariant> replace;
        QList<ReplacementBox*> boxes = frame->findChildren<ReplacementBox*>();
        for(int i=0;i<replace.size();i++){
            replace.insert(boxes[i]->from(),boxes[i]->to());
        }
        settings->setValue("ReplaceInFileName",replace);
    }
    delete settings;



}
