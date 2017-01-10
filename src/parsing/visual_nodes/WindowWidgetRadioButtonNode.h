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

#ifndef BIOGUI_WINDOWWIDGETREADIOBUTTONNODE_H
#define BIOGUI_WINDOWWIDGETREADIOBUTTONNODE_H

#include <src/app/QExtendedRadioButton.h>
#include "WindowWidgetNode.h"

class WindowWidgetRadioButtonNode : public WindowWidgetNode {

public:

    WindowWidgetRadioButtonNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
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

        QString sButtonValue = this->getQAttribute(pDOMElement, "value", sValue);
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
