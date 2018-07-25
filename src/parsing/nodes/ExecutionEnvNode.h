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

#ifndef BIOGUI_EXECUTIONENVNODE_H
#define BIOGUI_EXECUTIONENVNODE_H

#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QtCore/QStandardPaths>
#include <QtCore/QCoreApplication>

#include "ExecutionNode.h"

class ExecutionEnvNode : public ExecutionNode {


public:
    ExecutionEnvNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {

        m_sGet = this->getDomElementAttribute(pElement, "GET", "");
        m_sToWSL = this->getDomElementAttribute(pElement, "wsl", "false");


    }

    virtual ~ExecutionEnvNode()
    {

    }

    virtual std::vector<std::string> inputs()
    {
        std::vector<std::string> vBase;
        vBase.push_back(m_sGet.toStdString());

        return vBase;
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

    std::string getOS(std::map< std::string, ExecutionNode*>* pID2Node = NULL,
                      std::map<std::string, std::string>* pInputID2Value = NULL,
                      std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget = NULL)
    {


        bool bWSL = false;

        if ((pID2Node != NULL) && (pInputID2Value != NULL) && (pInputID2FunctionWidget != NULL))
        {
            bWSL = this->checkWSL(m_sToWSL, pID2Node, pInputID2Value, pInputID2FunctionWidget);
        }

        if (bWSL)
        {
            // TODO make this return WSL?
            return "LINUX";
        }

        if (QSysInfo::windowsVersion() != QSysInfo::WV_None)
            return "WINDOWS";

        if (QSysInfo::macVersion() != QSysInfo::MV_None)
            return "MAC";


        return "LINUX";
    }


    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget);

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("GET");
    }

    QString m_sGet;
    QString m_sToWSL;

};


#endif //BIOGUI_EXECUTIONENVNODE_H
