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
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {

        if (m_sFrom.size() == 0)
        {
            LOGERROR("from attribute must be set!");
            return "";
        }

        Validable<std::string> oRet = this->getNodeValue(m_sFrom, pID2Node, pInputID2Value, pInputID2FunctionWidget);

        if (!oRet.valid())
            return "";

        return oRet.value();

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
