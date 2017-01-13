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

#ifndef PROCESSLAUNCHER_H
#define PROCESSLAUNCHER_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include "ExecuteThread.h"
#include <src/Logging.h>

#ifndef __linux
#ifndef __APPLE__
    #include <windows.h>
#endif
#endif

#include <cstdint>
#if INTPTR_MAX == INT32_MAX
    #define __BIOGUI__32
#elif INTPTR_MAX == INT64_MAX
    #define __BIOGUI__64
#endif


class ProcessThread : public ExecuteThread
{
Q_OBJECT

public:

    ProcessThread(QString sLinuxCMD)
    : ExecuteThread()
    {
        m_sLinuxCMD = sLinuxCMD;

    }

    QByteArray readAllStandardOutput()
    {
        return QByteArray();
    }

    QByteArray readAllStandardError()
    {
        return QByteArray();
    }

    void setCMD(QString sCMD)
    {
        m_sLinuxCMD = sCMD;
    }

protected:

    void execute()
    {

        QString sBash;

#ifdef __BIOGUI__32
        sBash = "C:\\Windows\\sysnative\\bash";
#else
        sBash = "bash";
#endif

        //m_sLinuxCMD.replace(QString("\""), QString("\\\""));

        sBash.append( " -c \" "+ m_sLinuxCMD + "\"" );

#ifndef __linux
#ifndef __APPLE__

        STARTUPINFO sinfo = {sizeof(sinfo), 0};
        PROCESS_INFORMATION pinfo = {0};

        if (CreateProcess(NULL, (LPSTR)sBash.toStdString().c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sinfo, &pinfo)) {

            std::cerr << "executed: " << sBash.toStdString() << std::endl;
            std::cerr << QThread::currentThreadId() << std::endl;

            WaitForSingleObject (pinfo.hProcess, INFINITE);

            std::cerr << "finished" << std::endl;


        }
#endif
#endif


        emit executionFinished();
    }



    QString m_sLinuxCMD;
    QString m_sWSLlaunch;

};

class ProcessLauncher : public QObject
{
    Q_OBJECT
public:
    ProcessLauncher(QString sProgram, QString sParam, bool bWindowsProcNoHandle)
    {
        m_sProgram = sProgram;
        m_sParam = sParam;
        m_bWindowsProcNoHandle = bWindowsProcNoHandle;

        if (!m_bWindowsProcNoHandle)
        {
            m_pProcess = new QProcess();
        } else {
            m_pThread = NULL;//new ProcessThread(m_sProgram + " " + m_sParam);
            m_pProcess = new QProcess();
        }
    }

    void setProgram(QString sProg)
    {
        m_sProgram = sProg;
    }

    void setParams(QString sParam)
    {
        m_sParam = sParam;
    }


    /**
     *
     * @return null if this is a WPNH call!
     */
    QProcess* getProcess()
    {
        return m_pProcess;

        if (m_bWindowsProcNoHandle)
            return NULL;

        return m_pProcess;
    }

    ExecuteThread* getThread()
    {
        return NULL;

        if (m_bWindowsProcNoHandle)
            return m_pThread;

        return NULL;
    }

    QString startBlocking(uint32_t iWaitMSec = 10000)
    {
        this->start();

        m_pProcess->waitForFinished( iWaitMSec );

        QString sOut( m_pProcess->readAllStandardOutput() );

        return sOut;
    }

    bool start()
    {

        /*
        if (m_bWindowsProcNoHandle)
        {

            m_pThread->setCMD(m_sProgram + " " + m_sParam);

            this->connect(m_pThread, &ExecuteThread::executionFinished, this, &ProcessLauncher::executionFinished);

            m_pThread->start();

        }
        */
        QIODevice::OpenMode eMode = QIODevice::ReadWrite;

        std::cout << "Running QProcess for: " << m_sProgram.toStdString() << " " << m_sParam.toStdString() << std::endl;
        std::cerr << "Running in WSL mode? " << m_bWindowsProcNoHandle << std::endl;

        QString sProgram;
        QStringList oArgs;

        if (m_bWindowsProcNoHandle)
        {
            sProgram = "C:\\Windows\\sysnative\\bash";
            oArgs << "-ic" << m_sProgram + " " + m_sParam;


        } else {

            sProgram = m_sProgram;

            if (m_sParam.size() > 0)
                oArgs = ProcessLauncher::stringToArguments(m_sParam.toStdString(), '\"');

            for (int i = 0; i < oArgs.size(); ++i)
                std::cerr << oArgs.at(i).toStdString() << std::endl;
        }

        LOGLVL( sProgram.toStdString(), Logging::ERR );
        LOGLVL( oArgs.join(',').toStdString(), Logging::ERR);

        QProcess* pProcess = m_pProcess;

        this->connect(m_pProcess, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
                           [pProcess, this](QProcess::ProcessError errorCode){

                               std::cerr << "Process Errored !" << std::endl;
                               std::cerr << "Process Error Code: " << errorCode << std::endl;

                               std::string sError = pProcess->errorString().toStdString();
                               std::cerr << "Error: " << sError << std::endl;

                               std::string sProgram = pProcess->program().toStdString();
                               std::cerr << "Program: " << sProgram << std::endl;

                               for (int i = 0; i < pProcess->arguments().size(); ++i)
                               {
                                   std::string sArgument = pProcess->arguments().at(i).toStdString();

                                   std::cerr << "Argument " << i << " : " << sArgument << std::endl;
                               }

                               std::cout << "errored: " << pProcess << std::endl;

                               emit this->finished();
                           });

        this->connect(m_pProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                           [pProcess, this](int exitCode, QProcess::ExitStatus exitStatus){

                               std::cerr << "Exit Code: " << exitCode << std::endl;
                               std::cerr << "Exit Status: " << exitStatus << std::endl;

                               std::string sError = pProcess->errorString().toStdString();
                               std::cerr << "Error: " << sError << std::endl;

                               std::string sProgram = pProcess->program().toStdString();
                               std::cerr << "Program: " << sProgram << std::endl;

                               for (int i = 0; i < pProcess->arguments().size(); ++i)
                               {
                                   std::string sArgument = pProcess->arguments().at(i).toStdString();

                                   std::cerr << "Argument " << i << " : " << sArgument << std::endl;
                               }

                               std::cout << "finished: " << pProcess << std::endl;

                               emit this->finished();
                           });

        m_pProcess->start( sProgram, oArgs, eMode );

        return true;

    }

signals:

    void finished();

public slots:

    void executionFinished()
    {
        if (m_pThread != NULL)
            m_pThread->deleteLater();

        if (m_pProcess != NULL)
            m_pProcess->deleteLater();


        emit finished();
    }

    static QStringList stringToArguments(std::string sString, char cQuoteChar)
    {
        bool bWithinQuote = false;
        size_t iQuoteStart = 0;
        size_t iLastArgPartStart = 0;

        QStringList vArgsList;

        // possible quotes are " or ' - one has to be careful with escaped quotes within the string
        for (size_t i = 0; i < sString.size(); ++i)
        {

            if ((sString.at(i) == '\"') || (sString.at(i) == '\''))
            {

                // if escaped -> continue
                if ((i > 0) && (sString.at(i-1) == '\\'))
                    continue;

                if (!bWithinQuote)
                {
                    bWithinQuote = true;
                    iQuoteStart = i;

                } else {

                    if (sString.at(i) == sString.at(iQuoteStart))
                        bWithinQuote = false;
                }

            }

            if (bWithinQuote)
                continue;

            if ((sString.at(i) == ' ') || (sString.size()-1 == i))
            {
                size_t iOffSet = (sString.at(i) == ' ') ? 0 : 1;

                std::string sPart = sString.substr(iLastArgPartStart, i-iLastArgPartStart+iOffSet);
                if ((sPart[0] == sPart[sPart.size()-1]) && ( (sPart[0] == '\"' ) || ( sPart[0] == '\'')))
                    sPart = sPart.substr(1,sPart.size()-2);

                QString sQPart(sPart.c_str());
                sQPart = sQPart.trimmed();

                sQPart.replace("\\\"", "\"");
                sQPart.replace("\\\'", "\'");

                std::cout << sQPart.toStdString() << std::endl;

                //if (sPart.size() > 0)
                vArgsList.append( sQPart );

                iLastArgPartStart = i+1;
            }

        }

        if (bWithinQuote)
        {
            std::string sPart = sString.substr(iLastArgPartStart, -1);
            QString sQPart(sPart.c_str());

            if (sPart.size() > 0)
                vArgsList.append( sQPart );

        }

        return vArgsList;
    }

protected:


    QProcess* m_pProcess = NULL;
    ProcessThread* m_pThread = NULL;
    bool m_bWindowsProcNoHandle = false;
    QString m_sProgram;
    QString m_sParam;

};

#endif // PROCESSLAUNCHER_H
