//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETSTREAMNODE_H
#define BIOGUI_WINDOWWIDGETSTREAMNODE_H

#include <QtWidgets/QCheckBox>
#include "WindowWidgetNode.h"

class WindowWidgetStreamNode : public WindowWidgetNode {

public:

    WindowWidgetStreamNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetStreamNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QCheckBox* pCheckbox = new QCheckBox(sValue);

        oReturn.pElement = pCheckbox;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "width" );
        vAttributes.push_back( "height" );
        vAttributes.push_back( "title" );

    }


};

#endif //BIOGUI_WINDOWWIDGETSTREAMNODE_H
