//
// Created by joppich on 5/9/16.
//

#ifndef BIOGUI_EXTENDEDTHREADBUFFER_H
#define BIOGUI_EXTENDEDTHREADBUFFER_H


#include <QProcess>
#include "ExtendedStdBuffer.h"
#include "ExecuteThread.h"

class ExtendedThreadBuffer : public ExtendedStdBuffer {
    Q_OBJECT
public:

    ExtendedThreadBuffer(ExecuteThread* pThread, QProcess::ProcessChannel eChannel)
            : ExtendedStdBuffer()
    {

        this->m_pParentThread = new sParentThread();
        this->m_pParentThread->pThread = pThread;
        this->m_pParentThread->eChannel = eChannel;

    }

    ~ExtendedThreadBuffer()
    {

        if (this->m_pParentThread != NULL)
        {
            delete m_pParentThread;
        }

    }

public slots:

    virtual void receiveThreadData()
    {

        if (this->m_pParentThread == NULL)
            throw "no parent process given";

        QByteArray oArray;

        switch (this->m_pParentThread->eChannel)
        {
            default:

            case QProcess::StandardOutput:

                oArray = this->m_pParentThread->pThread->readAllStandardOutput();
                break;

            case QProcess::StandardError:

                oArray = this->m_pParentThread->pThread->readAllStandardError();
                break;
        }


        QString sString = QString(oArray);

        emit sendText(sString, m_oColor, m_sID);
    }



protected:

    struct sParentThread
    {
        ExecuteThread* pThread;
        QProcess::ProcessChannel eChannel;
    };




    sParentThread* m_pParentThread = NULL;
};


#endif //BIOGUI_EXTENDEDTHREADBUFFER_H
