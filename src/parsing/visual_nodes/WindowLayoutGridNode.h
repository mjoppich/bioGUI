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

#ifndef BIOGUI_WINDOWLAYOUTGRIDNODE_H
#define BIOGUI_WINDOWLAYOUTGRIDNODE_H

#include <src/app/QExtGridLayout.h>
#include "WindowLayoutNode.h"

class WindowLayoutGridNode : public WindowLayoutNode {

public:

    WindowLayoutGridNode(WindowComponentFactory* pFactory)
            : WindowLayoutNode(pFactory)
    {

    }

    virtual ~WindowLayoutGridNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        int iRows = this->getQAttribute(pDOMElement, "rows", 0).toInt();
        int iCols = this->getQAttribute(pDOMElement, "cols", 0).toInt();

        QExtGridLayout *pLayout = new QExtGridLayout(iRows, iCols);

        pLayout->setAlignment(Qt::AlignVertical_Mask);

        CreatedElement oReturn;

        oReturn.pElement = pLayout;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("rows");
        vAttributes.push_back("cols");
    }


};

#endif //BIOGUI_WINDOWLAYOUTGRIDNODE_H
