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

    std::string evaluate( std::map<std::string, std::string>* pID2Value )
    {


        std::string sReturn = "";

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (i > 0)
            {
                sReturn = sReturn + m_sSeperator;
            }

            sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Value);

        }

        return sReturn;

    }


protected:


    std::string m_sSeperator;

};


#endif //BIOGUI_EXECUTIONADDNODE_H
