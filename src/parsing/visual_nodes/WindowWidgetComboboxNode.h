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

#ifndef BIOGUI_WINDOWWIDGETCOMBOBOXNODE_H
#define BIOGUI_WINDOWWIDGETCOMBOBOXNODE_H

#include <QtWidgets/QComboBox>
#include <src/app/QComboItem.h>
#include "WindowWidgetNode.h"
#include "WindowWidgetComboitemNode.h"

class WindowWidgetComboboxNode : public WindowWidgetNode {

public:

    WindowWidgetComboboxNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetComboboxNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QString sSelected = this->getQAttribute(pDOMElement, "selected", "");
        QStringList vSelected;

        if (sSelected.length() > 0)
        {
            vSelected = sSelected.split(";");
        }

        WindowWidgetComboitemNode oComboItemNode(m_pFactory);

        QComboBox *pComboBox = new QComboBox();
        QDomNodeList oChildren = pDOMElement->childNodes();
        for (size_t i = 0; i < oChildren.size(); ++i)
        {
            QDomElement oChildNode = oChildren.at(i).toElement();

            QComboItem* pComboItem = dynamic_cast<QComboItem *>(oComboItemNode.getWindowElement(&oChildNode).pElement);
            if (pComboItem == NULL)
            {
                LOGERROR( "Not a valid ComboItem: " + oChildNode.toCDATASection().data().toStdString());
                continue;
            }


            pComboBox->addItem( pComboItem->getValue(), pComboItem->getData() );

            if (i == 0)
                pComboBox->setCurrentIndex(0);

            if ( vSelected.contains( pComboItem->getData().toString() ) )
                pComboBox->setCurrentIndex(i);
        }


        oReturn.pElement = pComboBox;
        oReturn.bHasChildrenFinished = true;

        oReturn.addRetriever(this->getDomID(pDOMElement) , [pComboBox] () {

            return pComboBox->currentData().toString().toStdString();

        });

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "selected" );

    }


};

#endif //BIOGUI_WINDOWWIDGETCOMBOBOXNODE_H
