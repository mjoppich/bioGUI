/*
 *  bioGUI - a new universal GUI for bioinformatic applications
 *  Copyright (C) 2016  Markus Joppich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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

    virtual ~ExecutionMathNode()
    {

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {


        float fReturn = 0.0f;

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (!m_vChildren.at(i)->getType() == NODE_TYPE::NUMBER)
            {
                LOGERROR("Incompatible node types for child nodes in : " + m_sID);
                continue;
            }

            std::string sChild = m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
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

                LOGERROR("Unimplemented math node operation: " + m_sOperation);

            }

        }

        if (m_sOperation.compare("int") == 0)
            return std::to_string( (int) fReturn );

        return std::to_string(fReturn);

    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back("OP");

    }

    std::string m_sOperation;


};


#endif //BIOGUI_EXECUTIONMATHNODE_H
