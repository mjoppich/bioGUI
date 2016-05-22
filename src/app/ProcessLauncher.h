#ifndef PROCESSLAUNCHER_H
#define PROCESSLAUNCHER_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include "ExecuteThread.h"

#ifndef __linux
#include <windows.h>

#endif

class ProcessThread : public ExecuteThread
{
Q_OBJECT

public:

    ProcessThread(QString sCMD)
    : ExecuteThread()
    {
        m_sCMD = sCMD;

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
        m_sCMD = sCMD;
    }

protected:

    void execute()
    {

#ifndef __linux

        STARTUPINFO sinfo = {sizeof(sinfo), 0};
        PROCESS_INFORMATION pinfo = {0};

        if (CreateProcess(NULL, (LPSTR)m_sCMD.toStdString().c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sinfo, &pinfo)) {

            std::cerr << "executed: " << m_sCMD.toStdString() << std::endl;
            std::cerr << QThread::currentThreadId() << std::endl;

            WaitForSingleObject (pinfo.hProcess, INFINITE);

            std::cerr << "finished" << std::endl;


        }

#endif


        emit executionFinished();
    }



    QString m_sCMD;

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
            m_pThread = new ProcessThread(m_sProgram + " " + m_sParam);
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
        if (m_bWindowsProcNoHandle)
            return NULL;

        return m_pProcess;
    }

    ExecuteThread* getThread()
    {
        if (m_bWindowsProcNoHandle)
            return m_pThread;

        return NULL;
    }

    bool start()
    {

        if (m_bWindowsProcNoHandle)
        {
            m_pThread->setCMD(m_sProgram + " " + m_sParam);

            this->connect(m_pThread, &ExecuteThread::executionFinished, this, &ProcessLauncher::executionFinished);

            m_pThread->start();

        } else {

            QIODevice::OpenMode eMode = QIODevice::ReadWrite;
            std::cout << "running " << m_sProgram.toStdString() << " " << m_sParam.toStdString() << std::endl;

            QStringList oArgs;
            if (m_sParam.size() > 0)
                oArgs = ProcessLauncher::stringToArguments(m_sParam, '\"');

            for (int i = 0; i < oArgs.size(); ++i)
                std::cerr << oArgs.at(i).toStdString() << std::endl;

            m_pProcess->start( m_sProgram, oArgs, eMode );
            QProcess* pProcess = m_pProcess;

            this->connect(m_pProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                               [pProcess, this](int exitCode, QProcess::ExitStatus exitStatus){

                                   qDebug() << pProcess->error();
                                   qDebug() << pProcess->program();
                                   qDebug() << pProcess->arguments();

                                   std::cout << "finished: " << pProcess << std::endl;

                                   emit this->finished();
                               });

        }


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

    static QStringList stringToArguments(QString sString, char cQuoteChar)
    {
        bool bContainsQuotes = (sString.at(0).toLatin1() == cQuoteChar); //true if the first character is "

        QString sTest("a");
        sTest[0] = cQuoteChar;

        QStringList vTmpList = sString.split(QRegExp(sTest), QString::SkipEmptyParts); // Split by " and make sure you don't have an empty string at the beginning
        QStringList vArgsList;

                foreach (QString s, vTmpList) {
                if (bContainsQuotes) { // If 's' is inside quotes ...
                    vArgsList.append(s); // ... get the whole string
                } else { // If 's' is outside quotes ...
                    vArgsList.append(s.split(" ", QString::SkipEmptyParts)); // ... get the splitted string
                }
                bContainsQuotes = !bContainsQuotes;
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
