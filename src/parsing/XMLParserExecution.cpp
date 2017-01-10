/*
 *  bioGUI - a new universal GUI for bioinformatic applications
 *  Copyright (C) 2016  Markus Joppich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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

        if ((oChildren.size() == 1) && (oChildren.at(0).isText() == true))
        {

            QString sText = pElement->text();

            if (sText.size() > 0)
            {

                ExecutionConstNode* pConstNode = new ExecutionConstNode(NULL);
                pConstNode->setValue( sText.toStdString() );
                pNode->addChild( pConstNode );

            }

        } else {

            for (size_t i = 0; i < oChildren.length(); ++i)
            {

                QDomNode oChildNode = oChildren.item(i);
                //std::cerr << oChildNode.isText() << std::endl;
                //std::cerr << oChildNode.isElement() << std::endl;

                QDomElement oChild = oChildren.item(i).toElement();



                ExecutionNode* pChild = this->getExecutionNodes(&oChild);

                if (pChild != NULL)
                    pNode->addChild( pChild );

            }

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
