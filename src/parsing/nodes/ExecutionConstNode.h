//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONCONSTNODE_H
#define BIOGUI_EXECUTIONCONSTNODE_H


#include "ExecutionNode.h"

class ExecutionConstNode : public ExecutionNode {
public:

    ExecutionConstNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {

        m_sValue = pElement->text().toStdString();

    }

    std::string evaluate( std::map<std::string, std::string>* pID2Value )
    {
        return m_sValue;
    }

protected:


};


#endif //BIOGUI_EXECUTIONCONSTNODE_H
