#include "ruleDialog.h"



ruleDialog::ruleDialog( rule *r, QWidget *parent ) : QDialog(parent){
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

void ruleDialog::finito(){
    if(value->text().isEmpty()){
        QMessageBox::information(this, "",
                                 "You have to specify a value for the rule",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    int ind = ruleComboBox->currentIndex();
    rule::ruleType t = static_cast<rule::ruleType>(ind);
    if( t==rule::TAG_YEAR_IS || t==rule::TAG_TRACK_IS || t==rule::AUDIO_BITRATE_IS || \
            t==rule::AUDIO_SAMPLERATE_IS || t==rule::AUDIO_CHANNELS_IS || t==rule::AUDIO_LENGTH_IS ){
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

    valueLabel->setText( "Value (Ex: "+Global::getRuleExample(static_cast<rule::ruleType>(ind))+")" );

}


void ruleDialog::setSettings(){

    QStringList list;
    for(int i=0;i<rule::NUMBEROFRULES;i++){
        list.append( Global::getRuleName( static_cast<rule::ruleType>(i) ) );
    }
    ruleComboBox->addItems( list );
    ShouldBeTrue->setChecked(true);
    CaseSensitive->setChecked(true);

}

rule ruleDialog::getSettings(){

    rule r;
    int ind = ruleComboBox->currentIndex();
    r.setType(static_cast<rule::ruleType>(ind));
    r.setValue(value->text());
    r.setShouldBeTrue(ShouldBeTrue->isChecked());
    r.setCaseSensitive(CaseSensitive->isChecked());
    return r;

}


void ruleDialog::disableRuleChange(){
    ruleComboBox->setDisabled( true );
}





