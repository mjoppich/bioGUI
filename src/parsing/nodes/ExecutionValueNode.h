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
        m_sFor = this->getDomElementAttribute(pElement, "for", "").toStdString();

        if (m_sFrom.size() == 0)
        {
            throw "value node without from " + pElement->toCDATASection().nodeValue().toStdString();
        }

    }

    static std::string getFromID(std::string sID, std::map< std::string, ExecutionNode*>* pID2Node,
                                 std::map<std::string, std::string>* pInputID2Value,
                                 std::map<std::string, QWidget*>* pInputID2Widget)
    {
        std::map<std::string, std::string>::iterator oIt = pInputID2Value->find( sID );

        // either the id is an input field
        if (oIt != pInputID2Value->end())
        {
            return oIt->second;
        }

        // or it also might be another node
        std::map<std::string, ExecutionNode*>::iterator oJt = pID2Node->find( sID );

        // either the id is an input field
        if (oJt != pID2Node->end())
        {
            return oJt->second->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
        }

        throw "id not found " + sID;
    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        return ExecutionValueNode::getFromID(m_sFrom, pID2Node, pInputID2Value, pInputID2Widget);

    }

    std::string getFrom()
    {
        return m_sFrom;
    }

    bool hasFrom()
    {
        return (m_sFrom.size() != 0);
    }

    std::string getFor()
    {
        return m_sFor;
    }

    bool hasFor()
    {
        return (m_sFor.size() != 0);
    }

protected:

    std::string m_sFrom;
    std::string m_sFor;


};


#endif //BIOGUI_EXECUTIONVALUENODE_H
