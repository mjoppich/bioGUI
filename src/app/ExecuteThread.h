//
// Created by mjopp on 09.05.2016.
//

#ifndef BIOGUI_EXECUTETHREAD_H
#define BIOGUI_EXECUTETHREAD_H

#include <QThread>

#include <iostream>


class ExecuteThread : public QThread
{
    Q_OBJECT

public:

    ExecuteThread() : QThread()
    {
    }

    virtual ~ExecuteThread()
    {

        std::cerr << QThread::currentThreadId() << " destroyed" << std::endl;

    }


    virtual QByteArray readAllStandardOutput() = 0;
    virtual QByteArray readAllStandardError() = 0;

public slots:

    virtual void startExecution() //QPrivateSignal* pSignal
    {
        this->execute();
    }

    signals:

    void threadRunned();
    void executionFinished();

    void readyReadStandardOutput();
    void readyReadStandardError();


protected:

    virtual void run()
    {
        this->connect(this, &ExecuteThread::threadRunned, this, &ExecuteThread::startExecution, Qt::DirectConnection);

        emit threadRunned();

        this->exec();
    }

    virtual void execute() = 0;

    //ExtendedThreadBuffer* m_pSTDBuffer;
    //ExtendedThreadBuffer* m_pERRBuffer;


};


#endif //BIOGUI_EXECUTETHREAD_H
