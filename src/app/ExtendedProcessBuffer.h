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

#ifndef BIOGUI_EXTENDEDBUFFER_H
#define BIOGUI_EXTENDEDBUFFER_H

#include <QProcess>
#include <QObject>
#include <QColor>
#include <iostream>
#include <src/Logging.h>
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
        {
            LOGERROR("No parent process given!")
            return;
        }


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
