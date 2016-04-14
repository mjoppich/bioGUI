//
// Created by joppich on 4/14/16.
//

#ifndef BIOGUI_EXECUTIONORDEREDADDNODE_H
#define BIOGUI_EXECUTIONORDEREDADDNODE_H

#include "ExecutionAddNode.h"

class ExecutionOrderedAddNode : public ExecutionAddNode {
public:

    ExecutionOrderedAddNode(QDomElement* pElement)
            : ExecutionAddNode(pElement)
    {
        throw "unimplemented";
    }

};


#endif //BIOGUI_EXECUTIONORDEREDADDNODE_H
