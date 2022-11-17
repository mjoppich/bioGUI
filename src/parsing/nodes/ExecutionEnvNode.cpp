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

#include "ExecutionEnvNode.h"

#include <QHostInfo>

#include <src/app/ProcessLauncher.h>
#include <src/bioGUIapp.h>

QHostAddress ExecutionEnvNode::getIPaddress(QAbstractSocket::NetworkLayerProtocol eProtocol)
{




    if (false)//(this->getOS().compare("WINDOWS") == 0)
    {
        // if on windows, call WSL to get IP address from there!
        // ip addr show eth0 | grep -oP ''

        ProcessLauncher* pLauncher = new ProcessLauncher("ip", "addr show eth0", true);
        QString result = pLauncher->startBlocking().trimmed();

        QStringList ipLines = result.split("\n");

        QString searchLine = " inet ";
        QRegExp rx("\\d+(\\.\\d+){3}");

        if (eProtocol == QAbstractSocket::IPv6Protocol )
        {
            QString searchLine = " inet6 ";

            //fe80::215:5dff:fef6:e9be
            QRegExp rx("([a-f0-9:]+:+)+[a-f0-9]+");
        }

        rx.setCaseSensitivity(Qt::CaseInsensitive);
        rx.setPatternSyntax(QRegExp::RegExp);

        QString identifiedAddress=QString::null;

        for (int i =0; i < ipLines.size(); ++i)
        {
            QString line = ipLines.at(i);

            if (line.contains(searchLine))
            {
                int pos = rx.indexIn(result);
                QStringList list = rx.capturedTexts();

                identifiedAddress = list.at(0);
                break;
            }
        }

        if (!identifiedAddress.isNull())
        {
            QHostInfo HI = QHostInfo::fromName(identifiedAddress);
            return HI.addresses().at(0);
        }

    }


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
        // HOST addresses do not start with 172! (these are internal ip addresses!)
        QHostAddress oCurrentHA = vSelHostAddresses.at(i);
        QString hostString = oCurrentHA.toString();
        if (hostString.startsWith("172."))
        {
            continue;
        }

        vInternetSelHostAddresses.append(oCurrentHA);
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

std::string ExecutionEnvNode::evaluate(std::map<std::string, ExecutionNode *> *pID2Node,
                                       std::map<std::string, std::string> *pInputID2Value,
                                       std::map<std::string, WidgetFunctionNode *> *pInputID2FunctionWidget) {


    bool bWSL = this->checkWSL(m_sToWSL, pID2Node, pInputID2Value, pInputID2FunctionWidget);

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
        return (this->getOS().compare("WINDOWS") == 0) ? "True" : "False";
    }

    if (m_sGet.compare("OS", Qt::CaseInsensitive) == 0)
    {
        return this->getOS();
    }

    if (m_sGet.compare("DATADIR", Qt::CaseInsensitive) == 0)
    {

        if (this->getOS().compare("LINUX") == 0)
            return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).toStdString();

        
        QString sQHome;
        if (this->getOS().compare("MAC") == 0)
        {
            sQHome = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
            
            if (sQHome.contains(" "))
            {
                sQHome = QString("/usr/local/share/");
            }
        } else {
            
            // we are on WSL
            ProcessLauncher* pLauncher = new ProcessLauncher("echo", "~", bWSL);
            QString sReturn = pLauncher->startBlocking();
            sQHome = sReturn.trimmed();
        }
        
        // exec in WSL "echo ~"
        std::string sHome = sQHome.toStdString() + "/";

        sHome += ".local/share/" + QCoreApplication::applicationName().toStdString();

        return sHome;

    }

    if (m_sGet.compare("APPDIR", Qt::CaseInsensitive) == 0)
    {

        bioGUIapp* pApp = (bioGUIapp*) qApp;
        return pApp->getAppPath().toStdString();

    }

    return sResult;

}
