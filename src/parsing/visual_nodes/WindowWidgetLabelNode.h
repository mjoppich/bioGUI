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
