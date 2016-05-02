#ifndef TCPEXTENDEDBUFFER_H
#define TCPEXTENDEDBUFFER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "ExtendedBuffer.h"

class TCPExtendedBuffer;
class QBufferTcpServer : public QTcpServer
{
    Q_OBJECT
public:

    QBufferTcpServer(QString sHost, int iPort, TCPExtendedBuffer* pParent)
        : QTcpServer()
    {
        m_sHost = sHost;
        m_iPort = iPort;
        m_pParent = pParent;

        QTcpServer::connect(this, &QTcpServer::acceptError, [this] () {
            std::cerr << "conn error: " << this->errorString().toStdString() << std::endl;
        });

        QHostAddress oHostAddr(sHost);
        if ((sHost.size() == 0) || (oHostAddr.isNull()))
            oHostAddr = QHostAddress::Any;

        if (!this->listen(oHostAddr, iPort))
        {
            std::cerr << "Error listening on port " <<  std::to_string(iPort) << std::endl;

            throw "no listen possible";
        }

        std::cerr << "listening on port " << iPort << " " << this->isListening() <<  std::endl;
    }

    virtual void incomingConnection(qintptr socketDescripter);
    virtual void parentCallback();

    ~QBufferTcpServer()
    {
        std::cerr << "no more listening on port " << m_iPort << " " << this->hasPendingConnections() << std::endl;
    }

protected:

    QString m_sHost;
    int m_iPort;
    TCPExtendedBuffer* m_pParent;
    QTcpSocket* m_pCurrentSocket;

};



class TCPExtendedBuffer : public ExtendedBuffer
{
    Q_OBJECT
public:
    TCPExtendedBuffer(QString sHost, int iPort)
        : ExtendedBuffer()
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

        int64_t iReadBytes = pSocket->read(cBuffer, iBuffer);

        if (iReadBytes == 0)
        {
            std::cerr << "error reading bytey: " << iReadBytes << std::endl;

            return;
        }

        if (iReadBytes == -1)
        {
            std::cerr << "reading from closed socket bytes: " << iReadBytes << std::endl;
        }

        QString sString = QString(cBuffer);
        free(cBuffer);
        //sString = QString(sString.toStdString().substr(0, sString.size()-3).c_str());

        emit sendText(sString, m_oColor, m_sID);
    }
protected:

    QString m_sHost;
    int m_iPort;

    QBufferTcpServer* m_pServer;

};

#endif // TCPEXTENDEDBUFFER_H
