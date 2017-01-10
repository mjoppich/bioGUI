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

#ifndef BIOGUI_WINDOWWIDGETWINDOWNODE_H
#define BIOGUI_WINDOWWIDGETWINDOWNODE_H

#include "WindowWidgetNode.h"

class WindowWidgetWindowNode : public WindowWidgetNode {

public:

    WindowWidgetWindowNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
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

        QString sWidth =  this->getQAttribute(pDOMElement, "width" , "400");
        QString sHeight = this->getQAttribute(pDOMElement, "height", "400");

        int iWidth = sWidth.toInt();
        int iHeight = sHeight.toInt();

        pWindow->setMinimumSize(iWidth, iHeight);

        /*
         * Window Title
         */

        QString sTitle = this->getQAttribute(pDOMElement, "title", "bioGUI");
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
