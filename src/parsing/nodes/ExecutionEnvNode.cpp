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

#include <src/app/ProcessLauncher.h>
#include <src/bioGUIapp.h>

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
        return (this->getOS().compare("WIN") == 0) ? "True" : "False";
    }

    if (m_sGet.compare("OS", Qt::CaseInsensitive) == 0)
    {
        return this->getOS();
    }

    if (m_sGet.compare("DATADIR", Qt::CaseInsensitive) == 0)
    {

        if (!bWSL)
            return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).toStdString();

        // we are on WSL
        ProcessLauncher* pLauncher = new ProcessLauncher("echo", "~", true);
        QString sQHome = pLauncher->startBlocking().trimmed();

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
