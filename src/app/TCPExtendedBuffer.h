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

    void startListening();

    ~QBufferTcpServer()
    {
        std::cerr << "no more listening on port " << m_iPort << " " << this->hasPendingConnections() << std::endl;
    }

signals:

    void socketReady(QTcpSocket* pSocket);

protected:

    QString m_sHost;
    QHostAddress m_oHostAddress;
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

        std::cerr << "TCPExtendedBuffer" << std::endl;
        std::cerr << QThread::currentThreadId() << std::endl;

    }

    ~TCPExtendedBuffer()
    {

        //this->stopTransmissions();

        m_pServer->deleteLater();

    }

    virtual void stopTransmissions()
    {
        ExtendedThreadBuffer::stopTransmissions();

        std::cout << "closing TCP buffer on port " << m_iPort << std::endl;

        if (m_pServer->isListening())
            m_pServer->close();
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


            QString sString = QString::fromLatin1(cBuffer, (int) iReadBytes);

            memset(cBuffer, 0, std::min(iReadBytes, iBuffer));

            if (m_bTransmissionAllowed)
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
