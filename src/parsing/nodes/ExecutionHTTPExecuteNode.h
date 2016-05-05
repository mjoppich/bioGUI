#ifndef EXECUTIONHTTPEXECUTENODE_H
#define EXECUTIONHTTPEXECUTENODE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>

#include "ExecutionExecuteNode.h"

class ExecutionHTTPExecuteNode : public ExecutionExecuteNode
{
public:

    ExecutionHTTPExecuteNode(QDomElement* pElement)
        : ExecutionExecuteNode(pElement)
    {

    }


    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, QWidget*>* pInputID2Widget)
    {

        QNetworkAccessManager *pNetworkManager = new QNetworkAccessManager(this);
        QNetworkAccessManager->connect(pNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));



        QString sURL;

        QUrlQuery postData;
        postData.addQueryItem("param1", "string");
        postData.addQueryItem("param2", "string");

        QNetworkRequest oNetRequest( sURL );
        oNetRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        pNetworkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());



        std::string sCLArg = this->getCLArgs(pID2Node, pInputID2Value, pInputID2Widget);
        std::string sProgram = m_sExecLocation + m_sExecutable;

        if (m_bTest)
        {
            sProgram = "/usr/bin/echo " + sProgram;
        }


        ProcessLauncher* pLauncher = new ProcessLauncher(QString(sProgram.c_str()), QString(sCLArg.c_str()), m_bWSL);

        this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, pLauncher->getProcess(), false);

        pLauncher->connect(pLauncher, &ProcessLauncher::finished,
                           [pLauncher, pID2Node, pInputID2Value, pInputID2Widget, this](){

            this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, pLauncher->getProcess(), true);
            pLauncher->deleteLater();

        });


        if (bActuallyRun)
        {

            pLauncher->start( );

        }

        return "";

    }


};

#endif // EXECUTIONHTTPEXECUTENODE_H
