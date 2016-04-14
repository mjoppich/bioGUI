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
#include <src/app/QExtGridLayout.h>
#include "XMLParser.h"

class bioGUIapp;

class XMLParserWindow : public XMLParser {

Q_OBJECT

public:

    XMLParserWindow(bioGUIapp* pApp, std::string sFileName)
    : XMLParser(sFileName)
    {

        m_pApp = pApp;

        m_pID2Value = new std::map<std::string, std::function< std::string()> >();
        m_pID2Widget = new std::map<std::string, QWidget* >();

        m_pKnownTags->push_back("hgroup");
        m_pKnownTags->push_back("vgroup");
        m_pKnownTags->push_back("grid");

        m_pKnownTags->push_back("label");
        m_pKnownTags->push_back("input");
        m_pKnownTags->push_back("image");
        m_pKnownTags->push_back("filedialog");
        m_pKnownTags->push_back("group");
        m_pKnownTags->push_back("groupbox");
        m_pKnownTags->push_back("radiobutton");
        m_pKnownTags->push_back("checkbox");
        m_pKnownTags->push_back("action");
        m_pKnownTags->push_back("window");
        m_pKnownTags->push_back("stream");
        m_pKnownTags->push_back("streambox");
        m_pKnownTags->push_back("combobox");
        m_pKnownTags->push_back("comboitem");

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

    void replaceElements(QDomElement* pElement)
    {
        qDebug() << pElement->tagName() << " child of " << pElement->parentNode().toElement().tagName() << " " << pElement->isElement();

        QString sID = this->getAttribute(pElement, "ID", "");

        if (sID.length() > 0)
        {

            std::map<std::string, std::function< std::string()> >::iterator oIt = m_pID2Value->find(sID.toStdString());

            if (!(oIt == m_pID2Value->end()))
            {

                std::string sNewValue = oIt->second();
                sNewValue = "Busted";

                std::cout << "replacing " << pElement->text().toStdString() << " with " << sNewValue << std::endl;

                pElement->setNodeValue( QString(sNewValue.c_str()) );


            }
        }

        QDomNodeList oChildren = pElement->childNodes();

        for (size_t i = 0; i < oChildren.length(); ++i)
        {

            QDomElement oChild = oChildren.item(i).toElement();

            replaceElements( &oChild );

        }
    }

    void saveTemplate(QString sFileName)
    {



        QDomDocument oSaveDocument = m_pDocument->toDocument();

        std::map<std::string, std::function< std::string()> >::iterator oIt = m_pID2Value->begin();



        for (size_t iTags = 0; iTags < m_pKnownTags->size(); ++iTags)
        {

            std::string sTag = m_pKnownTags->at(iTags);

            // Get the "Root" element
            QDomElement docElem = oSaveDocument.documentElement();

            // Find elements with tag name "firstchild"
            QDomNodeList nodes = docElem.elementsByTagName( QString(sTag.c_str()) );

            // Iterate through all we found
            for(int i=0; i<nodes.count(); i++)
            {
                QDomNode node = nodes.item(i);

                // Check the node is a DOM element
                if(node.nodeType() == QDomNode::ElementNode)
                {
                    // Access the DOM element
                    QDomElement element = node.toElement();

                    QString sElementID = this->getAttribute(&element, "ID", "");

                    oIt = m_pID2Value->find( sElementID.toStdString() );

                    if (oIt != m_pID2Value->end())
                    {


                        if ( element.tagName().compare("input", Qt::CaseInsensitive) == 0)
                        {

                            QDomText t = element.firstChild().toText();

                            if (!t.isNull())
                            { // inputs
                                QString sReplace = QString(oIt->second().c_str());

                                // Print out the original text
                                qDebug() << "Old text in " << sElementID << " of type " << element.tagName() << " was " << t.data() << " now is " << sReplace;
                                // Set the new text
                                t.setData( sReplace );
                            }


                        } else if ( ( element.tagName().compare("groupbox", Qt::CaseInsensitive) == 0) ||
                                    ( element.tagName().compare("combobox", Qt::CaseInsensitive) == 0) )
                        {

                            std::string sValue = oIt->second();
                            QString sqValue(sValue.c_str());

                            element.setAttribute("selected", sqValue );

                            qDebug() << "Old text in " << sElementID << " of type " << element.tagName() << " was " << " now is " << sqValue;


                        } else if ( element.tagName().compare("filedialog", Qt::CaseInsensitive) == 0)
                        {

                            std::string sValue = oIt->second();
                            QString sqValue(sValue.c_str());

                            element.setAttribute("location", sqValue );

                            qDebug() << "Old text in " << sElementID << " of type " << element.tagName() << " was " << " now is " << sqValue;


                        }

                    }


                    /*
                    // Iterate through it's children
                    for(QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling())
                    {
                        // Find the child that is of DOM type text
                        QDomText t = n.toText();
                        if (!t.isNull())
                        {
                            // Print out the original text
                            qDebug() << "Old text in " <<  << " of type " << element.tagName() << " was " << t.data();
                            // Set the new text
                            t.setData("Here is the new text");
                        }
                    }
                    */
                }
            }



        }

        qDebug() << oSaveDocument.toString();


        QFile file( sFileName );
        if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        {

            throw "Failed to open file for writing." ;
        }
        QTextStream stream( &file );
        stream << oSaveDocument.toString();
        file.close();

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
        QLayout* pLayout = this->createLayout(pElement);

        if (pLayout == NULL)
            return ELEMENT_TYPE::ELEMENT;

        delete pLayout;
        return ELEMENT_TYPE::LAYOUT;

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

                        this->addToLayout((QLayout*)pParent, pReturn);

                    }

                }
            }
        }

        return (QWidget*)pParent;

    }

    void addToLayout(QLayout* pLayout, QWidget* pWidget)
    {
        if (QExtGridLayout* pGridLayout = dynamic_cast<QExtGridLayout*>( pLayout ))
        {
            pGridLayout->addNextWidget(pWidget);
        } else {
            pLayout->addWidget( pWidget );
        }
    }


    bioGUIapp* m_pApp;

    std::map<std::string, std::function< std::string()> >* m_pID2Value;
    std::map<std::string, QWidget*>* m_pID2Widget;
    std::vector<QWidget*> m_vWidgets;
    std::vector<QPushButton*> m_vActions;

};


#endif //BIOGUI_PARSEXMLWINDOW_H
