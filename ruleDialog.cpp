#include "ruleDialog.h"



ruleDialog::ruleDialog(QWidget *parent, rule *r){
    setupUi(this); // this sets up GUI

    //ShouldBeTrue->hide();
    setSettings();
    if(r){
        value->setText(r->value);
        valueLabel->setText( "Value (Ex: "+Global::getRuleExample(r->type)+")" );
        ruleComboBox->setCurrentIndex( (int)r->type );
        ShouldBeTrue->setChecked( r->shouldBeTrue );
        CaseSensitive->setChecked( r->caseSensitive );
    }

    // signals/slots mechanism in action
    connect( okButton, SIGNAL( clicked() ), this, SLOT( finito() ) ); 
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) ); 	
    connect( ruleComboBox, SIGNAL( currentIndexChanged ( int ) ), this, SLOT( indexChanged( int ) ) );


}

void ruleDialog::finito(){
    if(value->text().isEmpty()){
        QMessageBox::information(this, "",
                                 "You have to specify a value for the rule",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    int ind = ruleComboBox->currentIndex();
    ruleType t = (ruleType)ind;
    if( t==TAG_YEAR_IS || t==TAG_TRACK_IS || t==AUDIO_BITRATE_IS || t==AUDIO_SAMPLERATE_IS || t==AUDIO_CHANNELS_IS || t==AUDIO_LENGTH_IS ){
        QVector<int> intvals;
        bool ok = Global::checkIntValue( &intvals, value->text() );
        if(!ok){
            QMessageBox::information(this, "",
                                     "Error in specified value. See example",
                                     QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }
    this->accept();
}

void ruleDialog::indexChanged( int ind ){

    valueLabel->setText( "Value (Ex: "+Global::getRuleExample((ruleType)ind)+")" );

}


void ruleDialog::setSettings(){

    QStringList list;
    for(int i=0;i<NUMBEROFRULES;i++){
        list.append( Global::getRuleName( (ruleType)i ) );
    }
    ruleComboBox->addItems( list );
    ShouldBeTrue->setChecked(true);
    CaseSensitive->setChecked(true);

}

rule ruleDialog::getSettings(){

    rule r;
    int ind = ruleComboBox->currentIndex();
    r.type = (ruleType)ind;
    r.value = value->text();
    r.shouldBeTrue = ShouldBeTrue->isChecked();
    r.caseSensitive = CaseSensitive->isChecked();
    return r;

}


void ruleDialog::disableRuleChange(){
    ruleComboBox->setDisabled( true );
}





