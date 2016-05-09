//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONEXECUTENODE_H
#define BIOGUI_EXECUTIONEXECUTENODE_H

#include <QDebug>
#include <QThread>
#include <QProcess>
#include <iostream>
#include <QtCore/qprocess.h>
#include <src/app/ProcessLauncher.h>
#include "ExecutionNode.h"
#include "ExecutionOutputNode.h"

#ifdef WINDOWS
    #include <windows.h>
#endif

class HTTPExecuteThread;

class ExecutionExecuteNode : public ExecutionNode {

public:

    ExecutionExecuteNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {

        std::string sNotSet = "#NOTSET#";

        m_sExecutable = this->getDomElementAttribute(pElement, "exec", QString(sNotSet.c_str())).toStdString();
        m_sExecLocation = this->getDomElementAttribute(pElement, "location", QString(sNotSet.c_str())).toStdString();
        m_sCLArg = this->getDomElementAttribute(pElement, "param", QString(sNotSet.c_str())).toStdString();

        m_bTest = (this->getDomElementAttribute(pElement, "test", "false").compare("True", Qt::CaseInsensitive) == 0);
        m_bWSL = (this->getDomElementAttribute(pElement, "wsl", "false").compare("True", Qt::CaseInsensitive) == 0);

        if (m_sExecutable.compare(sNotSet) == 0)
            throw "executable not set";

        if (m_sExecLocation.compare(sNotSet) == 0)
            throw "executable location not set";


    }

    virtual std::string evaluateChildren( std::map< std::string, ExecutionNode*>* pID2Node,
                                          std::map<std::string, std::string>* pInputID2Value,
                                          std::map<std::string, QWidget*>* pInputID2Widget,
                                          QProcess* pProcess, ExecuteThread* pThread,
                                          bool bDeferred = false)
    {
        std::string sReturn = "";

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (bDeferred)
            {
                // only output nodes
                if (ExecutionOutputNode* pOutPutnode = dynamic_cast<ExecutionOutputNode*>( m_vChildren.at(i) ))
                {
                    sReturn = sReturn + pOutPutnode->evaluateDeferred(pID2Node, pInputID2Value, pInputID2Widget, pProcess, pThread, bDeferred);
                }
            } else {

                if (pProcess != NULL)
                {

                    if (ExecutionOutputNode* pOutPutnode = dynamic_cast<ExecutionOutputNode*>( m_vChildren.at(i) ))
                    {
                        sReturn = sReturn + pOutPutnode->evaluateDeferred(pID2Node, pInputID2Value, pInputID2Widget, pProcess, pThread, bDeferred);
                    } else {
                        sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

                    }

                } else {

                    sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

                }



            }



        }

        return sReturn;
    }

    size_t parseCommand(std::string* pCommand, size_t iPos, std::map< std::string, ExecutionNode*>* pID2Node,
                        std::map<std::string, std::string>* pInputID2Value,
                        std::map<std::string, QWidget*>* pInputID2Widget)
    {

        size_t iVarPos;
        size_t iStartPos = iPos;

        while ( (iVarPos = pCommand->find("${", iPos)) != std::string::npos )
        {

            iPos = this->parseCommand(pCommand, iVarPos+2, pID2Node, pInputID2Value, pInputID2Widget);

        }

        size_t iVarEnd = pCommand->find("}", iPos);

        if (iVarEnd == std::string::npos)
        {
            iVarEnd = pCommand->size();
            return iVarEnd;
        }


        std::string sPrefix = pCommand->substr(0, iStartPos-2);
        std::string sSuffix = pCommand->substr(iVarEnd+1, pCommand->size());
        std::string sID = pCommand->substr(iStartPos, iVarEnd-iStartPos);

        pCommand->clear();
        pCommand->append(sPrefix);

        std::string sValue = this->evaluateID(sID, pID2Node, pInputID2Value, pInputID2Widget);


        std::cout << "id " << sID << " value " << sValue << std::endl;
        pCommand->append(sValue);
        size_t iReturn = pCommand->size();

        pCommand->append(sSuffix);


        return iReturn;


    }

    std::string getCLArgs( std::map< std::string, ExecutionNode*>* pID2Node,
                           std::map<std::string, std::string>* pInputID2Value,
                           std::map<std::string, QWidget*>* pInputID2Widget )
    {
        std::string sCLArg = "";

        if (m_sCLArg.size() > 0)
        {

            if (m_sCLArg.find("${") == std::string::npos)
            {

                std::map< std::string, ExecutionNode*>::iterator oCLNode = pID2Node->find(m_sCLArg);

                if (oCLNode != pID2Node->end())
                {
                    ExecutionNode* pParamNode = oCLNode->second;
                    sCLArg = pParamNode->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
                } else {
                    sCLArg = m_sCLArg;
                }

            } else {

                sCLArg = std::string(m_sCLArg);
                this->parseCommand(&sCLArg, 0, pID2Node, pInputID2Value, pInputID2Widget);

            }

        }

        return sCLArg;
    }

    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::string sCLArg = this->getCLArgs(pID2Node, pInputID2Value, pInputID2Widget);
        std::string sProgram = m_sExecLocation + m_sExecutable;

        if (m_bTest)
        {
            sProgram = "/usr/bin/echo " + sProgram;
        }


        ProcessLauncher* pLauncher = new ProcessLauncher(QString(sProgram.c_str()), QString(sCLArg.c_str()), m_bWSL);

        this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, pLauncher->getProcess(), pLauncher->getThread(), false);

        pLauncher->connect(pLauncher, &ProcessLauncher::finished,
                [pLauncher, pID2Node, pInputID2Value, pInputID2Widget, this](){

                    this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, pLauncher->getProcess(), pLauncher->getThread(), true);
                    pLauncher->deleteLater();

                });


        pLauncher->start( );

        return "";

    }

protected:



    std::string m_sExecutable;
    std::string m_sExecLocation;
    std::string m_sCLArg;

    bool m_bTest = false;
    bool m_bWSL = true;

};


#endif //BIOGUI_EXECUTIONEXECUTENODE_H
