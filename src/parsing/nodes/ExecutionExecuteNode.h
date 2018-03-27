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
                                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget,
                                          QProcess* pProcess, ExecuteThread* pThread,
                                          bool bDeferred = false)
    {
        std::string sReturn = "";

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (bDeferred)
            {
                // only output nodes
                if (ExecutionDeferredNode* pOutPutnode = dynamic_cast<ExecutionDeferredNode*>( m_vChildren.at(i) ))
                {
                    sReturn = sReturn + pOutPutnode->evaluateDeferred(pID2Node, pInputID2Value, pInputID2FunctionWidget, pProcess, pThread, bDeferred);
                }



            } else {

                if (pProcess != NULL)
                {

                    if (ExecutionOutputNode* pOutPutnode = dynamic_cast<ExecutionOutputNode*>( m_vChildren.at(i) ))
                    {
                        sReturn = sReturn + pOutPutnode->evaluateDeferred(pID2Node, pInputID2Value, pInputID2FunctionWidget, pProcess, pThread, bDeferred);
                    } else {
                        sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget);

                    }

                } else {

                    if (ExecutionOutputNode* pOutPutnode = dynamic_cast<ExecutionOutputNode*>( m_vChildren.at(i) ))
                    {
                        sReturn = sReturn + pOutPutnode->evaluateDeferred(pID2Node, pInputID2Value, pInputID2FunctionWidget, pProcess, pThread, bDeferred);
                    } else {
                        sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget);

                    }

                }



            }



        }

        return sReturn;
    }





    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget, bool bEmitSignal = false)
    {

        std::string sCLArg = this->parseDynamicValues(m_sCLArg, pID2Node, pInputID2Value, pInputID2FunctionWidget);
        std::string sLocation = this->parseDynamicValues(m_sExecLocation, pID2Node, pInputID2Value, pInputID2FunctionWidget);

        std::string sProgram = sLocation + m_sExecutable;

        bool bWSL = this->asBool(this->getNodeValueOrValue(m_sWSL, m_sWSL, pID2Node, pInputID2Value, pInputID2FunctionWidget));
        bWSL = bWSL && this->onWindows();

        ProcessLauncher* pLauncher = new ProcessLauncher(QString(sProgram.c_str()), QString(sCLArg.c_str()), bWSL);

        this->evaluateChildren(pID2Node, pInputID2Value, pInputID2FunctionWidget, pLauncher->getProcess(), NULL, false); //

        pLauncher->connect(pLauncher, &ProcessLauncher::finished,
                [pLauncher, pID2Node, pInputID2Value, pInputID2FunctionWidget, bEmitSignal, this](){

                    this->evaluateChildren(pID2Node, pInputID2Value, pInputID2FunctionWidget, pLauncher->getProcess(), NULL, true);
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
