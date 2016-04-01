//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONNETWORK_H
#define BIOGUI_EXECUTIONNETWORK_H


#include <functional>
#include "ExecutionNode.h"
#include "ExecutionExecuteNode.h"
#include "ExecutionIfNode.h"

class QWidget;

class ExecutionNetwork {

public:


    int execute( std::map< std::string, std::function< std::string () > >* pID2Value, std::map< std::string, QWidget* >* pInputID2Widget )
    {

        std::map< std::string, std::string> mInputID2Value;
        std::map<std::string, std::function< std::string () > >::iterator oIt = pID2Value->begin();

        for ( ; oIt != pID2Value->end(); ++oIt)
        {

            std::string sResult = oIt->second();

            mInputID2Value.insert( std::pair<std::string, std::string>( oIt->first, sResult ) );

        }

        std::vector< ExecutionNode*> vExecNodes;
        std::map< std::string, ExecutionNode*> mID2Node;

        for (size_t i = 0; i < m_vNodes.size(); ++i)
        {

            ExecutionNode* pNode = m_vNodes.at(i);
            pNode->getNodeMap( &mID2Node );

            if (ExecutionExecuteNode* pExecNode = dynamic_cast<ExecutionExecuteNode*>( pNode ))
            {

                vExecNodes.push_back(pExecNode);

            }

            if (ExecutionIfNode* pExecNode = dynamic_cast<ExecutionIfNode*>( pNode ))
            {

                vExecNodes.push_back(pExecNode);

            }

        }

        if (vExecNodes.size() == 0)
        {
            throw "No exec nodes found!";
        }





        for (size_t i = 0; i < vExecNodes.size(); ++i)
        {

            ExecutionNode* pExecNode = vExecNodes.at(i);

            pExecNode->evaluate( &mID2Node, &mInputID2Value, pInputID2Widget );

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


protected:


    std::vector<ExecutionNode*> m_vNodes;



};


#endif //BIOGUI_EXECUTIONNETWORK_H
