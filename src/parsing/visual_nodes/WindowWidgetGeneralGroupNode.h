//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETGENERALGROUP_H
#define BIOGUI_WINDOWWIDGETGENERALGROUP_H

#include <src/app/QExclusiveGroupBox.h>
#include <iostream>
#include "WindowWidgetNode.h"

class WindowWidgetGeneralGroupNode : public WindowWidgetNode {

public:

    WindowWidgetGeneralGroupNode(WindowComponentFactory* pFactory)
    : WindowWidgetNode(pFactory)
    {

    }

protected:

    enum LAYOUT_TYPE {NONE, VERTICAL, HORIZONTAL, GRID, UNKNOWN};

    LAYOUT_TYPE getLayoutType(QDomElement* pElement)
    {
        QLayout* pLayout = m_pFactory->createLayoutElement(pElement).pElement;

        std::cerr << pLayout << std::endl;

        if (pLayout == NULL)
            return LAYOUT_TYPE::NONE;

        if (QOrderedHBoxLayout* pOrderedLayout = dynamic_cast<QOrderedHBoxLayout*>(pLayout))
        {
            return LAYOUT_TYPE::HORIZONTAL;
        }

        if (QOrderedVBoxLayout* pOrderedLayout = dynamic_cast<QOrderedVBoxLayout*>(pLayout))
        {
            return LAYOUT_TYPE::VERTICAL;
        }

        if (QExtGridLayout* pOrderedLayout = dynamic_cast<QExtGridLayout*>(pLayout))
        {
            return LAYOUT_TYPE::GRID;
        }

        return LAYOUT_TYPE::UNKNOWN;
    }

    WindowNode<QWidget>::CreatedElement createGeneralGroup(QDomElement* pElement,
                                std::function<QLayout* (QDomElement*, QDomNodeList*, bool*)> oLayoutFunc,
                                std::function<QList<QWidget*> (QDomElement*, bool*, int)> oPreProcFunc = [] (QDomElement* pChildNode, bool* pBoolean, int iElement) {return QList<QWidget*>();},
                                std::function<void (QExclusiveGroupBox*, QLayout*, QWidget*, QWidget*, QStringList*, int)> oPostProcFunc = [] (QExclusiveGroupBox* pBox, QLayout* pLay, QWidget* pWid1, QWidget* pWid2, QStringList* pList, int iElement) {})
    {

        WindowNode<QWidget>::CreatedElement oReturn;
        oReturn.bHasChildrenFinished = true;

        QExclusiveGroupBox* pGroupBox = new QExclusiveGroupBox("");

        /*
         * Window Title
         */
        QString sTitle = this->getQAttribute(pElement, "title", "");
        pGroupBox->setTitle( sTitle );

        if (sTitle.compare("Step options") == 0)
        {
            std::cout << "Sliding Window";
        }

        /*
         * checkable?
         */
        std::string sCheckable = this->getQAttribute(pElement, "checkable", "false").toUpper().toStdString();
        if (sCheckable.compare("TRUE") == 0)
        {
            pGroupBox->setCheckable(true);
            pGroupBox->setChecked(false);

            std::string sCheckedValue = this->getAttribute(pElement, "checked_value", "true");
            std::string sUncheckedValue = this->getAttribute(pElement, "unchecked_value", "false");

            oReturn.addRetriever( this->getDomID(pElement), [pGroupBox, sCheckedValue, sUncheckedValue] () {

                if (pGroupBox->isChecked())
                {
                    return sCheckedValue;
                } else {
                    return sUncheckedValue;
                }

            });

        }

        /*
         * exclusive?
         */
        QString sExclusive = this->getQAttribute(pElement, "exclusive", "false");
        if (sExclusive.compare("TRUE", Qt::CaseInsensitive) == 0)
        {
            pGroupBox->setExclusive(true);
        } else {
            pGroupBox->setExclusive(false);
        }

        /*
         * ordered
         *
         */

        bool bOrdered = (this->getQAttribute(pElement, "ordered", "false").compare("true", Qt::CaseInsensitive) == 0);
        pGroupBox->setOrdered(bOrdered);

        /*
         * state?
         *
         * selected = true/false may indicate that group is selected.
         *
         * selected = [ids] may indicate that group is selected and children are selected
         *
         */
        QString sSelected = this->getQAttribute(pElement, "selected", "");
        QStringList vSelected;

        if (pGroupBox->isCheckable())
        {
            if (sSelected.compare("TRUE", Qt::CaseInsensitive) == 0)
            {
                pGroupBox->setChecked(true);
            } else if (sSelected.compare("FALSE", Qt::CaseInsensitive) == 0)
            {
                pGroupBox->setChecked(false);
            }

        } else {

            pGroupBox->setChecked(true);

            if (sSelected.length() > 0)
            {
                vSelected = sSelected.split(";");
            }

        }

        /*
         * LAYOUT
         */

        bool bHadLayout = false;

        QDomNodeList oChildren = pElement->childNodes();
        QLayout* pLayout = oLayoutFunc(pElement, &oChildren, &bHadLayout);

        if (bHadLayout)
        {
            // oChildren[0] is a layout node

            std::cerr << oChildren.size() << std::endl;
        }

        /*
         *
         * CREATE AND ADD CHILDREN
         *
         */

        bool bBoolean = false;
        int iAdded = 0;

        for (size_t i = 0; i < oChildren.size(); ++i)
        {
            bBoolean = false;

            QDomElement oChildNode = oChildren.at(i).toElement();
            QList<QWidget*> vChildren = oPreProcFunc(&oChildNode, &bBoolean, iAdded);

            if (vChildren.size() == 0)
            {
                WindowNode<QWidget>::CreatedElement oCreated = m_pFactory->createWidgetElement(&oChildNode);

                if (oCreated.pElement == NULL)
                {
                    continue;
                }

                for (Retriever& oRet : oCreated.vRetriever)
                {
                    oReturn.vRetriever.push_back(oRet);
                }

                if (oCreated.pElement != NULL)
                    vChildren.append(oCreated.pElement);
            }


            if (vChildren.size() == 0)
            {
                std::cout << "error in creating groupbox components: " + pElement->text().toStdString() << std::endl;
                std::cout << "error in creating groupbox components: " + oChildNode.text().toStdString() << std::endl;
                //throw "error in creating groupbox components: " + oChildNode.text().toStdString();

                continue;
            }

            for (size_t i = 0; i < vChildren.size(); ++i)
            {

                QWidget* pChildElement = vChildren.at(i);
                QWidget* pTransformedChildElement = pGroupBox->addNextWidget(pChildElement);

                m_pFactory->addToLayout(pLayout, pTransformedChildElement);

                std::string sID = this->getDomID(&oChildNode);
                m_pFactory->getApp()->getWindowParser()->addID2Widget(sID, pTransformedChildElement, true);

                oPostProcFunc(pGroupBox, pLayout, pChildElement, pTransformedChildElement, &vSelected, iAdded);
            }


            ++iAdded;

        }

        pGroupBox->setLayout(pLayout);


        // Finally set Layout
        QWidget* pComponentWidget = new QWidget();

        QLayout* pComponentLayout = new QHBoxLayout();
        pComponentLayout->addWidget(pGroupBox);
        pComponentWidget->setLayout(pComponentLayout);

        /*
         * AFTER SETTING LAYOUT CARE FOR VISIBILITY
         */

        if (pGroupBox->isCheckable() == true)
        {


            std::string sVisible = this->getQAttribute(pElement, "visible", "ALWAYS").toUpper().toStdString();

            if (sVisible.compare("ALWAYS") != 0)
            {
                QObject::connect( pGroupBox, &QExclusiveGroupBox::toggled, [pGroupBox, pComponentWidget] () {

                    if (pGroupBox->isCheckable())
                    {

                        if (pGroupBox->isChecked())
                        {
                            pComponentWidget->setVisible(true);
                        } else {
                            pComponentWidget->setVisible(false);
                        }

                    }

                } );

                if (sVisible.compare("FALSE") == 0)
                {
                    pGroupBox->setChecked(false);
                } else if (sVisible.compare("TRUE"))
                {
                    pGroupBox->setChecked(true);
                }

            }

        }

        pGroupBox->getConsistent();
        oReturn.pElement = pGroupBox;


        return oReturn;
    }



};


#endif //BIOGUI_WINDOWWIDGETGENERALGROUP_H
