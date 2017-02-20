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

#include "ExecutionIfNode.h"
#include "ExecutionExecuteNode.h"
#include "ExecutionPlaceholderNode.h"

std::string ExecutionIfNode::evaluateChildren(std::map<std::string, ExecutionNode *> *pID2Node,
                                              std::map<std::string, std::string> *pInputID2Value,
                                              std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget,
                                              bool bEmitSignal)
{

    std::vector<std::string> vReturn;

    for (size_t i = 0; i < m_vChildren.size(); ++i)
    {

        ExecutionNode* pChild = m_vChildren.at(i);

        if (m_bEvaluateElse)
        {
            if (pChild != m_pElseNode)
                continue;
        } else {
            if (pChild == m_pElseNode)
                continue;
        }

        if (bEmitSignal)
        {
            if (ExecutionExecutableNode* pExecNode = dynamic_cast<ExecutionExecutableNode*>( pChild ))
            {
                ++m_iHasExecuteChild;

                QObject::connect(pExecNode, &ExecutionExecutableNode::finishedExecution, [bEmitSignal, this] () { emit this->childHasFinished(bEmitSignal);});

                std::string sReturn = pExecNode->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget, bEmitSignal);
                if (sReturn.size() != 0)
                    vReturn.push_back(sReturn);

                continue;
            }
        }

        if (ExecutionPlaceholderNode* pElseNode = dynamic_cast<ExecutionPlaceholderNode*>( pChild )) {
            pElseNode->setSeperator(m_sSeperator);
        }

        std::string sReturn = pChild->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget);
        if (sReturn.size() != 0)
            vReturn.push_back(sReturn);

    }


    std::string sReturn = "";

    if ( vReturn.size() > 0 )
    {
        for(size_t i = 0; i < vReturn.size(); ++i)
        {
            if (i > 0)
                sReturn = sReturn + m_sSeperator;

            sReturn = sReturn + vReturn.at(i);
        }
    }

    return sReturn;
}

/*
std::string ExecutionIfNode::evaluateElseNode(std::map< std::string, ExecutionNode*>* pID2Node,
                                              std::map<std::string, std::string>* pInputID2Value,
                                              std::map<std::string, QWidget*>* pInputID2Widget) {

    if (m_pElseNode)
    {
        return m_pElseNode->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget);
    }

    return "";

}
*/