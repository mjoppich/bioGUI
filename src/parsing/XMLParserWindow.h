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

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include <functional>
#include <map>
#include "XMLParser.h"

class bioGUIapp;

class XMLParserWindow : public XMLParser {

Q_OBJECT

public:

    XMLParserWindow(std::string sFileName)
    : XMLParser(sFileName)
    {

        m_pID2Value = new std::map<std::string, std::function< std::string()> >();

        m_pKnownTags->push_back("hgroup");
        m_pKnownTags->push_back("vgroup");
        m_pKnownTags->push_back("label");
        m_pKnownTags->push_back("input");
        m_pKnownTags->push_back("image");
        m_pKnownTags->push_back("filedialog");
        m_pKnownTags->push_back("group");
        m_pKnownTags->push_back("radiobutton");
        m_pKnownTags->push_back("checkbox");
        m_pKnownTags->push_back("action");
        m_pKnownTags->push_back("window");

        m_pDocument = loadFromFile(sFileName);


    }


    void setActionsEnabled(bool bEnabled)
    {

        for (size_t i = 0; i < m_vActions.size(); ++i)
        {
            m_vActions.at(i)->setEnabled( bEnabled );
        }

    }

    QWidget* getWindow()
    {

        QDomElement* pWindowRoot = this->getRoot( m_pDocument );

        m_vWidgets.clear();
        m_vActions.clear();
        m_pID2Widget->clear();
        m_pID2Value->clear();

        bool bUnimportant;
        QWidget* pWindow = (QWidget*) createComponents(NULL, pWindowRoot, &bUnimportant);

        return pWindow;

    }

    std::map<std::string, QWidget* >* getID2Widget()
    {
        return m_pID2Widget;
    }

    std::map<std::string, std::function< std::string()> >* getID2Value()
    {
        return m_pID2Value;
    }

protected:

    virtual QDomElement* getRoot(QDomDocument* pDocument)
    {
        return this->getDocumentElementByName(pDocument, "window");
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

    bool addValueFetcher(QDomElement* pElement, std::function<std::string()> oFunc)
    {

        QString sID = this->getAttribute(pElement, "ID", "");

        if (sID.length() == 0)
        {
            std::cerr << "expected element id for " << pElement->tagName().toStdString() << " but none was given";

            return false;
        }



        m_pID2Value->insert( std::pair< std::string, std::function<std::string()> >( sID.toStdString(), oFunc ) );

        return true;

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

    std::map<std::string, std::function< std::string()> >* m_pID2Value;
    std::map<std::string, QWidget*>* m_pID2Widget;
    std::vector<QWidget*> m_vWidgets;
    std::vector<QPushButton*> m_vActions;

};


#endif //BIOGUI_PARSEXMLWINDOW_H
