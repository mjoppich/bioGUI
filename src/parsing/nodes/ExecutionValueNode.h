//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONVALUENODE_H
#define BIOGUI_EXECUTIONVALUENODE_H


#include "ExecutionNode.h"

class ExecutionValueNode : public ExecutionNode {

public:

    ExecutionValueNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {


        m_sFrom = this->getDomElementAttribute(pElement, "FROM", "").toStdString();

        if (m_sFrom.size() == 0)
        {
            throw "value node without from " + pElement->toCDATASection().nodeValue().toStdString();
        }

    }


    std::string evaluate( std::map<std::string, std::string>* pID2Value )
    {

        std::map<std::string, std::string>::iterator oIt = pID2Value->find( m_sFrom );

        if (oIt != pID2Value->end())
        {
            return oIt->second;
        }

        throw "no id found " + m_sFrom;

    }

protected:

    std::string m_sFrom;


};


#endif //BIOGUI_EXECUTIONVALUENODE_H
