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



    }

    virtual ~ExecutionValueNode()
    {

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        if (m_sFrom.size() == 0)
        {
            throw ExecutionNodeException("value node without from");
        }

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

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("FROM");
        vAttributes.push_back("FOR");
    }

    std::string m_sFrom;
    std::string m_sFor;


};


#endif //BIOGUI_EXECUTIONVALUENODE_H
