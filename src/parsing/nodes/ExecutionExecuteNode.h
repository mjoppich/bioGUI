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
#include "ExecutionExecutableNode.h"

#ifdef WINDOWS
    #include <windows.h>
#endif

class HTTPExecuteThread;

class ExecutionExecuteNode : public ExecutionExecutableNode {

public:

    ExecutionExecuteNode(QDomElement* pElement)
            : ExecutionExecutableNode(pElement)
    {

        std::string sNotSet = "#NOTSET#";

        m_sExecutable = this->getDomElementAttribute(pElement, "exec", "").toStdString();
        m_sExecLocation = this->getDomElementAttribute(pElement, "location", "").toStdString();
        m_sCLArg = this->getDomElementAttribute(pElement, "param", QString(sNotSet.c_str())).toStdString();

        m_sTest = this->getDomElementAttribute(pElement, "test", "false").toStdString();
        m_sWSL = this->getDomElementAttribute(pElement, "wsl", "false").toStdString();

        m_sProgram = this->getDomElementAttribute(pElement, "program", "").toStdString();

    }

    virtual ~ExecutionExecuteNode()
    {

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

                    if (ExecutionOutputNode* pOutPutnode = dynamic_cast<ExecutionOutputNode*>( m_vChildren.at(i) ))
                    {
                        sReturn = sReturn + pOutPutnode->evaluateDeferred(pID2Node, pInputID2Value, pInputID2Widget, pProcess, pThread, bDeferred);
                    } else {
                        sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

                    }

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

        // { prog  2>&amp;1 1>&amp;3 3>&amp;- | netcat ${ip} 55026; } 3>&amp;1 1>&amp;2 | netcat ${ip} 55025

        while( ( iVarPos = std::min(pCommand->find("${", iPos), pCommand->find("}", iPos))  ) != std::string::npos )
        {
            if (( pCommand->at(iVarPos) == '}' ) && (iStartPos != -1))
            {

                std::string sPrefix = pCommand->substr(0, iStartPos-2);
                std::string sSuffix = pCommand->substr(iVarPos+1, pCommand->size());
                std::string sID = pCommand->substr(iStartPos, iVarPos-iStartPos);

                pCommand->clear();
                pCommand->append(sPrefix);

                //std::string sValue = this->evaluateID(sID, pID2Node, pInputID2Value, pInputID2Widget);
                std::string sValue = this->getNodeValueOrValue(sID, sID, pID2Node, pInputID2Value, pInputID2Widget);

                std::cout << "id " << sID << " value " << sValue << std::endl;
                pCommand->append(sValue);

                pCommand->append(sSuffix);

                // uncommented because I might want to replace variables in variables :)
                iPos = sPrefix.size() + sValue.size();

                iStartPos = -1;


            } else {
                iPos = this->parseCommand(pCommand, iVarPos+2, pID2Node, pInputID2Value, pInputID2Widget);
            }
        }


        return iVarPos;


    }

    std::string parseArgs( std::string& sArgs, std::map< std::string, ExecutionNode*>* pID2Node,
                           std::map<std::string, std::string>* pInputID2Value,
                           std::map<std::string, QWidget*>* pInputID2Widget )
    {
        std::string sCLArg = "";

        if (sArgs.size() > 0)
        {

            if (sArgs.find("${") == std::string::npos)
            {

                std::map< std::string, ExecutionNode*>::iterator oCLNode = pID2Node->find(sArgs);

                if (oCLNode != pID2Node->end())
                {
                    ExecutionNode* pParamNode = oCLNode->second;
                    sCLArg = pParamNode->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
                } else {
                    sCLArg = sArgs;
                }

            } else {

                sCLArg = std::string(sArgs);
                this->parseCommand(&sCLArg, 0, pID2Node, pInputID2Value, pInputID2Widget);

            }

        }

        return sCLArg;
    }

    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget, bool bEmitSignal = false)
    {

        std::string sCLArg = this->parseArgs(m_sCLArg, pID2Node, pInputID2Value, pInputID2Widget);
        std::string sLocation = this->parseArgs(m_sExecLocation, pID2Node, pInputID2Value, pInputID2Widget);

        std::string sProgram = sLocation + m_sExecutable;

        bool bWSL = this->asBool(this->getNodeValueOrValue(m_sWSL, m_sWSL, pID2Node, pInputID2Value, pInputID2Widget));

        ProcessLauncher* pLauncher = new ProcessLauncher(QString(sProgram.c_str()), QString(sCLArg.c_str()), bWSL);

        this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, pLauncher->getProcess(), pLauncher->getThread(), false);

        pLauncher->connect(pLauncher, &ProcessLauncher::finished,
                [pLauncher, pID2Node, pInputID2Value, pInputID2Widget, bEmitSignal, this](){

                    this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, pLauncher->getProcess(), pLauncher->getThread(), true);
                    pLauncher->deleteLater();

                    // calculation finished!
                    if (bEmitSignal == true)
                        emit finishedExecution();

                });


        pLauncher->start( );

        return "";

    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        ExecutionExecutableNode::addNodeAttributes(vAttributes);

        vAttributes.push_back("EXEC");
        vAttributes.push_back("location");
        vAttributes.push_back("param");
        vAttributes.push_back("wsl");
        vAttributes.push_back("program");

    }


    std::string m_sExecutable;
    std::string m_sExecLocation;
    std::string m_sCLArg;

    std::string m_sTest = "false";
    std::string m_sWSL = "true";

    std::string m_sProgram = "";

};


#endif //BIOGUI_EXECUTIONEXECUTENODE_H
