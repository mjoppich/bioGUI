//
// Created by joppich on 4/14/16.
//

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

    }

    ExecutionValueNode* findNode(std::string sID, std::vector<ExecutionNode*>* pNodes)
    {

        QString sqID(sID.c_str());

        for (size_t i = 0; i < pNodes->size(); ++i)
        {
            ExecutionValueNode* pNode = NULL;

            if (pNode = dynamic_cast<ExecutionValueNode*>(pNodes->at(i)))
            {
                if (!pNode->hasFrom())
                    continue;

                if (sqID.compare(pNode->getFrom().c_str(), Qt::CaseInsensitive) == 0)
                    return pNode;
            }
        }

        return NULL;

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::vector<ExecutionNode*> vSorted;

        QExclusiveGroupBox* pFromElem = NULL;
        std::map<std::string, QWidget*>::iterator oIt = pInputID2Widget->find(m_sFrom);
        if (oIt != pInputID2Widget->end())
        {
            pFromElem = dynamic_cast<QExclusiveGroupBox*>(oIt->second);

        }

        if (pFromElem == NULL)
            throw "invalid from";

        std::map<QWidget*, std::string> mWidgetToString;

        for (oIt = pInputID2Widget->begin(); oIt != pInputID2Widget->end(); ++oIt)
        {
            mWidgetToString.insert(std::pair<QWidget*, std::string>(oIt->second, oIt->first));
        }

        // ordered ids in gridlayout
        std::vector<std::string> vOrderedIDs = pFromElem->getOrderedIDs( &mWidgetToString );

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
                if (pNode->hasFrom())
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

            sReturn = sReturn + vSorted.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

        }

        return sReturn;

    }

protected:

    std::string m_sFrom = "";

};


#endif //BIOGUI_EXECUTIONORDEREDADDNODE_H
