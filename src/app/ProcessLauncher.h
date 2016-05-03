#ifndef PROCESSLAUNCHER_H
#define PROCESSLAUNCHER_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <iostream>
#include <QDebug>

#ifndef __linux
#include <windows.h>
#endif

class QProcessThread : public QThread
{
Q_OBJECT

public:

    QProcessThread(QString sCMD)
    : QThread()
    {
        m_sCMD = sCMD;

    }

signals:

    void executionFinished();


protected:

    void run()
    {

#ifndef __linux

        STARTUPINFO sinfo = {sizeof(sinfo), 0};
        PROCESS_INFORMATION pinfo = {0};

        if (CreateProcess(NULL, (LPSTR)m_sCMD.toStdString().c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sinfo, &pinfo)) {

            std::cerr << "executed: " << m_sCMD.toStdString() << std::endl;

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

    bool start()
    {

        if (m_bWindowsProcNoHandle)
        {

            m_pThread = new QProcessThread(m_sProgram + " " + m_sParam);

            //this->connect(m_pThread, &QThread::started, m_pThread, &QProcessThread::startExecution);
            this->connect(m_pThread, &QProcessThread::executionFinished, this, &ProcessLauncher::executionFinished);

            m_pThread->start();

        } else {

            QIODevice::OpenMode eMode = QIODevice::ReadWrite;
            std::cout << "running " << m_sProgram.toStdString() << " " << m_sParam.toStdString() << std::endl;

            QStringList oArgs;
            if (m_sParam.size() > 0)
                oArgs = this->stringToArguments(m_sParam, '\"');

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
            delete m_pThread;

        if (m_pProcess != NULL)
            delete m_pProcess;


        emit finished();
    }


    protected:

    QStringList stringToArguments(QString sString, char cQuoteChar)
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


    QProcess* m_pProcess = NULL;
    QProcessThread* m_pThread = NULL;
    bool m_bWindowsProcNoHandle = false;
    QString m_sProgram;
    QString m_sParam;

};

#endif // PROCESSLAUNCHER_H
