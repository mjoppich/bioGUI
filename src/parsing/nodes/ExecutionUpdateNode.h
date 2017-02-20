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

#ifndef BIOGUI_EXECUTIONUPDATENODE_H
#define BIOGUI_EXECUTIONUPDATENODE_H


#include "ExecutionNode.h"

class ExecutionUpdateNode : public ExecutionNode {

public:

    ExecutionUpdateNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {
        m_sValue = this->getDomElementAttribute(pElement, "VALUE", "").toStdString();

        m_sTarget = this->getDomElementAttribute(pElement, "TARGET", "");
        m_sAttribute = this->getDomElementAttribute(pElement, "ATTRIB", "");

    }

    virtual ~ExecutionUpdateNode()
    {

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {

        std::map<std::string, WidgetFunctionNode*>::iterator oIt = pInputID2FunctionWidget->find(m_sTarget.toStdString());

        if (oIt != pInputID2FunctionWidget->end())
        {
            // do something
            oIt->second->setAttribute( m_sAttribute.toStdString(), m_sValue );

            return m_sValue;
        }

        return "";
    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("TARGET");
        vAttributes.push_back("ATTRIB");
        vAttributes.push_back("VALUE");
    }

    QString m_sAttribute;
    QString m_sTarget;


};


#endif //BIOGUI_EXECUTIONUPDATENODE_H
