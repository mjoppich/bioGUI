#ifndef EXTENDEDFILEBUFFER_H
#define EXTENDEDFILEBUFFER_H

#include "ExtendedStdBuffer.h"
#include <src/Logging.h>

#include <QProcess>
#include <iostream>


class ExtendedProcFileBuffer : public QObject, public std::basic_streambuf<char> {
    Q_OBJECT
public:

    ExtendedProcFileBuffer(QProcess* pProcess, QProcess::ProcessChannel eChannel)
    {

        this->m_pParentProcess = new sParentProcess();
        this->m_pParentProcess->pProcess = pProcess;
        this->m_pParentProcess->eChannel = eChannel;

        switch (eChannel)
        {
            default:

            case QProcess::ProcessChannel::StandardOutput:

                QObject::connect( pProcess, &QProcess::readyReadStandardOutput, this, &ExtendedProcFileBuffer::receiveProcData );


                break;
            case QProcess::ProcessChannel::StandardError :

                QObject::connect( pProcess, &QProcess::readyReadStandardError, this, &ExtendedProcFileBuffer::receiveProcData );

                break;

        }

    }

    ~ExtendedProcFileBuffer()
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

        emit sendText(sString);
    }

public slots:
    void transferText(QString sString)
    {
        emit sendText(sString);
    }

signals:
    void sendText(QString sString);

protected:
    virtual int_type overflow(int_type v)
    {
        if (v == '\n')
        {
        }
        return v;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n)
    {

        std::string sStdText(p, n);

        if ((p[n] == 4) || (p[n] == 0))
        {
            sStdText.append("\n");
        }

        QString sText(sStdText.c_str());

        this->transferText(sText);

        return n;
    }


protected:

    struct sParentProcess
    {
        QProcess* pProcess;
        QProcess::ProcessChannel eChannel;
    };




    sParentProcess* m_pParentProcess = NULL;

};


#endif // EXTENDEDFILEBUFFER_H
