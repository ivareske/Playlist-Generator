#include "StyleSheetEditor.h"

StyleSheetEditor::StyleSheetEditor(const QString& currentStyle, QWidget* parent) : QDialog(parent) {
    setupUi(this);


    styleCombo->addItems(QStyleFactory::keys());
    int ind = styleCombo->findText(currentStyle, Qt::MatchContains);
    styleCombo->setCurrentIndex(ind);
    currentStyle_ = styleCombo->currentText();

    applyButton->setEnabled(false);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyClicked()));
    connect(styleTextEdit, SIGNAL(textChanged()), this, SLOT(styleTextChanged()));
    connect(styleCombo, SIGNAL(activated(QString)), this, SLOT(on_styleCombo_activated(QString)));

}

void StyleSheetEditor::styleTextChanged() {
    applyButton->setEnabled(true);
}

QString StyleSheetEditor::styleSheetText() {
    return styleTextEdit->toPlainText();
}

QString StyleSheetEditor::currentStyle() {
    //return the style choosen in the combobox. If a stylesheet has
    //been applied over a style (on top of for instance plastique),
    //qApp->style() will not return plastique, but QWindowsStyle
    //because a stylesheet is applied.
    return currentStyle_;
}



void StyleSheetEditor::setStyleSheetText(const QString& styleSheet) {
    styleTextEdit->setPlainText(styleSheet);
}


void StyleSheetEditor::on_styleCombo_activated(const QString& styleName) {
    qApp->setStyle(styleName);
    applyButton->setEnabled(false);
    currentStyle_ = styleName;
}

void StyleSheetEditor::applyClicked() {
    qApp->setStyleSheet(styleTextEdit->toPlainText());
    applyButton->setEnabled(false);
}


/*
void StyleSheetEditor::loadStyleSheet(const QString &sheetName){

    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    styleTextEdit->setPlainText(styleSheet);
    qApp->setStyleSheet(styleSheet);
    applyButton->setEnabled(false);
}
*/
