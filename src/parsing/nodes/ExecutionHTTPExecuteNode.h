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

    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget, bool bEmitSignal = false)
    {
        std::string sURL = m_sExecLocation;

        if (m_iPort != -1)
        {
            sURL = sURL + ":" + std::to_string(m_iPort);
        }

        sURL = sURL + m_sExecutable;

        QString qsURL( sURL.c_str() );

        QUrlQuery* pPostData = new QUrlQuery();
        std::string sCLArg = this->parseDynamicValues(m_sCLArg, pID2Node, pInputID2Value, pInputID2FunctionWidget);
        QString qsCLArg(sCLArg.c_str());

        if (!m_bPortCLtoPOST)
        {

            QStringList vArgs = qsCLArg.split( QString(m_sDelim.c_str()) );

            for (int i = 0; i < vArgs.size(); ++i)
            {

                std::cout << "argument " << vArgs.at(i).toStdString() << std::endl;

                QString sParamPair = vArgs.at(i);
                QStringList vParamPair = sParamPair.split( "=" );

                // if multiple = in text ... for whatever reason...
                QString sParam = "";
                for (int j = 1; j < vParamPair.size(); ++j)
                {
                    sParam += vParamPair.at(j);
                }

                std::cout << vParamPair.at(0).toStdString() << " " << sParam.toStdString() << std::endl;

                pPostData->addQueryItem( vParamPair.at(0),  sParam);

            }


        } else {

            QStringList oArgs = ProcessLauncher::stringToArguments(qsCLArg.toStdString(), '\"');

            for (int i = 0; i < oArgs.size(); ++i)
            {

                QString sArg = oArgs.at(i);

                if (sArg.startsWith("--"))
                {
                    sArg.remove(0, 2);

                    pPostData->addQueryItem(sArg , "");
                } else if (sArg.startsWith("-"))
                {
                    if (i+1 < oArgs.size())
                    {
                        sArg.remove(0, 1);
                        pPostData->addQueryItem(sArg , oArgs.at(i+1));
                        ++i;
                    }
                } else {
                    LOGERROR("Error parsing POST data!"", Logging::ERROR);");
                    continue;
                }

            }

        }



        QNetworkRequest* pNetRequest = new QNetworkRequest( qsURL );
        pNetRequest->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QNetworkAccessManager* pNetworkAccessManager = new QNetworkAccessManager();
        HTTPExecuteThread* pThread = new HTTPExecuteThread(pNetworkAccessManager, pNetRequest, pPostData);


        pNetworkAccessManager->connect(pNetworkAccessManager, &QNetworkAccessManager::finished, [pThread,pNetworkAccessManager](QNetworkReply* pReply){
            pThread->getExecutionResponse(pReply);

            pNetworkAccessManager->deleteLater();
        });


        this->evaluateChildren(pID2Node, pInputID2Value, pInputID2FunctionWidget, NULL, pThread, false);

        QObject::connect(pThread, &ExecuteThread::executionFinished, [pThread, pID2Node, pInputID2Value, pInputID2FunctionWidget, bEmitSignal, this](){

            this->evaluateChildren(pID2Node, pInputID2Value, pInputID2FunctionWidget, NULL, pThread, true);
            pThread->deleteLater();

            if (bEmitSignal)
                emit finishedExecution();

        });

        pThread->start();

        return "";

    }

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
