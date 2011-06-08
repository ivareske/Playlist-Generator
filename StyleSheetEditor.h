
#ifndef STYLESHEETEDITOR_H
#define STYLESHEETEDITOR_H

#include <QtGui>
#include "ui_StyleSheetEditor.h"

class StyleSheetEditor : public QDialog, private Ui::StyleSheetEditor {
        Q_OBJECT

    public:
        StyleSheetEditor(const QString& currentStyle, QWidget* parent = 0);

        QString styleSheetText();
        QString currentStyle();
        void setStyleSheetText(const QString& styleSheet);
    private slots:
        void on_styleCombo_activated(const QString& styleName);
        void applyClicked();
        void styleTextChanged();

    private:
        QString currentStyle_;
        //void loadStyleSheet(const QString &sheetName);

};

#endif
