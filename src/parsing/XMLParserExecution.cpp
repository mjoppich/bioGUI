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
        LOGERROR("node not created exception " + pElement->tagName().toStdString())

        return NULL;
    }

    if (pElement->tagName().compare("if", Qt::CaseInsensitive) == 0)
    {
        LOGERROR("if node: " + pElement->text().toStdString());
    }

    QDomNodeList oChildren = pElement->childNodes();

    if (oChildren.size() > 0)
    {
        for (size_t i = 0; i < oChildren.length(); ++i)
        {

            QDomElement oChild = oChildren.item(i).toElement();

            ExecutionNode* pChild = this->getExecutionNodes(&oChild);

            if (pChild != NULL)
                pNode->addChild( pChild );

        }
    } else {

        QString sText = pElement->text();

        if (sText.size() > 0)
        {

            ExecutionConstNode* pConstNode = new ExecutionConstNode(NULL);
            pConstNode->setValue( sText.toStdString() );
            pNode->addChild( pConstNode );

        }

    }



    pNode->finishedCreation();


    return pNode;

}

ExecutionNetwork *XMLParserExecution::createNetwork(QDomElement* pElement) {

    ExecutionNetwork* pNetwork = new ExecutionNetwork(NULL, NULL);

    QDomNodeList oExecutionNodes = pElement->childNodes();
    for (size_t i = 0; i < oExecutionNodes.length(); ++i)
    {
        QDomElement oElement = oExecutionNodes.at(i).toElement();

        ExecutionNode* pNode = this->getExecutionNodes(&oElement);

        if (pNode == NULL)
        {
            LOGERROR("Failed to create execution node for element: " + oElement.tagName().toStdString());
        }

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

    ExecutionNetwork* pNetwork = createNetwork(pExecutionRoot);

    if (pNetwork == NULL)
    {
        LOGERROR("Failed to create the execution network");
    }


    return pNetwork;


}
