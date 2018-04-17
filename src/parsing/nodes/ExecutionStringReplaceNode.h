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

#ifndef BIOGUI_EXECUTIONSTRINGREPLACENODE_H
#define BIOGUI_EXECUTIONSTRINGREPLACENODE_H


#include <QFile>
#include <QTextStream>
#include "ExecutionNode.h"
#include "ExecutionValueNode.h"

class ExecutionStringReplaceNode : public ExecutionNode {

    public:

        ExecutionStringReplaceNode(QDomElement* pElement)
                : ExecutionNode(pElement)
        {


            m_sReplace = this->getDomElementAttribute(pElement, "replace", "").toStdString();
            m_sReplaceWith = this->getDomElementAttribute(pElement, "replace_with", "").toStdString();

            m_sFrom = this->getDomElementAttribute(pElement, "from", "").toStdString();

        }

        virtual std::vector<std::string> inputs()
        {
            std::vector<std::string> vBase;
            vBase.push_back(m_sFrom);

            return vBase;
        }

        std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                              std::map<std::string, std::string>* pInputID2Value,
                              std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
        {

            std::string sChildren = "";

            if (m_sFrom.size() == 0)
            {

                 sChildren = this->evaluateChildren(pID2Node, pInputID2Value, pInputID2FunctionWidget);

            } else {

                Validable<std::string> oRet = this->getNodeValue(m_sFrom, pID2Node, pInputID2Value, pInputID2FunctionWidget);

                if (!oRet.valid())
                    return "";

            }

            QString qsChildren(sChildren.c_str());
            qsChildren = qsChildren.replace(QString(m_sReplace.c_str()), QString(m_sReplaceWith.c_str()));

            return qsChildren.toStdString();

        }

    virtual ~ExecutionStringReplaceNode()
    {

    }

protected:

        void addNodeAttributes(std::vector<std::string>& vAttributes)
        {
            vAttributes.push_back("REPLACE");
            vAttributes.push_back("REPLACE_WITH");

        }

    std::string m_sReplace;
    std::string m_sReplaceWith;

    std::string m_sFrom;

};


#endif //BIOGUI_EXECUTIONSTRINGREPLACENODE_H
