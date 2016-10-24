//
// Created by joppich on 10/21/16.
//

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

        bioGUIapp* pApp = this->m_pApp;

        pAction->connect(pAction,&QAbstractButton::clicked,[pApp, sProgramToRun] (bool bChecked){

            std::string sTmp = sProgramToRun;

            pApp->runProgram( sTmp );

        });

        oReturn.pElement = pAction;

        m_pFactory->getApp()->getWindowParser()->addAction(pAction);

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "program" );

    }

    bioGUIapp* m_pApp;
    std::vector<QPushButton*>* m_pActionButtons;


};

#endif //BIOGUI_WINDOWWIDGETACTIONNODE_H
