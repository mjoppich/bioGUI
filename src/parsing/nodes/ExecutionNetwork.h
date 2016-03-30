//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONNETWORK_H
#define BIOGUI_EXECUTIONNETWORK_H


#include "ExecutionNode.h"

class ExecutionNetwork {

public:


    int execute()
    {
        return -1;
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
