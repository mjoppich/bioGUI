#ifndef EXTENDEDTHREADFILEBUFFER_H
#define EXTENDEDTHREADFILEBUFFER_H

#include "ExecuteThread.h"
#include "ExtendedStdBuffer.h"
#include <src/Logging.h>

#include <QProcess>
#include <iostream>


class ExtendedThreadFileBuffer : public QObject, public std::basic_streambuf<char> {
    Q_OBJECT
public:

    ExtendedThreadFileBuffer(ExecuteThread* pThread, QProcess::ProcessChannel eChannel)
    {

        this->m_pParentThread = new sParentThread();
        this->m_pParentThread->pThread = pThread;
        this->m_pParentThread->eChannel = eChannel;

        switch (eChannel)
        {
            default:

            case QProcess::ProcessChannel::StandardOutput:

                QObject::connect( pThread, &ExecuteThread::readyReadStandardOutput, this, &ExtendedThreadFileBuffer::receiveProcData );


                break;
            case QProcess::ProcessChannel::StandardError :

                QObject::connect( pThread, &ExecuteThread::readyReadStandardError, this, &ExtendedThreadFileBuffer::receiveProcData );

                break;

        }
    }

    ~ExtendedThreadFileBuffer()
    {

        if (this->m_pParentThread != NULL)
        {
            delete m_pParentThread;
            m_pParentThread = NULL;
        }

    }
    virtual void stopTransmissions()
    {
        m_bTransmissionAllowed = false;
    }

public slots:

    virtual void receiveProcData()
    {

        if (this->m_pParentThread == NULL)
        {
            LOGERROR("No parent process given!")
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
            emit sendText(sString);
        }

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

    struct sParentThread
    {
        ExecuteThread* pThread;
        QProcess::ProcessChannel eChannel;
    };


    bool m_bTransmissionAllowed = true;

    sParentThread* m_pParentThread = NULL;

};


#endif // EXTENDEDTHREADFILEBUFFER_H
