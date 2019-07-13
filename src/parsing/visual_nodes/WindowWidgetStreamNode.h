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

        bool bSelected = true;
        if (this->getQAttribute(pDOMElement, "active", "").compare("FALSE", Qt::CaseInsensitive) == 0)
        {
            bSelected=false;
        }
        pCheckbox->setChecked(bSelected);

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
