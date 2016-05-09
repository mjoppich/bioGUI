#ifndef TCPEXTENDEDBUFFER_H
#define TCPEXTENDEDBUFFER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "ExtendedThreadBuffer.h"

class TCPExtendedBuffer;
class QBufferTcpServer : public QTcpServer
{
    Q_OBJECT
public:

    QBufferTcpServer(QString sHost, int iPort, TCPExtendedBuffer* pParent);

    virtual void incomingConnection(qintptr socketDescripter);

    ~QBufferTcpServer()
    {
        std::cerr << "no more listening on port " << m_iPort << " " << this->hasPendingConnections() << std::endl;
    }

signals:

    void socketReady(QTcpSocket* pSocket);

protected:

    QString m_sHost;
    int m_iPort;
    TCPExtendedBuffer* m_pParent;
    QTcpSocket* m_pCurrentSocket;

};



class TCPExtendedBuffer : public ExtendedThreadBuffer
{
    Q_OBJECT
public:
    TCPExtendedBuffer(ExecuteThread* pThread, QString sHost, int iPort)
        : ExtendedThreadBuffer(pThread, QProcess::StandardOutput)
    {
        m_sHost = sHost;
        m_iPort = iPort;

        m_pServer = new QBufferTcpServer(sHost, iPort, this);
    }

    ~TCPExtendedBuffer()
    {

        m_pServer->deleteLater();

    }


    virtual void receiveProcData(QTcpSocket* pSocket)
    {

        std::cerr << "data received on port " << m_iPort << std::endl;

        const uint64_t iBuffer = 256;
        char* cBuffer = (char*) calloc(iBuffer, sizeof(char));

        uint64_t iReadBytes;

        while ((iReadBytes = pSocket->read(cBuffer, iBuffer)) != 0)
        {
            if (iReadBytes == 0)
            {
                std::cerr << "error reading bytes: " << iReadBytes << std::endl;

                return;
            }

            if (iReadBytes == -1)
            {
                std::cerr << "reading from closed socket bytes: " << iReadBytes << std::endl;
            }

            std::cerr << "received bytes: " << iReadBytes << std::endl;


            QString sString = QString(cBuffer);

            memset(cBuffer, 0, std::min(iReadBytes, iBuffer));

            emit sendText(sString, m_oColor, m_sID);
        }

        free(cBuffer);


    }
protected:

    QString m_sHost;
    int m_iPort;

    QBufferTcpServer* m_pServer;

};

#endif // TCPEXTENDEDBUFFER_H
