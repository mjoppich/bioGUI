//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETFILESELECTBOXNODE_H
#define BIOGUI_WINDOWWIDGETFILESELECTBOXNODE_H

#include <src/app/QSortableFileList.h>
#include "WindowWidgetNode.h"

class WindowWidgetFileSelectBoxNode : public WindowWidgetNode {

public:

    WindowWidgetFileSelectBoxNode()
            : WindowWidgetNode()
    {

    }

    virtual ~WindowWidgetFileSelectBoxNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QString sFileFilter = this->getAttribute(pDOMElement, "filter", "");
        QString sFilePath = this->getAttribute(pDOMElement, "location", "");
        QString sDelimeter = this->getAttribute(pDOMElement, "delim", " ");

        QSortableFileList *pList = new QSortableFileList( sFilePath, sFileFilter, sDelimeter );


        oReturn.pElement = pList;
        oReturn.oRetriever = [pList] () {return pList->evaluate().toStdString();};
        oReturn.bHasRetriever = true;


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
