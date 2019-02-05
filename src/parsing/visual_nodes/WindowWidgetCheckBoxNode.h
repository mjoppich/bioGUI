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

    virtual void saveInQDomElement(QDomElement* pDOMElement,
                                   std::map<std::string, std::function< std::string() > >* pID2Value,
                                   QDomDocument* pDoc)
    {

        std::string sID = this->getAttribute(pDOMElement, "ID", "");
        std::map<std::string, std::function< std::string() > >::iterator oFind = pID2Value->find(sID);

        if (oFind != pID2Value->end())
        {
            std::string sValue = oFind->second();

            QString sButtonValue = this->getQAttribute(pDOMElement, "selected", "");

            if ((sButtonValue.size() > 0) && (sButtonValue.compare(QString(sValue.c_str()), Qt::CaseInsensitive) == 0))
            {
                pDOMElement->setAttribute("selected", "true");
            } else {
                pDOMElement->setAttribute("selected", "false");
            }


        }

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
        std::string sValueUnselected = this->getAttribute(pDOMElement, "value-unselected", "");


        if ((!bSelected) && (bSelectOnWindows))
        {
            // get current OS
            if (QSysInfo::windowsVersion() != QSysInfo::WV_None)
            {
                bSelected = true;
            }
        }

        pButtonItem->setChecked(bSelected);

        oReturn.addRetriever(this->getDomID(pDOMElement) , [pButtonItem, sValueUnselected] () {

            if (pButtonItem->isChecked())
                return pButtonItem->getValue().toStdString();

            return sValueUnselected;

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
        vAttributes.push_back( "value-unselected" );

    }


};

#endif //BIOGUI_WINDOWWIDGETCHECKBOXNODE_H
