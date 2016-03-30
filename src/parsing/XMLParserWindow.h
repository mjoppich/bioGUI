//
// Created by joppich on 3/29/16.
//

#ifndef BIOGUI_PARSEXMLWINDOW_H
#define BIOGUI_PARSEXMLWINDOW_H

#include <iostream>

#include <QtCore>
#include <QDebug>
#include <QtGui>
#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QXmlQuery>
#include <QDir>

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QPushButton>
#include <QButtonGroup>

#include <functional>
#include "XMLParser.h"

class bioGUIapp;

class XMLParserWindow : public XMLParser {

Q_OBJECT

public:

    XMLParserWindow(std::string sFileName)
    : XMLParser(sFileName)
    {

        m_pDocument = loadFromFile(sFileName);

    }

    QWidget* getWindow()
    {

        QDomElement* pWindowRoot = this->getRoot();

        bool bUnimportant;
        QWidget* pWindow = (QWidget*) createComponents(NULL, pWindowRoot, &bUnimportant);

        return pWindow;

    }

protected:

    QDomElement* getRoot()
    {
        return this->getDocumentElementByName("window");
    }



    enum ELEMENT_TYPE {ELEMENT, LAYOUT};

    ELEMENT_TYPE getElementType(QDomElement* pElement)
    {
        QString sTag = pElement->tagName();


        if (sTag.compare("hgroup", Qt::CaseInsensitive) == 0)
        {
            return ELEMENT_TYPE::LAYOUT;
        }

        if (sTag.compare("vgroup", Qt::CaseInsensitive) == 0)
        {
            return ELEMENT_TYPE::LAYOUT;
        }

        return ELEMENT_TYPE::ELEMENT;

    }


    QLayout* createLayout(QDomElement* pElement);
    QWidget* createComponent(QDomElement* pElement, bool* pChildrenFinished);

    QWidget* createComponents(QWidget* pParentWidget, QDomElement* pElement, bool* pIsElement)
    {

        if (pElement->isElement() == false)
        {
            qDebug() << pElement->tagName() << " child of " << pElement->parentNode().toElement().tagName() << " " << pElement->isElement();
            return NULL;
        }

        (*pIsElement) = (ELEMENT_TYPE::ELEMENT == getElementType(pElement));

        if (*pIsElement)
        {
            qDebug() << pElement->tagName() << " is an element";
        } else {
            qDebug() << pElement->tagName() << " is not an element";
        }

        void* pParent = NULL;
        bool bChildrenFinished = false;

        if (*pIsElement)
        {

            pParent = (void*) createComponent(pElement, &bChildrenFinished);

        } else {

            if (pParentWidget == NULL)
            {
                qDebug() << "invalid null parent widget";
            }

            pParent = (void*) createLayout(pElement);

            if (pParentWidget != NULL)
                pParentWidget->setLayout( (QBoxLayout*) pParent);

        }

        if (!bChildrenFinished)
        {
            QDomNodeList oChildren = pElement->childNodes();

            for (size_t i = 0; i < oChildren.length(); ++i)
            {

                QDomElement oChild = oChildren.item(i).toElement();

                if (oChild.isElement() == false)
                {
                    continue;
                }

                if (*pIsElement)
                {
                    bool bNotNeeded = false;
                    createComponents((QWidget*)pParent, &oChild, &bNotNeeded);
                } else {

                    bool bChildIsElement = false;
                    QWidget* pReturn = createComponents( NULL, &oChild, &bChildIsElement);

                    if (bChildIsElement == false)
                    {
                        ((QBoxLayout *) pParent)->addLayout( (QLayout*) pReturn);

                    } else {
                        ((QBoxLayout *) pParent)->addWidget( (QWidget*) pReturn);
                    }

                }
            }
        }

        return (QWidget*)pParent;

    }


    bioGUIapp* m_pApp;

};


#endif //BIOGUI_PARSEXMLWINDOW_H
