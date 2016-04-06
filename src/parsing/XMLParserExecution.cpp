//
// Created by joppich on 3/30/16.
//

#include "XMLParserExecution.h"

ExecutionNode *XMLParserExecution::getExecutionNodes(QDomElement* pElement ) {

    if (!pElement->isElement())
        return NULL;

    //std::cout << pElement->tagName().toStdString() << std::endl;

    ExecutionNode* pNode = this->createExecutionNode( pElement );

    if (pNode == NULL)
    {
        throw "node not created exception " + pElement->tagName().toStdString();
    }

    QDomNodeList oChildren = pElement->childNodes();

    for (size_t i = 0; i < oChildren.length(); ++i)
    {

        QDomElement oChild = oChildren.item(i).toElement();

        ExecutionNode* pChild = this->getExecutionNodes(&oChild);

        if (pChild != NULL)
            pNode->addChild( pChild );

    }


    return pNode;

}

ExecutionNetwork *XMLParserExecution::createNetwork(QDomElement* pElement) {

    ExecutionNetwork* pNetwork = new ExecutionNetwork();

    QDomNodeList oExecutionNodes = pElement->childNodes();
    for (size_t i = 0; i < oExecutionNodes.length(); ++i)
    {
        QDomElement oElement = oExecutionNodes.at(i).toElement();

        ExecutionNode* pNode = this->getExecutionNodes(&oElement);

        if (pNode != NULL)
            pNetwork->setNodes( pNode );
    }

    return pNetwork;
}

QDomElement *XMLParserExecution::getRoot( QDomDocument* pDocument ) {
    return this->getDocumentElementByName(pDocument, "execution");
}

ExecutionNetwork *XMLParserExecution::getExecutionNetwork() {

    QDomElement* pExecutionRoot = this->getRoot( m_pDocument );

    bool bUnimportant;
    ExecutionNetwork* pNetwork = createNetwork(pExecutionRoot);

    return pNetwork;

}