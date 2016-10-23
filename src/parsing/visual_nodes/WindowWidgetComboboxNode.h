//
// Created by joppich on 10/21/16.
//

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
                throw WindowNodeException( "Not a valid ComboItem: " + oChildNode.toCDATASection().data().toStdString() );

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
