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

    WindowWidgetGroupNode()
            : WindowWidgetNode()
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


        std::function<QLayout* (QDomElement*, QDomNodeList*)> oLayoutFunc = [this] (QDomElement* pDElement, QDomNodeList* pChildren) {

            QLayout* pReturn = NULL;

            if (pChildren->size() == 1)
            {
                QDomElement oChildNode = pChildren->at(0).toElement();

                WindowComponentFactory oFactory(NULL);

                if ( oFactory.isLayout(&oChildNode) )
                {

                    pReturn = oFactory.createLayoutElement(&oChildNode).pElement;

                    // TODO does this do the trick?
                    *pChildren = QDomNodeList(pChildren->at(0).childNodes());

                    return pReturn;
                }
            }


            return (QLayout*) new QVBoxLayout();
        };

        QWidget* pReturn = this->createGeneralGroup(pElement, pChildrenFinished, oLayoutFunc);

        oReturn.pElement = pReturn;

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
