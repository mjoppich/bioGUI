//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONNODE_H
#define BIOGUI_EXECUTIONNODE_H

#include <vector>

class ExecutionNode {

public:

    void addChild(ExecutionNode* pNode)
    {
        m_vChildren.push_back(pNode);
    }

    void addChildren( std::vector<ExecutionNode*>::iterator oItStart, std::vector<ExecutionNode*>::iterator oItEnd )
    {

        while( oItStart != oItEnd)
        {

            m_vChildren.push_back( *oItStart );

            ++oItStart;
        }


    }


protected:

    std::vector<ExecutionNode*> m_vChildren;


};


#endif //BIOGUI_EXECUTIONNODE_H
