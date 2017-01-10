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

#ifndef BIOGUI_WINDOWWIDGETGROUPNODE_H
#define BIOGUI_WINDOWWIDGETGROUPNODE_H

#include <QtWidgets/QVBoxLayout>
#include "WindowWidgetNode.h"
#include "WindowWidgetGeneralGroupNode.h"
#include "WindowComponentFactory.h"

class WindowWidgetGroupNode : public WindowWidgetGeneralGroupNode {

public:

    WindowWidgetGroupNode(WindowComponentFactory* pFactory)
            : WindowWidgetGeneralGroupNode(pFactory)
    {

    }

    virtual ~WindowWidgetGroupNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;


        std::function<QLayout* (QDomElement*, QDomNodeList*, bool*)> oLayoutFunc = [this] (QDomElement* pDElement, QDomNodeList* pChildren, bool* bHadLayout) {

            QLayout* pReturn = NULL;

            if (pChildren->size() == 1)
            {
                QDomElement oChildNode = pChildren->at(0).toElement();

                LAYOUT_TYPE eLayoutType = this->getLayoutType(&oChildNode);

                std::cout << eLayoutType << std::endl;

                if (LAYOUT_TYPE::NONE != eLayoutType)
                {

                    *bHadLayout = true;


                    *pChildren = QDomNodeList(pChildren->at(0).childNodes());

                    int iRows = 0;
                    int iCols = 0;

                    QExtGridLayout* pTestLayout = NULL;
                    if (eLayoutType == LAYOUT_TYPE::GRID)
                        pTestLayout = (QExtGridLayout*) m_pFactory->createLayoutElement(&oChildNode).pElement;


                    switch ( eLayoutType )
                    {
                        case LAYOUT_TYPE::HORIZONTAL:

                            iRows = 1;
                            iCols = pChildren->size();

                            break;

                        case LAYOUT_TYPE::VERTICAL:

                            iRows = pChildren->size();
                            iCols = 2;

                            break;

                        case LAYOUT_TYPE::GRID:


                            iRows = pTestLayout->getRows();
                            iCols = pTestLayout->getCols();

                            delete pTestLayout;

                            break;

                        default:

                            std::cout << "default to vertical layout" << std::endl;

                            eLayoutType = LAYOUT_TYPE::VERTICAL;
                            iRows = pChildren->size();
                            iCols = 1;

                    }

                    pReturn = new QExtGridLayout(iRows, iCols);

                    return pReturn;
                }
            }

            // defaults a vertical layout
            int iRows = pChildren->size();
            return (QLayout*) new QExtGridLayout(iRows, 1);
        };

        oReturn = this->createGeneralGroup(pDOMElement, oLayoutFunc);

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

#endif //BIOGUI_WINDOWWIDGETGROUPNODE_H
