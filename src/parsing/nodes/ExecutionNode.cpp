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
