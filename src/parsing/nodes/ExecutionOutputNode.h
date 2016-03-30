//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONOUTPUTNODE_H
#define BIOGUI_EXECUTIONOUTPUTNODE_H


#include "ExecutionNode.h"

class ExecutionOutputNode : public ExecutionNode {

public:
    ExecutionOutputNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {

    }

};


#endif //BIOGUI_EXECUTIONOUTPUTNODE_H
