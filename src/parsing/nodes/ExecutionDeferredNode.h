//
// Created by joppich on 2/20/17.
//

#ifndef BIOGUI_EXECUTIONDEFERREDNODE_H
#define BIOGUI_EXECUTIONDEFERREDNODE_H


#include <QProcess>
#include "ExecutionNode.h"
#include <src/app/ExecuteThread.h>


class ExecutionDeferredNode : public ExecutionNode {

public:

    ExecutionDeferredNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {
        m_bDeferred = (this->getDomElementAttribute(pElement, "DEFERRED", "true").toUpper().compare("TRUE", Qt::CaseInsensitive) == 0);

    }

    virtual std::vector<std::string> inputs()
    {
        std::vector<std::string> vBase;
        return vBase;
    }

    virtual std::string evaluateDeferred( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget,
                                  QProcess* pProcess, ExecuteThread* pThread, bool bDeferred) = 0;

protected:

    bool m_bDeferred;


};


#endif //BIOGUI_EXECUTIONDEFERREDNODE_H
