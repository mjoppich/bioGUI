#include "TCPExtendedBuffer.h"


void QBufferTcpServer::incomingConnection(qintptr socketDescripter)
{
    m_pCurrentSocket = new QTcpSocket();
    m_pCurrentSocket->setSocketDescriptor(socketDescripter);

    QObject::connect(m_pCurrentSocket, &QTcpSocket::readyRead, this, &QBufferTcpServer::parentCallback, Qt::DirectConnection);

}

void QBufferTcpServer::parentCallback()
{

    m_pParent->receiveProcData(m_pCurrentSocket);

}
