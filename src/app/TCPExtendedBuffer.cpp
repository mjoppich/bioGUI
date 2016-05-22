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

    if (!this->listen(oHostAddr, iPort))
    {
        std::cerr << "Error listening on port " <<  std::to_string(iPort) << std::endl;

        throw "no listen possible";
    }

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
