//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONEXECUTENODE_H
#define BIOGUI_EXECUTIONEXECUTENODE_H


#include <QThread>
#include <QProcess>
#include <iostream>
#include "ExecutionNode.h"

class ExecutionExecuteNode : public ExecutionNode {

public:

    ExecutionExecuteNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {

        std::string sNotSet = "#NOTSET#";

        m_sExecutable = this->getDomElementAttribute(pElement, "exec", QString(sNotSet.c_str())).toStdString();
        m_sExecLocation = this->getDomElementAttribute(pElement, "location", QString(sNotSet.c_str())).toStdString();
        m_sCLArg = this->getDomElementAttribute(pElement, "param", QString(sNotSet.c_str())).toStdString();

        if (m_sExecutable.compare(sNotSet) == 0)
            throw "executable not set";

        if (m_sExecLocation.compare(sNotSet) == 0)
            throw "executable location not set";


    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::string sCLArg = "";

        if (m_sCLArg.size() > 0)
        {
            std::map< std::string, ExecutionNode*>::iterator oCLNode = pID2Node->find(m_sCLArg);

            if (!(oCLNode != pID2Node->end()))
                throw "param node not found";

            ExecutionNode* pParamNode = oCLNode->second;

            sCLArg = pParamNode->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

        }

        std::cout << "running " << m_sExecLocation << m_sExecutable << " " << sCLArg << std::endl;

        bool bActuallyRun = false;

        if (bActuallyRun)
        {

            std::string sProgram = m_sExecLocation + m_sExecutable;
            int iReturnCode = QProcess::execute( QString(sProgram.c_str()), QStringList(sCLArg.c_str()) );

            return std::to_string(iReturnCode);
        }

        return 0;

    }

protected:

    std::string m_sExecutable;
    std::string m_sExecLocation;
    std::string m_sCLArg;

};


#endif //BIOGUI_EXECUTIONEXECUTENODE_H
