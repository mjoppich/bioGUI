//
// Created by joppich on 3/31/16.
//

#ifndef BIOGUI_EXTENDEDBUFFER_H
#define BIOGUI_EXTENDEDBUFFER_H

#include <QProcess>
#include <QObject>
#include <QColor>
#include <iostream>
#include "ExtendedStdBuffer.h"

class ExtendedProcessBuffer : public ExtendedStdBuffer {
    Q_OBJECT
public:

    ExtendedProcessBuffer()
            : ExtendedStdBuffer()
    {
    }

    ExtendedProcessBuffer(QProcess* pProcess, QProcess::ProcessChannel eChannel)
    {

        this->m_pParentProcess = new sParentProcess();
        this->m_pParentProcess->pProcess = pProcess;
        this->m_pParentProcess->eChannel = eChannel;

    }

    ~ExtendedProcessBuffer()
    {

        if (this->m_pParentProcess != NULL)
        {
            delete m_pParentProcess;
        }

    }

public slots:

    virtual void receiveProcData()
    {

        if (this->m_pParentProcess == NULL)
            throw "no parent process given";

        QByteArray oArray;

        switch (this->m_pParentProcess->eChannel)
        {
            default:

            case QProcess::StandardOutput:

                oArray = this->m_pParentProcess->pProcess->readAllStandardOutput();
                break;

            case QProcess::StandardError:

                oArray = this->m_pParentProcess->pProcess->readAllStandardError();
                break;
        }


        QString sString = QString(oArray);

        emit sendText(sString, m_oColor, m_sID);
    }



protected:

    struct sParentProcess
    {
        QProcess* pProcess;
        QProcess::ProcessChannel eChannel;
    };




    sParentProcess* m_pParentProcess = NULL;

};


#endif //BIOGUI_EXTENDEDBUFFER_H
