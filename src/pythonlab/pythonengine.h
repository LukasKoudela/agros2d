#ifndef PYTHONENGINE_H
#define PYTHONENGINE_H

#include "util.h"
#include <Python.h>

struct PythonVariable
{
    QString name;
    QString type;
    QVariant value;
};

class PythonEngine : public QObject
{
    Q_OBJECT

signals:
    void pythonClear();
    void pythonShowMessage(const QString &);
    void pythonShowHtml(const QString &);
    void pythonShowImage(const QString &);

    void executedExpression();
    void executedScript();

public:
    PythonEngine() {}
    ~PythonEngine();

    void init();

    // python commands
    void pythonClearCommand();
    void pythonShowMessageCommand(const QString &message);
    void pythonShowHtmlCommand(const QString &fileName);
    void pythonShowImageCommand(const QString &fileName);

    ScriptResult runPythonScript(const QString &script, const QString &fileName = "");
    ExpressionResult runPythonExpression(const QString &expression, bool returnValue);
    ScriptResult parseError();
    inline bool isRunning() { return m_isRunning; }

    void deleteUserModules();
    QStringList codeCompletion(const QString& code, int offset, const QString& fileName = "");
    QStringList codePyFlakes(const QString& fileName);
    QList<PythonVariable> variableList();

protected:
    PyObject *m_dict;
    bool m_isRunning;

    virtual void addCustomExtensions() {}
    virtual void runPythonHeader() {}

    PyObject *runCode(const QString &code, bool returnOutput = false);

private slots:
    void stdOut(const QString &message);

private:
    QString m_stdOut;
    QString m_functions;

    QMutex muttex;
};

// create custom python engine
void createPythonEngine(PythonEngine *custom = NULL);
// current python engine
PythonEngine *currentPythonEngine();

#endif // PYTHONENGINE_H
