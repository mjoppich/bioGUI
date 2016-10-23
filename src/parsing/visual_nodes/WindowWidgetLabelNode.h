//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETLABELNODE_H
#define BIOGUI_WINDOWWIDGETLABELNODE_H

#include <QtWidgets/QLabel>
#include <src/app/QClickableLabel.h>
#include <QtGui/QDesktopServices>
#include <QUrl>
#include "WindowWidgetNode.h"

class WindowWidgetLabelNode : public WindowWidgetNode {

public:

    WindowWidgetLabelNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetLabelNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;
        QClickableLabel *pLabel = new QClickableLabel( sValue );

        if (this->hasAttribute(pDOMElement, "link"))
        {
            pLabel->setStyleSheet("QLabel { color : blue; }");
            QObject::connect(pLabel,&QClickableLabel::clicked,[pLabel] (){

                QDesktopServices::openUrl(QUrl(pLabel->text()));

            });
        }

        oReturn.pElement = pLabel;
        oReturn.vRetriever.push_back( {this->getAttribute(pDOMElement, "id", ""), [sValue] () {return sValue.toStdString();}} );

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "link" );

    }


};

#endif //BIOGUI_WINDOWWIDGETLABELNODE_H
