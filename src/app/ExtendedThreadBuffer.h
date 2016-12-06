//
// Created by joppich on 5/9/16.
//

#ifndef BIOGUI_EXTENDEDTHREADBUFFER_H
#define BIOGUI_EXTENDEDTHREADBUFFER_H


#include <QProcess>
#include <src/Logging.h>
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
        {
            LOGLVL("No parent thread given!", Logging::ERROR);
            return;
        }

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

        if (m_bTransmissionAllowed)
        {
            emit sendText(sString, m_oColor, m_sID);
        }

    }

    virtual void stopTransmissions()
    {
        m_bTransmissionAllowed = false;
    }

protected:

    struct sParentThread
    {
        ExecuteThread* pThread;
        QProcess::ProcessChannel eChannel;
    };

    sParentThread* m_pParentThread = NULL;
    bool m_bTransmissionAllowed = true;
};


#endif //BIOGUI_EXTENDEDTHREADBUFFER_H
