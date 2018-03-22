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

#ifndef BIOGUI_WINDOWLAYOUTVERTICALNODE_H
#define BIOGUI_WINDOWLAYOUTVERTICALNODE_H


#include <QtWidgets/QVBoxLayout>
#include <src/app/QOrderedLayout.h>
#include "WindowLayoutNode.h"

class WindowLayoutVerticalNode : public WindowLayoutNode {

public:

    WindowLayoutVerticalNode(WindowComponentFactory* pFactory)
            : WindowLayoutNode(pFactory)
    {

    }

    virtual ~WindowLayoutVerticalNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QVBoxLayout *pLayout = new QOrderedVBoxLayout();

        //pLayout->setAlignment(Qt::AlignVertical_Mask);
        pLayout->setSizeConstraint(QLayout::SetDefaultConstraint); //SetFixedSize


        CreatedElement oReturn;
        oReturn.pElement = pLayout;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {


    }


};

#endif //BIOGUI_WINDOWLAYOUTVERTICALNODE_H
