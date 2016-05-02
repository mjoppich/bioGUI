//
// Created by joppich on 3/31/16.
//

#ifndef BIOGUI_EXECUTIONRUNTHREAD_H
#define BIOGUI_EXECUTIONRUNTHREAD_H

#include <QThread>

class XMLParserWindow;
class ExecutionNetwork;

class ExecutionRunThread : public QThread
{
    Q_OBJECT

public:

    ExecutionRunThread(XMLParserWindow* pWindowParser, ExecutionNetwork* pNetwork);

public slots:

    void startExecution();


protected:

    ExecutionNetwork* m_pNetwork;
    XMLParserWindow* m_pWindowParser;

};


#endif //BIOGUI_EXECUTIONRUNTHREAD_H
