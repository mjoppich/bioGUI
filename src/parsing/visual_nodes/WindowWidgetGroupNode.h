//
// Created by joppich on 10/21/16.
//

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
