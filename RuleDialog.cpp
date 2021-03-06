#include "RuleDialog.h"



/*!
 \brief

 \param r
 \param parent
*/
RuleDialog::RuleDialog(const Rule& r, QWidget* parent) : QDialog(parent) {
    setupUi(this); // this sets up GUI

    //ShouldBeTrue->hide();
    setSettings();
    if (r.type() != Rule::UNKNOWN) {
        value->setText(r.value());
        valueLabel->setText("Value (Ex: " + getRuleExample(r.type()) + ")");
        int ind = 0;
        for (int i = 0; i < ruleComboBox->count(); i++) {
            if (ruleComboBox->itemData(i).toInt() == r.type()) {
                ind = i;
                break;
            }
        }
        ruleComboBox->setCurrentIndex(ind);
        ShouldBeTrue->setChecked(r.shouldBeTrue());
        CaseSensitive->setChecked(r.caseSensitive());
    }

    // signals/slots mechanism in action
    connect(okButton, SIGNAL(clicked()), this, SLOT(finito()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ruleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));


}

/*!
 \brief

*/
void RuleDialog::hideValueFrame() {
    valueFrame->hide();
}


/*!
 \brief

*/
void RuleDialog::finito() {
    if (value->text().isEmpty()) {
        QMessageBox::information(this, "",
                                 "You have to specify a value for the Rule",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    int ind = ruleComboBox->currentIndex();
    Rule::RuleType t = static_cast<Rule::RuleType>(ruleComboBox->itemData(ind).toInt());
    if (t == Rule::TAG_YEAR_IS || t == Rule::TAG_TRACK_IS || t == Rule::AUDIO_BITRATE_IS || \
        t == Rule::AUDIO_SAMPLERATE_IS || t == Rule::AUDIO_CHANNELS_IS || t == Rule::AUDIO_LENGTH_IS) {
        QVector<int> intvals;
        bool ok = Global::checkIntValue(&intvals, value->text());
        if (!ok) {
            QMessageBox::information(this, "",
                                     "Error in specified value. See example",
                                     QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }
    this->accept();
}

/*!
 \brief

 \param ind
*/
void RuleDialog::indexChanged(int ind) {

    valueLabel->setText("Value (Ex: " + getRuleExample(static_cast<Rule::RuleType>(ind)) + ")");

}


/*!
 \brief

*/
void RuleDialog::setSettings() {

    for (int i = 0; i < Rule::NUMBEROFRULES; i++) {
        Rule::RuleType type = static_cast<Rule::RuleType>(i);
        if (type != Rule::UNKNOWN) {
            ruleComboBox->addItem(Rule::getRuleName(type), type);
        }
    }
    ShouldBeTrue->setChecked(true);
    CaseSensitive->setChecked(true);

}

/*!
 \brief

 \return Rule
*/
Rule RuleDialog::getRule() {

    Rule r;
    int ind = ruleComboBox->currentIndex();
    r.setType(static_cast<Rule::RuleType>(ruleComboBox->itemData(ind).toInt()));
    r.setValue(value->text());
    r.setShouldBeTrue(ShouldBeTrue->isChecked());
    r.setCaseSensitive(CaseSensitive->isChecked());
    return r;

}


/*!
 \brief

*/
void RuleDialog::disableRuleChange() {
    ruleComboBox->setDisabled(true);
}


/*!
 \brief

 \param type
 \return QString
*/
QString RuleDialog::getRuleExample(const Rule::RuleType& type) {


    QString ans = "";
    if (type == Rule::FILENAME_CONTAINS || type == Rule::FILENAME_EQUALS) {
        ans = "pantera";
    }
    else if (type == Rule::TAG_TITLE_CONTAINS || type == Rule::TAG_TITLE_EQUALS) {
        ans = "acdc";
    }
    else if (type == Rule::TAG_ARTIST_CONTAINS || type == Rule::TAG_ARTIST_EQUALS) {
        ans = "acdc";
    }
    else if (type == Rule::TAG_ALBUM_CONTAINS || type == Rule::TAG_ALBUM_EQUALS) {
        ans = "live";
    }
    else if (type == Rule::TAG_YEAR_IS) {
        ans = "Single nr.: '2001', or a range: '1990-1998'";
    }
    else if (type == Rule::TAG_COMMENT_CONTAINS || type == Rule::TAG_COMMENT_EQUALS) {
        ans = "whatever...";
    }
    else if (type == Rule::TAG_TRACK_IS) {
        ans = "Single nr.: '1', or a range: '7-12'";
    }
    else if (type == Rule::TAG_GENRE_CONTAINS || type == Rule::TAG_GENRE_EQUALS) {
        ans = "'rock'";
    }
    else if (type == Rule::AUDIO_BITRATE_IS) {
        ans = "Single nr.: '128', or a range: '128-196'";
    }
    else if (type == Rule::AUDIO_SAMPLERATE_IS) {
        ans = "Single nr.: '44100', or a range: '44100-9999999'";
    }
    else if (type == Rule::AUDIO_CHANNELS_IS) {
        ans = "Single nr.: '2', or a range: '2-6'";
    }
    else if (type == Rule::AUDIO_LENGTH_IS) {
        ans = "Single nr.: '300', or a range: '500-700' (use seconds) ";
    }

    return ans;

}



