//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETWINDOWNODE_H
#define BIOGUI_WINDOWWIDGETWINDOWNODE_H

#include "WindowWidgetNode.h"

class WindowWidgetWindowNode : public WindowWidgetNode {

public:

    WindowWidgetWindowNode()
            : WindowWidgetNode()
    {

    }

    virtual ~WindowWidgetWindowNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QWidget *pWindow = new QWidget();

        /*
         * apply sizes here
         *
         */

        QString sWidth =  this->getAttribute(pDOMElement, "width" , "400");
        QString sHeight = this->getAttribute(pDOMElement, "height", "400");

        int iWidth = sWidth.toInt();
        int iHeight = sHeight.toInt();

        pWindow->setMinimumSize(iWidth, iHeight);

        /*
         * Window Title
         */

        QString sTitle = this->getAttribute(pDOMElement, "title", "bioGUI");
        if (sTitle.length() > 0)
        {
            pWindow->setWindowTitle( sTitle );
        }

        oReturn.pElement = pWindow;

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

#endif //BIOGUI_WINDOWWIDGETWINDOWNODE_H
