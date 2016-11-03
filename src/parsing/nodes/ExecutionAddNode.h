//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONADDNODE_H
#define BIOGUI_EXECUTIONADDNODE_H


#include "ExecutionNode.h"

class ExecutionAddNode : public ExecutionNode {
public:

    ExecutionAddNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {

        m_sSeperator = this->getDomElementAttribute(pElement, "sep", " ").toStdString();



    }

    virtual ~ExecutionAddNode()
    {

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {


        std::string sReturn = "";

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (i > 0)
            {
                sReturn = sReturn + m_sSeperator;
            }

            sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

        }

        return sReturn;

    }


protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("sep");
    }


};


#endif //BIOGUI_EXECUTIONADDNODE_H
