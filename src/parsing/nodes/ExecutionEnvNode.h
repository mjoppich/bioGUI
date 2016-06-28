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

    std::string getOS()
    {

        if (QSysInfo::windowsVersion() != QSysInfo::WV_None)
            return "WINDOWS";

        if (QSysInfo::macVersion() != QSysInfo::MV_None)
            return "MAC";


        return "LINUX";
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

        if (m_sGet.compare("LINUX", Qt::CaseInsensitive) == 0)
        {

            return (this->getOS().compare("LINUX") == 0) ? "True" : "False";

        }

        if (m_sGet.compare("UNIX", Qt::CaseInsensitive) == 0)
        {
            return ((this->getOS().compare("LINUX") == 0) || (this->getOS().compare("MAC") == 0)) ? "True" : "False";

        }

        if (m_sGet.compare("MAC", Qt::CaseInsensitive) == 0)
        {
            return (this->getOS().compare("MAC") == 0) ? "True" : "False";
        }

        if (m_sGet.compare("WIN", Qt::CaseInsensitive) == 0)
        {
            return (this->getOS().compare("WIN") == 0) ? "True" : "False";
        }

        if (m_sGet.compare("OS", Qt::CaseInsensitive) == 0)
        {
            return this->getOS();
        }

        return sResult;

    }

protected:

    QString m_sGet;

};


#endif //BIOGUI_EXECUTIONENVNODE_H
