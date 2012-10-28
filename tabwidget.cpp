#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent){

    EditableTabBar* t = new EditableTabBar();

    connect(t, SIGNAL(tabTextChanged(int, const QString &)), this,SLOT(tabTextChanged(int, const QString &)));
    setTabBar(t);

    this->setTabsClosable(true);
    this->setMovable(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenu(const QPoint&)));
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));

    QToolButton *toolb = new QToolButton(this);
    toolb->setCursor(Qt::ArrowCursor);
    toolb->setToolTip("Add tab");
    toolb->setAutoRaise(true);
    toolb->setIcon(QIcon(":/plus.png"));
    this->setCornerWidget(toolb, Qt::TopLeftCorner);
    connect(toolb,SIGNAL(clicked()),this,SLOT(addTab()));
}



void TabWidget::closeTab(int ind){
    if(this->count()>2&&this->isDeletable(ind)){
        QTabWidget::removeTab(ind);
    }
}

void TabWidget::contextMenu(const QPoint &p){
    QMenu* contextMenu = new QMenu("Context Menu", this);


    QAction* addTabAction = new QAction("Add tab", this);
    connect(addTabAction, SIGNAL(triggered()), this, SLOT(addTab()));
    contextMenu->addAction(addTabAction);


    /*
    if(this->count()>1){
        QAction* removeTabAction = new QAction("Remove tab", this);
        connect(removeTabAction, SIGNAL(triggered()), this, SLOT(removeCurrentTab()));
        contextMenu->addAction(removeTabAction);
    }
    */

    const QPoint globalPos = this->mapToGlobal(p);
    contextMenu->exec(globalPos);
}

int TabWidget::addTab(const QString &label){
    CodeEditor *e = new CodeEditor;
    connect(e,SIGNAL(textChanged()),this,SIGNAL(textChanged()));
    int index = QTabWidget::addTab(e,label);

    QStringList c;

    QString tip = Global::scriptFunctionsHelpText(&c);
    tip +="\nTag functions:";
    tip += "\nvar tag = new Tag(const QString &fileName)";c<<"Tag";
    tip += "bool tagOk() const;\n";
    tip += "bool audioPropertiesOk() const;\n";
    tip += "QString fileName() const;\n";
    tip += "QString artist() const;\n";
    tip += "QString title() const;\n";
    tip += "QString album() const;\n";
    tip += "QString comment() const;\n";
    tip += "QString genre() const;\n";
    tip += "uint year() const;\n";
    tip += "uint track() const;\n";
    tip += "uint length() const;\n";
    tip += "uint bitRate() const;\n";
    tip += "uint sampleRate() const;\n";
    tip += "uint channels() const;\n";
    tip += "QHash<QString,QStringList> xiphFrames() const;\n";c<<"xiphFrames";
    tip += "QHash<QString,QStringList> ID3v2Frames() const;\n";c<<"ID3v2Frames";
    tip += "QHash<QString,QStringList> APEItems() const;\n";c<<"APEItems";
    tip += "QHash<QString,QStringList> MP4Items() const;\n";c<<"MP4Items";
    tip += "QHash<QString,QStringList> ASFAttributes() const;";c<<"ASFAttributes";

    e->setToolTip(tip);
    e->addCompletionWords(c);

    return index;
}

void TabWidget::removeCurrentTab(){
    this->removeTab(this->currentIndex());
}

void TabWidget::removeAllTabs(){
    for(int i=this->count();i>=0;i--){
        this->removeTab(i);
    }
}

QString TabWidget::commonScript() const{
    QString text_;
    for(int i=0;i<this->count();i++){
        if(!isDeletable(i)){
            text_=this->text(i);
            break;
        }
    }
    return text_;
}

QList<QPair<QString, QString> > TabWidget::scripts() const{
    QList< QPair<QString,QString> > s;
    for(int i=0;i<this->count();i++){
        if(!isDeletable(i)){
            continue;
        }
        QPair<QString,QString> p;
        p.first=this->tabText(i);
        p.second=this->text(i);
        s.append(p);
    }
    return s;
}

CodeEditor* TabWidget::codeEditor(int ind) const{
    if(ind==-1){
        ind=this->currentIndex();
    }
    CodeEditor *e=0;
    QWidget *w = this->widget(ind);
    if(w!=0){
        e=qobject_cast<CodeEditor*>(w);
    }
    return e;
}

QString TabWidget::text(int ind) const{
    if(ind==-1){
        ind=this->currentIndex();
    }
    CodeEditor *e = codeEditor(ind);
    QString script;
    if(e!=0){
        script = e->toPlainText();
    }
    return script;
}

void TabWidget::setText(const QString &text,int ind){
    if(ind==-1){
        ind=this->currentIndex();
    }
    CodeEditor *e = codeEditor(ind);
    if(e){
        e->setPlainText(text);
    }
}



void TabWidget::setIsDeletable(int ind,bool isDeletable){
    CodeEditor *e = this->codeEditor(ind);
    if(e==0){
        return;
    }
    e->setUserData("isDeletable",isDeletable);
}

bool TabWidget::isDeletable(int ind,bool default_) const{
    CodeEditor *e = this->codeEditor(ind);
    if(e==0){
        return default_;
    }
    QVariant v = e->userData().value("isDeletable");
    if(v.canConvert(QVariant::Bool)){
        return v.toBool();
    }else{
        return default_;
    }
}

QString TabWidget::tabText(int ind) const{
    if(ind==-1){
        ind=this->currentIndex();
    }
    return QTabWidget::tabText(ind);
}


void TabWidget::tabTextChanged(int index, const QString &text){
    this->setTabText(index,text);
}
