//
// Created by joppich on 3/31/16.
//

#ifndef BIOGUI_EXECUTIONRUNTHREAD_H
#define BIOGUI_EXECUTIONRUNTHREAD_H

#include <QThread>
#include <QMessageBox>

class XMLParserWindow;
class XMLParserExecution;
class ExecutionNetwork;

class ExecutionRunThread : public QThread
{
    Q_OBJECT

public:

    ExecutionRunThread(XMLParserWindow* pWindowParser, XMLParserExecution* pExecution);

    ~ExecutionRunThread();

public slots:

    void startExecution();

signals:

    void executionFinished();


protected:

    XMLParserExecution* m_pExecutionParser;
    ExecutionNetwork* m_pNetwork;
    XMLParserWindow* m_pWindowParser;

};


#endif //BIOGUI_EXECUTIONRUNTHREAD_H
