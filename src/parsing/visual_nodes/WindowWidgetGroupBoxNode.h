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
#ifndef BIOGUI_WINDOWWIDGETGROUPBOXNODE_H
#define BIOGUI_WINDOWWIDGETGROUPBOXNODE_H

#include <src/app/QAbstractButtonItem.h>
#include <src/app/QExtGridLayout.h>
#include "WindowWidgetNode.h"
#include "WindowWidgetGeneralGroupNode.h"

class WindowWidgetGroupBoxNode : public WindowWidgetGeneralGroupNode {

public:

    WindowWidgetGroupBoxNode(WindowComponentFactory* pFactory)
            : WindowWidgetGeneralGroupNode(pFactory)
    {

    }

    virtual ~WindowWidgetGroupBoxNode()
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
                QStringList oLayouts;
                oLayouts << "VLAYOUT" << "GRID" << "HLAYOUT";

                if ( oLayouts.contains(oChildNode.tagName().toUpper()) )
                {
                    // create layout


                }

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
                            iCols = pChildren->size() * 2;

                            break;

                        case LAYOUT_TYPE::VERTICAL:

                            iRows = pChildren->size() * 2;
                            iCols = 2;

                            break;

                        case LAYOUT_TYPE::GRID:


                            iRows = pTestLayout->getRows();
                            iCols = pTestLayout->getCols() * 2;

                            delete pTestLayout;

                            break;

                        default:

                            std::cout << "default to vertical layout" << std::endl;

                            eLayoutType = LAYOUT_TYPE::VERTICAL;
                            iRows = pChildren->size() * 2;
                            iCols = 2;

                    }

                    pReturn = new QExtGridLayout(iRows, iCols);

                    return pReturn;
                }
            }


            // defaults a vertical layout
            int iRows = pChildren->size();
            return (QLayout*) new QExtGridLayout(iRows, 2);
        };

        QButtonGroup* pButtonGroup = new QButtonGroup();

        std::function<void (QExclusiveGroupBox*, QLayout*, QWidget*, QWidget*, QStringList*, int)> oPostProc = [pButtonGroup] (QExclusiveGroupBox* pBox,
                                                                                                                               QLayout* pLayout,
                                                                                                                               QWidget* pChildWidget,
                                                                                                                               QWidget* pTransWidget,
                                                                                                                               QStringList* pSelected,
                                                                                                                               int iElement) {

            if (QAbstractButtonItem* pButtonItem = dynamic_cast<QAbstractButtonItem *>(pChildWidget))
            {
                if ((iElement == 0) || ( pSelected->contains(pButtonItem->getValue()) ))
                {

                    QAbstractButton* pButton = dynamic_cast<QAbstractButton*>(pChildWidget);

                    pButton->setChecked(true);

                }

                pButtonGroup->addButton((QAbstractButton*) pChildWidget, iElement);
            }

        };


        std::function<QList<QWidget*> (QDomElement*, bool*, int)> oPreProc = [this,pButtonGroup] (QDomElement* pChildNode, bool* pBoolean, int iElement) {


            QDomText oSimpleTextNode = pChildNode->childNodes().at(0).toText();

            // default: only a simple radio/check
            if (!oSimpleTextNode.isNull())
            {
                QAbstractButton* pBox = (QAbstractButton*) m_pFactory->createWidgetElement(pChildNode).pElement;
                QString sText = pBox->text();
                pBox->setText("");

                pButtonGroup->addButton(pBox, iElement);

                QList<QWidget*> vWidgets;
                vWidgets.append(pBox);
                vWidgets.append(new QLabel(sText));

                return vWidgets;
            }

            QAbstractButton* pBox = (QAbstractButton*) m_pFactory->createWidgetElement(pChildNode).pElement;
            pBox->setText("");
            pButtonGroup->addButton(pBox, iElement);

            QWidget* pContentTransporter = new QWidget();
            QHBoxLayout* pContentTransporterLayout = new QHBoxLayout();

            QDomNodeList oChildren = pChildNode->childNodes();

            for (size_t i = 0; i < oChildren.size(); ++i)
            {

                QDomElement oChildElement = pChildNode->childNodes().at(i).toElement();
                QWidget* pContent = m_pFactory->createWidgetElement(&oChildElement).pElement;

                pContentTransporterLayout->addWidget(pContent);

            }

            pContentTransporter->setLayout(pContentTransporterLayout);

            QList<QWidget*> vWidgets;
            vWidgets.append(pBox);
            vWidgets.append(pContentTransporter);

            return vWidgets;

        };


        oReturn = this->createGeneralGroup(pDOMElement, oLayoutFunc, oPreProc, oPostProc);

        QExclusiveGroupBox* pGroupBox = (QExclusiveGroupBox*) oReturn.pElement;
        oReturn.addRetriever( this->getDomID(pDOMElement), [pButtonGroup, pGroupBox] () {


            bool bEvaluate = true;
            if (pGroupBox->isCheckable())
            {
                bEvaluate = pGroupBox->isChecked();
            }

            if (bEvaluate)
            {

                QAbstractButton* pButton = pButtonGroup->checkedButton();
                QAbstractButtonItem* pButtonItem = dynamic_cast<QAbstractButtonItem *>(pButton);

                if (pButtonItem)
                {
                    return pButtonItem->getValue().toStdString();
                }

            }

            return std::string("");

        });

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "multi" );

    }


};

#endif //BIOGUI_WINDOWWIDGETGROUPBOXNODE_H
