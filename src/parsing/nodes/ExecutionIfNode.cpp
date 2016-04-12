//
// Created by joppich on 3/31/16.
//

#include "ExecutionIfNode.h"

std::string ExecutionIfNode::evaluateElseNode(std::map< std::string, ExecutionNode*>* pID2Node,
                                              std::map<std::string, std::string>* pInputID2Value,
                                              std::map<std::string, QWidget*>* pInputID2Widget) {

    if (m_pElseNode)
    {
        return m_pElseNode->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget);
    }

    return "";

}