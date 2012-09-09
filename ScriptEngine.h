#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <QScriptEngine>
#include "ScriptWrappers.h"

class ScriptEngine : public QScriptEngine{
        Q_OBJECT
    public:
        explicit ScriptEngine(QObject *parent = 0);
        
    signals:
        
    public slots:
        
};

#endif // SCRIPTENGINE_H
