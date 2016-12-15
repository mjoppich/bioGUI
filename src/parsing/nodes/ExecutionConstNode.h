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

        m_sValue = "";
        m_sTag = "const";

        if (pElement != NULL)
            m_sValue = pElement->text().toStdString();

    }

    virtual ~ExecutionConstNode()
    {

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {
        return m_sValue;
    }

    void setValue(std::string sValue)
    {
        m_sValue = sValue;
    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
    }


};


#endif //BIOGUI_EXECUTIONCONSTNODE_H
