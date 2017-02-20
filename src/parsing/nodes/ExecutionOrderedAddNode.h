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

#ifndef BIOGUI_EXECUTIONORDEREDADDNODE_H
#define BIOGUI_EXECUTIONORDEREDADDNODE_H

#include "ExecutionAddNode.h"
#include "ExecutionValueNode.h"
#include <src/app/QExtGridLayout.h>
#include <src/app/QExclusiveGroupBox.h>

class ExecutionOrderedAddNode : public ExecutionAddNode {
public:

    ExecutionOrderedAddNode(QDomElement* pElement)
            : ExecutionAddNode(pElement)
    {

        m_sFrom = this->getDomElementAttribute(pElement, "from", "").toStdString();

        m_bSelected = (this->getDomElementAttribute(pElement, "selected", "false").compare("true", Qt::CaseInsensitive) == 0);

    }

    virtual ~ExecutionOrderedAddNode()
    {

    }

    ExecutionValueNode* findNode(std::string sID, std::vector<ExecutionNode*>* pNodes)
    {

        QString sqID(sID.c_str());

        for (size_t i = 0; i < pNodes->size(); ++i)
        {
            ExecutionValueNode* pNode = NULL;

            if (pNode = dynamic_cast<ExecutionValueNode*>(pNodes->at(i)))
            {
                if (!pNode->hasFor())
                    continue;

                if (sqID.compare(pNode->getFor().c_str(), Qt::CaseInsensitive) == 0)
                    return pNode;
            }
        }

        return NULL;

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {

        std::vector<ExecutionNode*> vSorted;

        QExclusiveGroupBox* pFromElem = NULL;
        std::map<std::string, WidgetFunctionNode*>::iterator oIt = pInputID2FunctionWidget->find(m_sFrom);
        if (oIt != pInputID2FunctionWidget->end())
        {
            pFromElem = dynamic_cast<QExclusiveGroupBox*>(oIt->second->getWidget());

        }

        if (pFromElem == NULL)
        {
            LOGERROR("from element is null");

            return "";
        }


        std::map<QWidget*, std::string> mWidgetToString;

        for (oIt = pInputID2FunctionWidget->begin(); oIt != pInputID2FunctionWidget->end(); ++oIt)
        {
            mWidgetToString.insert(std::pair<QWidget*, std::string>(oIt->second->getWidget(), oIt->first));
        }

        // ordered ids in gridlayout
        std::vector<std::string> vOrderedIDs = pFromElem->getOrderedIDs( &mWidgetToString, m_bSelected );

        for (size_t i = 0; i < vOrderedIDs.size(); ++i)
        {
            ExecutionNode* pNode = this->findNode(vOrderedIDs.at(i), &m_vChildren);

            if (pNode != NULL)
                vSorted.push_back( pNode );
        }

        // add remaining nodes
        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            ExecutionNode* pChild = m_vChildren.at(i);

            ExecutionValueNode* pNode = NULL;

            if (pNode = dynamic_cast<ExecutionValueNode*>(pChild))
            {
                if (pNode->hasFor())
                    continue;
            }


            vSorted.push_back( pChild );

        }

        std::string sReturn = "";

        for (size_t i = 0; i < vSorted.size(); ++i)
        {

            if (i > 0)
            {
                sReturn = sReturn + m_sSeperator;
            }

            std::string sReturned = vSorted.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget);
            sReturn = sReturn + sReturned;

        }

        return sReturn;

    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("FROM");
        vAttributes.push_back("SELECTED");

    }

    std::string m_sFrom = "";

    bool m_bSelected = false;

};


#endif //BIOGUI_EXECUTIONORDEREDADDNODE_H
