//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONMATHNODE_H
#define BIOGUI_EXECUTIONMATHNODE_H

#include "ExecutionNode.h"

class ExecutionMathNode : public ExecutionNode {

public:

    ExecutionMathNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {

        m_eType = NODE_TYPE::NUMBER;

        m_sOperation = this->getDomElementAttribute(pElement, "op", "+").toStdString();

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node, std::map<std::string, std::string>* pInputID2Value )
    {


        float fReturn = 0.0f;

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (!m_vChildren.at(i)->getType() == NODE_TYPE::NUMBER)
            {
                throw "incompatible node types";
            }

            std::string sChild = m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value);
            float fChild = std::stof(sChild);

            if (i == 0)
            {
                fReturn = fChild;
            } else {

                if (m_sOperation.compare("+") == 0)
                {
                    fReturn += fChild;
                    continue;
                }

                if (m_sOperation.compare("-") == 0)
                {
                    fReturn -= fChild;
                    continue;
                }

                if (m_sOperation.compare("*") == 0)
                {
                    fReturn *= fChild;
                    continue;
                }

                if (m_sOperation.compare("/") == 0)
                {
                    fReturn /= fChild;
                    continue;
                }

                if (m_sOperation.compare("%") == 0)
                {
                    fReturn = (int)fReturn % (int)fChild;
                    continue;
                }

                throw "unimplemented operation";

            }

        }

        if (m_sOperation.compare("int") == 0)
            return std::to_string( (int) fReturn );

        return std::to_string(fReturn);

    }

protected:

    std::string m_sOperation;


};


#endif //BIOGUI_EXECUTIONMATHNODE_H