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

#ifndef BIOGUI_WINDOWWIDGETFILESELECTBOXNODE_H
#define BIOGUI_WINDOWWIDGETFILESELECTBOXNODE_H

#include <src/app/QSortableFileList.h>
#include "WindowWidgetNode.h"

class WindowWidgetFileSelectBoxNode : public WindowWidgetNode {

public:

    WindowWidgetFileSelectBoxNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetFileSelectBoxNode()
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
            pDOMElement->setAttribute("selected", QString(sValue.c_str()));
        }

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QString sFileFilter = this->getQAttribute(pDOMElement, "filter", "");
        QString sFilePath = this->getQAttribute(pDOMElement, "location", "");
        QString sDelimeter = this->getQAttribute(pDOMElement, "delim", " ");

        QString sSelected = this->getQAttribute(pDOMElement, "selected", "");
        QStringList vSelected;

        if (sSelected.size() > 0)
        {
            vSelected = sSelected.split(sDelimeter);
        }

        QSortableFileList *pList = new QSortableFileList( sFilePath, sFileFilter, sDelimeter, &vSelected );



        oReturn.pElement = pList;

        oReturn.addRetriever( this->getDomID(pDOMElement), [pList] () {return pList->evaluate().toStdString();} );

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "filter" );
        vAttributes.push_back( "location" );
        vAttributes.push_back( "delim" );

    }


};

#endif //BIOGUI_WINDOWWIDGETFILESELECTBOXNODE_H
