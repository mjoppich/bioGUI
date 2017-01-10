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

#include "TCPExtendedBuffer.h"

QBufferTcpServer::QBufferTcpServer(QString sHost, int iPort, TCPExtendedBuffer* pParent)
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

    m_oHostAddress = oHostAddr;

    this->startListening();

    std::cerr << "listening on port " << iPort << " " << this->isListening() <<  std::endl;

    QTcpServer::connect(this, &QBufferTcpServer::socketReady, m_pParent, &TCPExtendedBuffer::receiveProcData, Qt::DirectConnection);

}

void QBufferTcpServer::incomingConnection(qintptr socketDescripter)
{
    std::cout << "incoming connection" << std::endl;

    QTcpSocket* pSocket = new QTcpSocket();
    m_pCurrentSocket = pSocket;
    pSocket->setSocketDescriptor(socketDescripter);

    QObject::connect(pSocket, &QTcpSocket::readyRead, [this,pSocket] () {
        emit socketReady(pSocket);
    });

}

void QBufferTcpServer::startListening() {
    if (!this->listen(m_oHostAddress, m_iPort))
    {
        std::cerr << "Error listening on port " <<  std::to_string(m_iPort) << std::endl;

        LOGERROR("TRYING TO LISTEN ON OCCUPIED PORT: " + std::to_string(m_iPort));
    }
}
