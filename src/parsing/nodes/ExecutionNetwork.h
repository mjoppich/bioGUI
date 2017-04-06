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

#ifndef BIOGUI_EXECUTIONNETWORK_H
#define BIOGUI_EXECUTIONNETWORK_H


#include <functional>
#include <src/parsing/visual_nodes/WidgetFunctionNode.h>
#include "ExecutionNode.h"
#include "ExecutionExecuteNode.h"
#include "ExecutionIfNode.h"

class QWidget;

class ExecutionNetwork : public QObject{
 Q_OBJECT
public:

    ExecutionNetwork(std::map< std::string, std::function< std::string () > >* pID2Value,
                     std::map< std::string, WidgetFunctionNode* >* pInputID2FunctionWidget )
    {
        this->setMaps(pID2Value, pInputID2FunctionWidget);
    }

    ~ExecutionNetwork()
    {
        // those are variables of xmlparserwindow!
        //delete m_pID2Value;
        //delete m_pInputID2Widget;

    }

    void setMaps(std::map< std::string, std::function< std::string () > >* pID2Value,
                 std::map< std::string, WidgetFunctionNode* >* pInputID2FunctionWidget)
    {
        m_pID2Value = pID2Value;
        m_pInputID2FunctionWidget = pInputID2FunctionWidget;
    }

    int execute( std::string& sProgramToRun )
    {

        mInputID2Value.clear();
        mID2Node.clear();

        // fill inputid2value
        std::map<std::string, std::function< std::string () > >::iterator oIt = m_pID2Value->begin();

        for ( ; oIt != m_pID2Value->end(); ++oIt)
        {

            std::string sResult = oIt->second();

            //std::cout << "inputid2value " << oIt->first << " " << sResult << std::endl;

            mInputID2Value.insert( std::pair<std::string, std::string>( oIt->first, sResult ) );

        }

        std::vector< ExecutionNode*> vExecNodes;

        for (size_t i = 0; i < m_vNodes.size(); ++i)
        {

            ExecutionNode* pNode = m_vNodes.at(i);
            pNode->getNodeMap( &mID2Node );

            if (ExecutionExecuteNode* pExecNode = dynamic_cast<ExecutionExecuteNode*>( pNode ))
            {

                if ((sProgramToRun.size() == 0) || (sProgramToRun.compare( pExecNode->getProgramName() ) == 0))
                {
                    m_vExecNodes.push_back(pExecNode);
                }

            }

            if (ExecutionIfNode* pExecNode = dynamic_cast<ExecutionIfNode*>( pNode ))
            {

                if ((sProgramToRun.size() == 0) || (sProgramToRun.compare( pExecNode->getProgramName() ) == 0))
                {
                    m_vExecNodes.push_back(pExecNode);
                }

            }

        }

        if (m_vExecNodes.size() == 0)
        {

            return -1;

        } else {
            return this->performExecution();
        }

    }

    void setNodes(ExecutionNode* pNode)
    {
        m_vNodes.push_back(pNode);
    }

    void setNodes(std::vector<ExecutionNode*> vNodes)
    {
        m_vNodes.insert(m_vNodes.end(), vNodes.begin(), vNodes.end());
    }

    int performExecution()
    {

        std::vector<ExecutionNode*>::iterator oIt = m_vExecNodes.begin();

        if (oIt == m_vExecNodes.end())
        {
            LOGERROR("EXECUTION FINISHED!")

            emit executionFinished();
            return 0;
        }

        ExecutionNode* pNode = *oIt;

        // this should never be called again :)
        m_vExecNodes.erase(oIt);

        if (ExecutionExecutableNode* pExecNode = dynamic_cast<ExecutionExecutableNode*>( pNode ))
        {

            // when node is finished, we start the next execution
            this->connect( pExecNode,&ExecutionExecutableNode::finishedExecution, this, &ExecutionNetwork::performExecution );

            std::cout << "evaluate " << pExecNode->getID() << std::endl;

            pExecNode->evaluate( &mID2Node, &mInputID2Value, m_pInputID2FunctionWidget, true );

        }

        return 0;

    }

signals:

    void executionFinished();

protected:


    // given from outside
    std::map< std::string, std::function< std::string () > >* m_pID2Value;
    std::map< std::string, WidgetFunctionNode* >* m_pInputID2FunctionWidget;


    // to be filled in execute
    std::map< std::string, std::string> mInputID2Value;
    std::map< std::string, ExecutionNode*> mID2Node;


    // for internal selection
    std::vector<ExecutionNode*> m_vNodes;
    std::vector<ExecutionNode*> m_vExecNodes;



};


#endif //BIOGUI_EXECUTIONNETWORK_H
