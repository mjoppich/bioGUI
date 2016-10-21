//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETREADIOBUTTONNODE_H
#define BIOGUI_WINDOWWIDGETREADIOBUTTONNODE_H

#include <src/app/QExtendedRadioButton.h>
#include "WindowWidgetNode.h"

class WindowWidgetRadioButtonNode : public WindowWidgetNode {

public:

    WindowWidgetRadioButtonNode()
            : WindowWidgetNode()
    {

    }

    virtual ~WindowWidgetRadioButtonNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QString sButtonValue = this->getAttribute(pDOMElement, "value", sValue);
        QExtendedRadioButton* pButtonItem = new QExtendedRadioButton(sValue, sButtonValue);

        oReturn.bHasChildrenFinished = true;
        oReturn.pElement = pButtonItem;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "value" );

    }


};

#endif //BIOGUI_WINDOWWIDGETREADIOBUTTONNODE_H
