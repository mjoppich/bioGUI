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

#ifndef BIOGUI_WINDOWWIDGETACTIONNODE_H
#define BIOGUI_WINDOWWIDGETACTIONNODE_H

#include <QtWidgets/QPushButton>
#include <src/bioGUIapp.h>
#include "WindowWidgetNode.h"

class WindowWidgetActionNode : public WindowWidgetNode {

public:

    WindowWidgetActionNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetActionNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QPushButton *pAction = new QPushButton( sValue );

        std::string sProgramToRun = this->getAttribute(pDOMElement, "program", "");

        bioGUIapp* pApp = this->m_pFactory->getApp();

        pAction->connect(pAction,&QAbstractButton::clicked,[pApp, sProgramToRun] (bool bChecked){

            std::string sTmp = sProgramToRun;

            pApp->runProgram( sTmp );

        });

        oReturn.pElement = pAction;
        oReturn.pWidgetFuncNode = new WidgetFunctionNode(pAction);

        m_pFactory->getApp()->getWindowParser()->addAction(pAction);

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "program" );

    }

    std::vector<QPushButton*>* m_pActionButtons = NULL;


};

#endif //BIOGUI_WINDOWWIDGETACTIONNODE_H
