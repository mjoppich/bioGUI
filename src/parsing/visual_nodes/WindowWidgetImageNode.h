//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETIMAGENODE_H
#define BIOGUI_WINDOWWIDGETIMAGENODE_H

#include <QtWidgets/QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "WindowWidgetNode.h"

class WindowWidgetImageNode : public WindowWidgetNode {

public:

    WindowWidgetImageNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetImageNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QString sFileName = this->getQAttribute(pDOMElement, "src", "");

        QGraphicsScene* pScene = new QGraphicsScene();
        QGraphicsView* pView = new QGraphicsView(pScene);
        QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(QPixmap( sFileName ));

        pScene->addItem( pItem );

        /*
         * apply sizes here
         *
         */

        QString sWidth =  this->getQAttribute(pDOMElement, "width" , "");
        QString sHeight = this->getQAttribute(pDOMElement, "height", "");

        if ((sWidth.length() > 0) && (sHeight.length() > 0))
        {
            int iWidth = sWidth.toInt();
            int iHeight = sHeight.toInt();

            pView->setFixedSize(iWidth, iHeight);

        }

        oReturn.bHasChildrenFinished = true;
        oReturn.pElement = pView;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "width" );
        vAttributes.push_back( "height" );
        vAttributes.push_back( "src" );

    }


};

#endif //BIOGUI_WINDOWWIDGETIMAGENODE_H
