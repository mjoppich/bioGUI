#ifndef EXECUTIONHTTPEXECUTENODE_H
#define EXECUTIONHTTPEXECUTENODE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QThread>
#include <src/app/ExecuteThread.h>

#include "ExecutionExecuteNode.h"

class HTTPExecuteThread : public ExecuteThread
{
public:
    HTTPExecuteThread(QNetworkAccessManager* pNetworkAccessManager, QNetworkRequest* pRequest, QUrlQuery* pParams)
    : ExecuteThread()
    {
        m_pNetworkAccessManager = pNetworkAccessManager;
        m_pRequest = pRequest;
        m_pParams = pParams;


        m_pNetworkAccessManager->connect(m_pNetworkAccessManager, &QNetworkAccessManager::finished(QNetworkReply*), [this](QNetworkReply* pReply){
            this->getExecutionResponse(pReply);
        });

        this->connect(this, &QThread::started, this, &ExecuteThread::startExecution);

    }

    void getExecutionResponse(QNetworkReply* pReply)
    {

        emit executionFinished();
    }

    void execute()
    {
        m_pNetworkAccessManager->post(*m_pRequest, m_pParams->toString(QUrl::FullyEncoded).toUtf8());
    }

protected:


    QNetworkAccessManager* m_pNetworkAccessManager;
    QNetworkRequest* m_pRequest;
    QUrlQuery* m_pParams;

};

class ExecutionHTTPExecuteNode : public ExecutionExecuteNode
{
public:

    ExecutionHTTPExecuteNode(QDomElement* pElement)
        : ExecutionExecuteNode(pElement)
    {


        m_iPort = this->getDomElementAttribute(pElement, "port", "-1").toInt();

    }


    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, QWidget*>* pInputID2Widget)
    {

        QNetworkAccessManager *pNetworkManager = new QNetworkAccessManager();

        std::string sURL = m_sExecLocation;

        if (m_iPort != -1)
        {
            sURL = sURL + ":" + std::to_string(m_iPort);
        }

        sURL = sURL + m_sExecutable;

        QString qsURL( sURL.c_str() );

        QUrlQuery postData;

        // TODO split arguments by " -" first, (append - to each element) and then again by " " to get param and string.
        postData.addQueryItem("param1", "string");
        postData.addQueryItem("param2", "string");

        QNetworkRequest oNetRequest( qsURL );
        oNetRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        HTTPExecuteThread* pThread = new HTTPExecuteThread(pNetworkManager, &oNetRequest, &postData);

        this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, NULL, pThread, false);

        pThread->start();

        QObject::connect(pThread, &ExecuteThread::executionFinished, [pThread, pID2Node, pInputID2Value, pInputID2Widget, this](){

            this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, NULL, pThread, true);
            pNetworkManager->deleteLater();

        });

        return "";

    }

protected:

    int m_iPort = -1;

};

#endif // EXECUTIONHTTPEXECUTENODE_H
