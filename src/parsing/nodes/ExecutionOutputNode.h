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

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node, std::map<std::string, std::string>* pInputID2Value )
    {

    }

};


#endif //BIOGUI_EXECUTIONOUTPUTNODE_H
