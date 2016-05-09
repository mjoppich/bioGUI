//
// Created by mjopp on 09.05.2016.
//

#ifndef BIOGUI_EXECUTETHREAD_H
#define BIOGUI_EXECUTETHREAD_H

#include <QThread>

class ExecuteThread : public QThread
{
    Q_OBJECT

public:

    ExecuteThread() : QThread()
    {

    }


    virtual QByteArray readAllStandardOutput() = 0;
    virtual QByteArray readAllStandardError() = 0;

public slots:

    virtual void startExecution(QPrivateSignal* pSignal)
    {
        this->execute();
    }

    signals:

    void executionFinished();

    void readyReadStandardOutput();
    void readyReadStandardError();


protected:

    virtual void execute() = 0;

};


#endif //BIOGUI_EXECUTETHREAD_H
