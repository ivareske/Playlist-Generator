#include "ScriptEngine.h"

ScriptEngine::ScriptEngine(QObject *parent) : QScriptEngine(parent){

    globalObject().setProperty("getDirContent",newFunction(ScriptWrappers::scriptGetDirContent));
    globalObject().setProperty("copyFiles",newFunction(ScriptWrappers::scriptCopyFiles));
    globalObject().setProperty("writeFile",newFunction(ScriptWrappers::writeFile));
    globalObject().setProperty("contains",newFunction(ScriptWrappers::scriptContains));
    globalObject().setProperty("unique",newFunction(ScriptWrappers::scriptUnique));
    globalObject().setProperty("shuffle",newFunction(ScriptWrappers::scriptRandomize));
    globalObject().setProperty("relativeTo",newFunction(ScriptWrappers::scriptRelativeTo));

}
