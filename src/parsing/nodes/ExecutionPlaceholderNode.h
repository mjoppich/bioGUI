//
// Created by joppich on 4/11/16.
//

#ifndef BIOGUI_EXECUTIONPLACEHOLDERNODE_H
#define BIOGUI_EXECUTIONPLACEHOLDERNODE_H


#include "ExecutionNode.h"

class ExecutionPlaceholderNode : public ExecutionNode {
public:

    ExecutionPlaceholderNode(QDomElement* pElement)
    : ExecutionNode(pElement)
            {


            }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {


        return this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget);

    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.clear();
    }

};


#endif //BIOGUI_EXECUTIONPLACEHOLDERNODE_H
