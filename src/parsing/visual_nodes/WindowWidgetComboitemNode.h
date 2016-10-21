//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETCOMBOITEMNODE_H
#define BIOGUI_WINDOWWIDGETCOMBOITEMNODE_H

#include <src/app/QComboItem.h>
#include "WindowWidgetNode.h"

class WindowWidgetComboitemNode : public WindowWidgetNode {

public:

    WindowWidgetComboitemNode()
            : WindowWidgetNode()
    {

    }

    virtual ~WindowWidgetComboitemNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QComboItem* pItem =  new QComboItem("", "");

        pItem->setValue( sValue );
        QString sData = this->getAttribute(pDOMElement, "value", sValue);
        pItem->setData(sData);

        oReturn.pElement = pItem;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back( "value" );
    }


};

#endif //BIOGUI_WINDOWWIDGETCOMBOITEMNODE_H
