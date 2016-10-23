//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETCHECKBOXNODE_H
#define BIOGUI_WINDOWWIDGETCHECKBOXNODE_H

#include <src/app/QExtendedCheckBox.h>
#include "WindowWidgetNode.h"

class WindowWidgetCheckBoxNode : public WindowWidgetNode {

public:

    WindowWidgetCheckBoxNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetCheckBoxNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QString sButtonValue = this->getQAttribute(pDOMElement, "value", sValue);
        QExtendedCheckBox* pButtonItem = new QExtendedCheckBox(sValue, sButtonValue);

        bool bSelectOnWindows = (this->getQAttribute(pDOMElement, "selectonwindows", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);
        bool bSelected = (this->getQAttribute(pDOMElement, "selected", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);

        if ((!bSelected) && (bSelectOnWindows))
        {
            // get current OS
            if (QSysInfo::windowsVersion() != QSysInfo::WV_None)
            {
                bSelected = true;
            }
        }

        pButtonItem->setChecked(bSelected);

        oReturn.addRetriever(this->getDomID(pDOMElement) , [pButtonItem] () {

            if (pButtonItem->isChecked())
                return pButtonItem->getValue().toStdString();

            return std::string("");

        });

        oReturn.pElement = pButtonItem;
        oReturn.bHasChildrenFinished = true;

        return oReturn;

    }




protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "selectonwindows" );
        vAttributes.push_back( "selected" );
        vAttributes.push_back( "value" );

    }


};

#endif //BIOGUI_WINDOWWIDGETCHECKBOXNODE_H
