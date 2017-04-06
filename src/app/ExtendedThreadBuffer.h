/*
 *  bioGUI - a new universal GUI for bioinformatic applications
 *  Copyright (C) 2016  Markus Joppich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
            m_pParentThread->pThread->deleteLater();
            delete m_pParentThread;
        }

    }

public slots:

    virtual void receiveThreadData()
    {

        if (this->m_pParentThread == NULL)
        {
            LOGERROR("No parent thread given!")
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
