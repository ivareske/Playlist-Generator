#include "RuleDialog.h"



RuleDialog::RuleDialog( Rule *r, QWidget *parent ) : QDialog(parent){
    setupUi(this); // this sets up GUI

    //ShouldBeTrue->hide();
    setSettings();
    if(r){
        value->setText(r->value());
        valueLabel->setText( "Value (Ex: "+Global::getRuleExample(r->type())+")" );
        ruleComboBox->setCurrentIndex( r->type() );
        ShouldBeTrue->setChecked( r->shouldBeTrue() );
        CaseSensitive->setChecked( r->caseSensitive() );
    }

    // signals/slots mechanism in action
    connect( okButton, SIGNAL( clicked() ), this, SLOT( finito() ) ); 
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) ); 	
    connect( ruleComboBox, SIGNAL( currentIndexChanged ( int ) ), this, SLOT( indexChanged( int ) ) );


}

void RuleDialog::finito(){
    if(value->text().isEmpty()){
        QMessageBox::information(this, "",
                                 "You have to specify a value for the Rule",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    int ind = ruleComboBox->currentIndex();
    Rule::ruleType t = static_cast<Rule::ruleType>(ind);
    if( t==Rule::TAG_YEAR_IS || t==Rule::TAG_TRACK_IS || t==Rule::AUDIO_BITRATE_IS || \
            t==Rule::AUDIO_SAMPLERATE_IS || t==Rule::AUDIO_CHANNELS_IS || t==Rule::AUDIO_LENGTH_IS ){
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

void RuleDialog::indexChanged( int ind ){

    valueLabel->setText( "Value (Ex: "+Global::getRuleExample(static_cast<Rule::ruleType>(ind))+")" );

}


void RuleDialog::setSettings(){

    QStringList list;
    for(int i=0;i<Rule::NUMBEROFRULES;i++){
        list.append( Global::getRuleName( static_cast<Rule::ruleType>(i) ) );
    }
    ruleComboBox->addItems( list );
    ShouldBeTrue->setChecked(true);
    CaseSensitive->setChecked(true);

}

Rule RuleDialog::getSettings(){

    Rule r;
    int ind = ruleComboBox->currentIndex();
    r.setType(static_cast<Rule::ruleType>(ind));
    r.setValue(value->text());
    r.setShouldBeTrue(ShouldBeTrue->isChecked());
    r.setCaseSensitive(CaseSensitive->isChecked());
    return r;

}


void RuleDialog::disableRuleChange(){
    ruleComboBox->setDisabled( true );
}





