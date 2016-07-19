//
// Created by joppich on 3/31/16.
//

#include "ExecutionIfNode.h"
#include "ExecutionExecuteNode.h"

std::string ExecutionIfNode::evaluateChildren(std::map<std::string, ExecutionNode *> *pID2Node,
                                              std::map<std::string, std::string> *pInputID2Value,
                                              std::map<std::string, QWidget *> *pInputID2Widget,
                                              bool bEmitSignal)
{

    std::vector<std::string> vReturn;

    for (size_t i = 0; i < m_vChildren.size(); ++i)
    {

        ExecutionNode* pChild = m_vChildren.at(i);

        if (m_bEvaluateElse)
        {
            if (pChild != m_pElseNode)
                continue;
        } else {
            if (pChild == m_pElseNode)
                continue;
        }

        if (bEmitSignal)
        {
            if (ExecutionExecutableNode* pExecNode = dynamic_cast<ExecutionExecutableNode*>( pChild ))
            {
                ++m_iHasExecuteChild;

                QObject::connect(pExecNode, &ExecutionExecutableNode::finishedExecution, [bEmitSignal, this] () { emit this->childHasFinished(bEmitSignal);});

                std::string sReturn = pExecNode->evaluate(pID2Node, pInputID2Value, pInputID2Widget, bEmitSignal);
                if (sReturn.size() != 0)
                    vReturn.push_back(sReturn);

                continue;
            }
        }

        std::string sReturn = pChild->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
        if (sReturn.size() != 0)
            vReturn.push_back(sReturn);

    }


    std::string sReturn = "";

    if ( vReturn.size() > 0 )
    {
        for(size_t i = 0; i < vReturn.size(); ++i)
        {
            if (i > 0)
                sReturn = sReturn + m_sSeperator;

            sReturn = sReturn + vReturn.at(i);
        }
    }

    return sReturn;
}

/*
std::string ExecutionIfNode::evaluateElseNode(std::map< std::string, ExecutionNode*>* pID2Node,
                                              std::map<std::string, std::string>* pInputID2Value,
                                              std::map<std::string, QWidget*>* pInputID2Widget) {

    if (m_pElseNode)
    {
        return m_pElseNode->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget);
    }

    return "";

}
*/