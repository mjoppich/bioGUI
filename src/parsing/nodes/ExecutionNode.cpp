//
// Created by joppich on 3/30/16.
//

#include "ExecutionNode.h"

bool ExecutionNode::hasID() {
    return (this->m_sID.size() > 0);
}

void ExecutionNode::addChild(ExecutionNode* pNode) {
    m_vChildren.push_back(pNode);
}

void ExecutionNode::addChildren(std::vector<ExecutionNode*>::iterator oItStart, std::vector<ExecutionNode*>::iterator oItEnd ) {

    while( oItStart != oItEnd)
    {

        m_vChildren.push_back( *oItStart );

        ++oItStart;
    }


}

QString ExecutionNode::getDomElementAttribute(QDomElement* pElement, QString sAttribName, QString sDefault) {

    if (pElement == NULL)
        return sDefault;

    QDomNamedNodeMap mAttributes = pElement->attributes();

    for (size_t i = 0; i < mAttributes.length(); ++i)
    {

        QDomAttr oItem = mAttributes.item(i).toAttr();

        if (oItem.name().compare( sAttribName, Qt::CaseInsensitive ) == 0)
        {
            return oItem.value();
        }

    }

    return sDefault;

}

bool ExecutionNode::hasDomElementAttribute(QDomElement* pElement, QString sAttribName) {

    QDomNamedNodeMap mAttributes = pElement->attributes();

    for (size_t i = 0; i < mAttributes.length(); ++i)
    {

        QDomAttr oItem = mAttributes.item(i).toAttr();

        if (oItem.name().compare( sAttribName, Qt::CaseInsensitive ) == 0)
        {
            return true;
        }

    }

    return false;

}
