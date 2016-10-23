//
// Created by joppich on 10/21/16.
//

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

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QString sFileFilter = this->getQAttribute(pDOMElement, "filter", "");
        QString sFilePath = this->getQAttribute(pDOMElement, "location", "");
        QString sDelimeter = this->getQAttribute(pDOMElement, "delim", " ");

        QSortableFileList *pList = new QSortableFileList( sFilePath, sFileFilter, sDelimeter );


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
