//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETSLIDERITEMNODE_H
#define BIOGUI_WINDOWWIDGETSLIDERITEMNODE_H

#include <src/app/QComboItem.h>
#include "WindowWidgetNode.h"

class WindowWidgetSlideritemNode : public WindowWidgetNode {

public:

    WindowWidgetSlideritemNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetSlideritemNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QComboItem* pItem =  new QComboItem("", "");

        pItem->setValue( sValue );
        QString sData = this->getQAttribute(pDOMElement, "value", sValue);
        pItem->setData(sData);

        oReturn.pElement = pItem;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "value" );
        vAttributes.push_back( "display" );

    }


};

#endif //BIOGUI_WINDOWWIDGETSLIDERITEMNODE_H
