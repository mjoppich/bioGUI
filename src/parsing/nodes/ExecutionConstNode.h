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
        {
            m_sValue = pElement->text().toStdString();
        }

        m_sValue = m_sValue;

    }

    virtual ~ExecutionConstNode()
    {

    }

    std::string getID()
    {

        if (m_sID.size() > 0)
            return m_sID;

        const void * address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        std::string name = ss.str();

        return name;
    }

    virtual std::vector<std::string> outputs() {

        std::string sID = this->getID();

        std::vector<std::string> vReturn;
        vReturn.push_back(sID);

        return vReturn;
    }


    virtual std::vector<std::string> inputs()
    {
        std::vector<std::string> vBase;
        vBase.push_back(this->getID() + "_" + m_sValue);

        return vBase;
    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
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
