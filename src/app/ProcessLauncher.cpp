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

#include "ProcessLauncher.h"


ProcessLauncher::ProcessLauncher(QString sProgram, QString sParam, bool bWindowsProcNoHandle, bioGUIapp* pApp)
{
    m_sProgram = sProgram;
    m_sParam = sParam;
    m_bWindowsProcNoHandle = bWindowsProcNoHandle;

    m_pProcess = new QProcess();

    m_pApp = pApp;
}

bool ProcessLauncher::start()
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
        sProgram = this->getWSLPath();//"C:\\Windows\\sysnative\\bash";
        QStringList oProgArgs = ProcessLauncher::stringToArguments(m_sParam.toStdString(), '\"');

        QString sRunSignal = this->getWSLSuffix();

        if (sRunSignal.size() > 0)
        {
            oArgs << sRunSignal;
        }
        oArgs << m_sProgram;

        for (int i = 0; i < oProgArgs.size(); ++i)
        {
            oArgs.append( oProgArgs.at(i) );
        }


    } else {

        sProgram = m_sProgram;

        if (m_sParam.size() > 0)
            oArgs = ProcessLauncher::stringToArguments(m_sParam.toStdString(), '\"');
    }

    for (int i = 0; i < oArgs.size(); ++i)
    {
        std::cerr << i << "\t" << oArgs.at(i).toStdString() << std::endl;
    }

    LOGLVL( sProgram.toStdString(), Logging::ERR );
    LOGLVL( oArgs.join(',').toStdString(), Logging::ERR);

    QProcess* pProcess = m_pProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    pProcess->setProcessEnvironment(env);

#ifdef __APPLE__
    QStringList envlist = env.toStringList();
    envlist.replaceInStrings(QRegularExpression("^(?i)PATH=(.*)"), "PATH=/opt/local/bin:/usr/local/bin:$HOME/bin:\\1");
    pProcess->setEnvironment(envlist);
#endif

    LOGLVL(pProcess->environment().join(", ").toStdString(), Logging::ERR);


    if (m_pApp != NULL)
    {
        m_pApp->addRunningProcess(pProcess);
    }


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

                            LOGERROR( "Exit Code: " + std::to_string(exitCode) );
                            LOGERROR( "Exit Status: " + std::to_string(exitStatus) );
                            LOGERROR( "Error: " + pProcess->errorString().toStdString() );
                            LOGERROR( "Program: " + pProcess->program().toStdString() );

                           for (int i = 0; i < pProcess->arguments().size(); ++i)
                           {
                               std::string sArgument = pProcess->arguments().at(i).toStdString();

                               std::cerr << "Argument " << i << " : " << sArgument << std::endl;

                               LOGERROR( "Argument " + std::to_string(i) + ": " + pProcess->arguments().at(i).toStdString() );

                           }

                           LOGERROR( "Process ID: " + std::to_string(pProcess->processId()) );
                           LOGERROR( "Process: " + std::to_string((uint64_t) pProcess) );

                           emit this->finished();
                       });

    
    QProcessEnvironment sysenv = QProcessEnvironment::systemEnvironment();
    m_pProcess->setProcessEnvironment(sysenv);

    
    m_pProcess->start( sProgram, oArgs, eMode );

    return true;

}
