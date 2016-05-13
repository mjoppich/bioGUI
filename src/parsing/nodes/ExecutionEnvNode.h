//
// Created by joppich on 5/13/16.
//

#ifndef BIOGUI_EXECUTIONENVNODE_H
#define BIOGUI_EXECUTIONENVNODE_H

#include <QNetworkInterface>
#include <QAbstractSocket>
#include "ExecutionNode.h"

class ExecutionEnvNode : public ExecutionNode {


public:
    ExecutionEnvNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {

        m_sGet = this->getDomElementAttribute(pElement, "GET", "");


    }


    QHostAddress getIPaddress( QAbstractSocket::NetworkLayerProtocol eProtocol )
    {

        QList<QHostAddress> vHostAddresses = QNetworkInterface::allAddresses();
        QList<QHostAddress> vSelHostAddresses;

        for(int i=0; i < vHostAddresses.count(); ++i)
        {
            if(!vHostAddresses[i].isLoopback())
            if (vHostAddresses[i].protocol() == eProtocol )
                vSelHostAddresses.append( vHostAddresses[i] );

        }

        QList<QHostAddress> vInternetSelHostAddresses;
        for (int i = 0; i < vSelHostAddresses.count(); ++i)
        {



        }

        if (vInternetSelHostAddresses.size() == 0)
        {
            if (vSelHostAddresses.size() != 0)
                return vSelHostAddresses.at(0);
            else
                if (vHostAddresses.size() != 0)
                    return vHostAddresses.at(0);
                else
                    return QHostAddress();

        } else {

            return vInternetSelHostAddresses.at(0);
        }



    }


    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget) {


        std::string sResult;

        if (m_sGet.compare("IP", Qt::CaseInsensitive) == 0)
        {

            return this->getIPaddress(QAbstractSocket::AnyIPProtocol).toString().toStdString();

        }

        if (m_sGet.compare("IPv4", Qt::CaseInsensitive) == 0)
        {

            return this->getIPaddress(QAbstractSocket::IPv4Protocol).toString().toStdString();

        }

        if (m_sGet.compare("IPv6", Qt::CaseInsensitive) == 0)
        {
            return this->getIPaddress(QAbstractSocket::IPv6Protocol).toString().toStdString();
        }


        return sResult;

    }

protected:

    QString m_sGet;

};


#endif //BIOGUI_EXECUTIONENVNODE_H
