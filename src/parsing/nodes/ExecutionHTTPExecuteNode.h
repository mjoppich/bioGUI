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

#ifndef EXECUTIONHTTPEXECUTENODE_H
#define EXECUTIONHTTPEXECUTENODE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QThread>
#include <src/app/ExecuteThread.h>
#include "ExecutionExecuteNode.h"

class HTTPExecuteThread : public ExecuteThread
{
public:
    HTTPExecuteThread(QNetworkAccessManager* pManager, QNetworkRequest* pRequest, QUrlQuery* pParams)
    : ExecuteThread()
    {        
        m_pRequest = pRequest;
        m_pParams = pParams;
        m_pNetworkAccessManager = pManager;

    }
    ~HTTPExecuteThread()
    {
        m_pNetworkAccessManager->deleteLater();
    }

    void getExecutionResponse(QNetworkReply* pReply)
    {

        m_vReplies.push_back(pReply);

        std::cerr << "received reply" << std::endl;

        emit readyReadStandardOutput();
        emit executionFinished();
    }

    QByteArray readAllStandardOutput()
    {

        QByteArray oReturn;

        std::vector<QNetworkReply*>::iterator oIt = m_vReplies.begin();

        while( oIt != m_vReplies.end() )
        {

            QNetworkReply* pReply = *oIt;

            QByteArray oReplyData = pReply->readAll();

            oReturn.append(oReplyData);

            oIt = m_vReplies.erase(oIt);

        }

        return oReturn;

    }

    QByteArray readAllStandardError()
    {
        return QByteArray();
    }

    void execute()
    {
        QString sRequestAddress = m_pRequest->url().toString();
        std::string sParams = m_pParams->toString(QUrl::FullyEncoded).toUtf8().toStdString();

        std::cerr << "sending " << sParams << " to " << sRequestAddress.toStdString() << std::endl;

        m_pNetworkAccessManager->post(*m_pRequest, m_pParams->toString(QUrl::FullyEncoded).toUtf8());
    }

protected:


    QNetworkAccessManager* m_pNetworkAccessManager;
    QNetworkRequest* m_pRequest;
    QUrlQuery* m_pParams;

    std::vector<QNetworkReply*> m_vReplies;

};

class ExecutionHTTPExecuteNode : public ExecutionExecuteNode
{
public:

    ExecutionHTTPExecuteNode(QDomElement* pElement)
        : ExecutionExecuteNode(pElement)
    {


        m_iPort = this->getDomElementAttribute(pElement, "port", "-1").toInt();
        m_sDelim = this->getDomElementAttribute(pElement, "delim", "&").toStdString();
        m_bPortCLtoPOST = (this->getDomElementAttribute(pElement, "cl_to_post", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);

    }

    virtual ~ExecutionHTTPExecuteNode()
    {

    }

    virtual std::vector<std::string> inputs()
    {
        std::vector<std::string> vCLArgs = this->referencedInputs(m_sCLArg, NULL, NULL, NULL);
        return vCLArgs;
    }

    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget, bool bEmitSignal = false);

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        ExecutionExecutableNode::addNodeAttributes(vAttributes);

        vAttributes.push_back("PORT");
        vAttributes.push_back("DELIM");
        vAttributes.push_back("CL_TO_POST");

    }

    int m_iPort = -1;
    std::string m_sDelim;
    bool m_bPortCLtoPOST = false;

};

#endif // EXECUTIONHTTPEXECUTENODE_H
