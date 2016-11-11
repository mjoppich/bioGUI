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

    WindowWidgetInputNode(WindowComponentFactory* pFactory)
    : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetInputNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sValue = pDOMElement->text();
        std::string sID = this->getAttribute(pDOMElement, "ID", "");

        CreatedElement oReturn;

        bool bMultiLine = ( this->getQAttribute(pDOMElement, "multi", "false").compare("true", Qt::CaseInsensitive) == 0);

        if (!bMultiLine)
        {
            QLineEdit *pLineEdit = new QLineEdit( sValue );

            QString sType = this->getQAttribute(pDOMElement, "type", "");

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

                    if (this->hasAttribute(pDOMElement, "min") && this->hasAttribute(pDOMElement, "max"))
                    {
                        int iMin = this->getQAttribute(pDOMElement, "min", "0").toInt();
                        int iMax = this->getQAttribute(pDOMElement, "max", "0").toInt();

                        pLineEdit->setValidator( new QIntValidator(iMin, iMax) );

                    } else {

                        pLineEdit->setValidator( new QIntValidator() );

                    }

                }

                if (sType.compare("float", Qt::CaseInsensitive) == 0)
                {
                    if (this->hasAttribute(pDOMElement, "min") && this->hasAttribute(pDOMElement, "max"))
                    {
                        double dMin = this->getQAttribute(pDOMElement, "min", "0").toDouble();
                        double dMax = this->getQAttribute(pDOMElement, "max", "0").toDouble();

                        pLineEdit->setValidator( new QDoubleValidator(dMin, dMax, 64) );

                    } else {
                        pLineEdit->setValidator(new QDoubleValidator());
                    }
                }

            }

            oReturn.pElement = pLineEdit;
            oReturn.addRetriever(this->getDomID(pDOMElement) , [pLineEdit] () {return pLineEdit->text().toStdString();} );

        } else {

            QTextEdit* pTextEdit = new QTextEdit(sValue);

            oReturn.addRetriever(this->getDomID(pDOMElement) , [pTextEdit] () {return pTextEdit->toPlainText().toStdString();} );
            oReturn.pElement = pTextEdit;

        }

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "multi" );
        vAttributes.push_back( "min && max" );
        vAttributes.push_back( "type {string, int, float, password}" );

    }


};


#endif //BIOGUI_WINDOWWIDGETINPUTNODE_H
