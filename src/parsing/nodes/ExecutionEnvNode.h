#ifndef EXECUTIONENVNODE_H
#define EXECUTIONENVNODE_H

#include <QHostAddress>
#include <QNetworkInterface>

class ExecutionEnvNode
{
public:
    ExecutionEnvNode();

    QString getIPAddress()
    {

        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            {



                 qDebug() << address.toString();
            }
        }


    }
};

#endif // EXECUTIONENVNODE_H
