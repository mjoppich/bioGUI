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
