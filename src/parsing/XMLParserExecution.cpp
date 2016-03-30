//
// Created by joppich on 3/30/16.
//

#include "XMLParserExecution.h"

ExecutionNode *XMLParserExecution::getExecutionNodes(QDomElement* pElement ) {

    if (!pElement->isElement())
        return NULL;

    ExecutionNode* pNode = this->createExecutionNode( pElement);

    if (pNode == NULL)
    {
        throw "node not created exception " + pElement->tagName().toStdString();
    }

    QDomNodeList oChildren = pElement->childNodes();

    for (size_t i = 0; i < oChildren.length(); ++i)
    {

        QDomElement oChild = oChildren.item(i).toElement();

        ExecutionNode* pChild = this->getExecutionNodes(&oChild);

        pNode->addChild( pChild );

    }


    return pNode;

}

ExecutionNetwork *XMLParserExecution::createNetwork(QDomElement* pElement) {

    ExecutionNetwork* pNetwork = new ExecutionNetwork();
    pNetwork->setNodes( this->getExecutionNodes(pElement) );

}

QDomElement *XMLParserExecution::getRoot() {
    return this->getDocumentElementByName("execution");
}

ExecutionNetwork *XMLParserExecution::getExecutionNetwork() {

    QDomElement* pExecutionRoot = this->getRoot();

    bool bUnimportant;
    ExecutionNetwork* pNetwork = createNetwork(pExecutionRoot);

    return pNetwork;

}