//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETINPUTNODE_H
#define BIOGUI_WINDOWWIDGETINPUTNODE_H

#include <QtWidgets/QTextEdit>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include "WindowWidgetNode.h"

class WindowWidgetInputNode : public WindowWidgetNode {

public:

    WindowWidgetInputNode()
    : WindowWidgetNode()
    {

    }

    virtual ~WindowWidgetInputNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        bool bMultiLine = ( this->getAttribute(pDOMElement, "multi", "false").compare("true", Qt::CaseInsensitive) == 0);

        if (!bMultiLine)
        {
            QLineEdit *pLineEdit = new QLineEdit( sValue );

            QString sType = this->getAttribute(pDOMElement, "type", "");

            if (sType.size() != 0)
            {

                if (sType.compare("string", Qt::CaseInsensitive) == 0)
                {
                    // nothing
                }

                if (sType.compare("password", Qt::CaseInsensitive) == 0)
                {
                    pLineEdit->setEchoMode(QLineEdit::Password);

                }

                if (sType.compare("int", Qt::CaseInsensitive) == 0)
                {
                    pLineEdit->setValidator( new QIntValidator() );
                }

                if (sType.compare("float", Qt::CaseInsensitive) == 0)
                {
                    pLineEdit->setValidator( new QDoubleValidator() );
                }

            }

            oReturn.pElement = pLineEdit;
            oReturn.oRetriever = [pLineEdit] () {return pLineEdit->text().toStdString();};

        } else {

            QTextEdit* pTextEdit = new QTextEdit(sValue);

            oReturn.oRetriever = [pTextEdit] () {return pTextEdit->toPlainText().toStdString();};
            oReturn.pElement = pTextEdit;

        }

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "multi" );
        vAttributes.push_back( "type {string, int, float, password}" );

    }


};


#endif //BIOGUI_WINDOWWIDGETINPUTNODE_H
