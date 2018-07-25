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
#include <src/app/QAbstractButtonItem.h>
#include <src/app/QExclusiveGroupBox.h>
#include <src/parsing/visual_nodes/WindowComponentFactory.h>
#include <src/parsing/visual_nodes/WidgetFunctionNode.h>
#include "XMLParser.h"

class bioGUIapp;

class XMLParserWindow : public XMLParser {

Q_OBJECT

public:

    XMLParserWindow(bioGUIapp* pApp)
    : XMLParser()
    {

        m_pApp = pApp;

        m_pID2Value = new std::map<std::string, std::function< std::string()> >();
        m_pInputID2FunctionWidget = new std::map<std::string, WidgetFunctionNode* >();


        /**
         * ADD WIDGETS
         */
        m_pWidgetNodeFactory = new WindowComponentFactory(pApp);

        m_pKnownTags = m_pWidgetNodeFactory->getKnownTags();
    }

    virtual void printAvailableNodes()
    {

        m_pWidgetNodeFactory->printAvailableNodes();

    }

    void addID2WidgetFunction(std::string& sID, WidgetFunctionNode* pWidgetNode, bool bOverwrite=false)
    {

        if (sID.length() > 0)
        {

            if (bOverwrite)
            {

                std::map<std::string, WidgetFunctionNode*>::iterator oIt = m_pInputID2FunctionWidget->find(sID);

                if (oIt != m_pInputID2FunctionWidget->end())
                {
                    // overwrite
                    oIt->second = pWidgetNode;
                    return;
                }

            }

            // insert new
            m_pInputID2FunctionWidget->insert( std::pair<std::string, WidgetFunctionNode*>(sID, pWidgetNode));

        }

    }


    void loadFile(std::string& sFileName)
    {
        if (m_pDocument != NULL)
            delete m_pDocument;

        m_pDocument = loadFromFile(sFileName);
    }

    void setActionsEnabled(bool bEnabled)
    {

        for (size_t i = 0; i < m_vActions.size(); ++i)
        {
            m_vActions.at(i)->setEnabled( bEnabled );
        }

    }

    void addAction(QWidget* pActionWidget)
    {
        m_vActions.push_back( pActionWidget );
    }

    QWidget* getWindow()
    {

        QDomElement* pWindowRoot = this->getRoot( m_pDocument );

        if (pWindowRoot == NULL)
            return NULL;

        m_vWidgets.clear();
        m_vActions.clear();
        m_pInputID2FunctionWidget->clear();
        m_pID2Value->clear();

        QWidget* pWindow = (QWidget*) createComponents(NULL, pWindowRoot);

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


    void saveAllDocumentElems(QDomElement* pElement, QList<QDomElement>* pList, QDomDocument* pDoc)
    {

        QDomNodeList oChildren = pElement->childNodes();

        QString sTag = pElement->tagName().toUpper();
        WindowWidgetNode* pNodeCreator = this->m_pWidgetNodeFactory->getCreatorForWidget( sTag );

        if (pNodeCreator != NULL)
        {

            pNodeCreator->saveInQDomElement(pElement, m_pID2Value, pDoc);

        }


        for (size_t i = 0; i < oChildren.length(); ++i)
        {
            QDomElement oChild = oChildren.item(i).toElement();

            this->saveAllDocumentElems( &oChild, pList, pDoc);

        }


    }


    void saveTemplate(QString sFileName)
    {



        QDomDocument oSaveDocument = m_pDocument->toDocument();

        std::map<std::string, std::function< std::string() > >::iterator oIt = m_pID2Value->begin();

        QDomElement oRoot = oSaveDocument.documentElement();

        QList<QDomElement>* pList = new QList<QDomElement>();

        this->saveAllDocumentElems(&oRoot, pList, &oSaveDocument);

        /*

        for (size_t iTags = 0; iTags < m_pKnownTags->size(); ++iTags)
        {

            std::string sTag = m_pKnownTags->at(iTags);

            // Find elements with tag name "firstchild"
            QDomNodeList nodes = oSaveDocument.elementsByTagName( QString(sTag.c_str()) );

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

                        // set qelement as needed
                        WindowWidgetNode* pNodeCreator = this->m_pWidgetNodeFactory->getCreatorForWidget( element.tagName() );
                        pNodeCreator->saveInQDomElement(&element, m_pID2Value);


                    } else {

                        // no id, no change needed :)

                    }

                }
            }



        }

        */


        QFile file( sFileName );
        if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        {

            LOGERROR("Failed to open file for writing: " + sFileName.toStdString());
        }
        QTextStream stream( &file );
        stream << oSaveDocument.toString();
        file.close();

    }

    void printRetrieverIDs()
    {
        std::map<std::string, std::function<std::string()>>::iterator oIt = m_pID2Value->begin();

        while (oIt != m_pID2Value->end())
        {
            std::cout << oIt->first << " has RETRIEVER" <<std::endl;

            ++oIt;
        }
    }

    std::map<std::string, WidgetFunctionNode* >* getID2Widget()
    {
        return m_pInputID2FunctionWidget;
    }

    std::map<std::string, std::function< std::string()> >* getID2Value()
    {
        return m_pID2Value;
    }

    ~XMLParserWindow()
    {
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



    bool addValueFetcher(std::string& sElemID, std::function<std::string()> oFunc)
    {

        if (sElemID.length() == 0)
        {
            // std::cerr << "expected element id for but none was given";

            return false;
        }

        m_pID2Value->insert( std::pair< std::string, std::function<std::string()> >( sElemID, oFunc ) );

        return true;

    }



    QLayout* createLayout(QDomElement* pElement);
    QWidget* createComponent(QDomElement* pElement, bool* pChildrenFinished);

    QWidget* createComponents(QWidget* pParentWidget, QDomElement* pElement)
    {

        if (pElement->isElement() == false)
        {
            qDebug() << pElement->tagName() << " child of " << pElement->parentNode().toElement().tagName() << " " << pElement->isElement();
            return NULL;
        }

        bool bIsWidget = m_pWidgetNodeFactory->isWidget( pElement );

        if ( bIsWidget )
        {
            qDebug() << pElement->tagName() << " is an element";
        } else {
            qDebug() << pElement->tagName() << " is not an element";
        }

        void* pParent = NULL;
        bool bChildrenFinished = false;

        if ( bIsWidget )
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

                // get next child of parent
                QDomElement oChild = oChildren.item(i).toElement();

                if (!oChild.isElement())
                {
                    continue;
                }

                if (bIsWidget)
                {
                    QWidget* pReturn = createComponents((QWidget*)pParent, &oChild);

                    if (m_pWidgetNodeFactory->isWidget(&oChild))
                    {
                        // create a layout for current parent and add pReturn to this layout
                        QWidget* pWParent = (QWidget*) pParent;

                        QLayout* pLParent = pWParent->layout();

                        if (pLParent == NULL)
                        {
                            pLParent = new QVBoxLayout();
                            pWParent->setLayout(pLParent);
                        }

                        pLParent->addWidget(pReturn);

                    }

                } else {

                    std::cerr << oChild.tagName().toStdString() << std::endl;


                    bool bChildIsElement = m_pWidgetNodeFactory->isWidget(&oChild);
                    QWidget* pReturn = createComponents( NULL, &oChild);


                    if (bChildIsElement == false)
                    {
                        ((QBoxLayout *) pParent)->addLayout( (QLayout*) pReturn);

                    } else {

                        // indeed this is a sort of "factor it"
                        m_pWidgetNodeFactory->addToLayout((QLayout*)pParent, pReturn);

                    }

                }
            }
        }

        return (QWidget*)pParent;

    }





    /*

    QWidget* createGroup(QDomElement* pElement, bool* pChildrenFinished)
    {

        std::function<QLayout* (QDomElement*, QDomNodeList*)> oLayoutFunc = [this] (QDomElement* pDElement, QDomNodeList* pChildren) {

            QLayout* pReturn = NULL;

            if (pChildren->size() == 1)
            {
                QDomElement oChildNode = pChildren->at(0).toElement();

                if (this->getElementType(&oChildNode) == ELEMENT_TYPE::LAYOUT)
                {

                    pReturn = this->createLayout(&oChildNode);

                    // TODO does this do the trick?
                    *pChildren = QDomNodeList(pChildren->at(0).childNodes());

                    return pReturn;
                }
            }


            return (QLayout*) new QVBoxLayout();
        };

        QWidget* pReturn = this->createGeneralGroup(pElement, pChildrenFinished, oLayoutFunc);

        return pReturn;

    }

    QWidget* createGroupBox(QDomElement* pElement, bool* pChildrenFinished)
    {

        std::function<QLayout* (QDomElement*, QDomNodeList*)> oLayoutFunc = [this] (QDomElement* pDElement, QDomNodeList* pChildren) {

            QLayout* pReturn = NULL;

            if (pChildren->size() == 1)
            {
                QDomElement oChildNode = pChildren->at(0).toElement();
                LAYOUT_TYPE eLayoutType = this->getLayoutType(&oChildNode);

                std::cout << eLayoutType << std::endl;

                if (LAYOUT_TYPE::NONE != eLayoutType)
                {


                    *pChildren = QDomNodeList(pChildren->at(0).childNodes());

                    int iRows = 0;
                    int iCols = 0;

                    QExtGridLayout* pTestLayout = NULL;
                    if (eLayoutType == LAYOUT_TYPE::GRID)
                        pTestLayout = (QExtGridLayout*) this->createLayout(&oChildNode);


                    switch ( eLayoutType )
                    {
                        case LAYOUT_TYPE::HORIZONTAL:

                            iRows = 1;
                            iCols = pChildren->size() * 2;

                            break;

                        case LAYOUT_TYPE::VERTICAL:

                            iRows = pChildren->size() * 2;
                            iCols = 2;

                            break;

                        case LAYOUT_TYPE::GRID:


                            iRows = pTestLayout->getRows();
                            iCols = pTestLayout->getCols() * 2;

                            delete pTestLayout;

                            break;

                        default:

                        std::cout << "default to vertical layout" << std::endl;

                            eLayoutType = LAYOUT_TYPE::VERTICAL;
                            iRows = pChildren->size() * 2;
                            iCols = 2;

                    }

                    pReturn = new QExtGridLayout(iRows, iCols);

                    return pReturn;
                }
            }


            // defaults a vertical layout
            int iRows = pChildren->size();
            return (QLayout*) new QExtGridLayout(iRows, 2);
        };

        QButtonGroup* pButtonGroup = new QButtonGroup();

        std::function<void (QExclusiveGroupBox*, QLayout*, QWidget*, QWidget*, QStringList*, int)> oPostProc = [pButtonGroup] (QExclusiveGroupBox* pBox,
                                                                                                                               QLayout* pLayout,
                                                                                                          QWidget* pChildWidget,
                                                                                                          QWidget* pTransWidget,
                                                                                                          QStringList* pSelected,
                                                                                                          int iElement) {

            if (QAbstractButtonItem* pButtonItem = dynamic_cast<QAbstractButtonItem *>(pChildWidget))
            {
                if ((iElement == 0) || ( pSelected->contains(pButtonItem->getValue()) ))
                {

                    QAbstractButton* pButton = dynamic_cast<QAbstractButton*>(pChildWidget);

                    pButton->setChecked(true);

                }

                pButtonGroup->addButton((QAbstractButton*) pChildWidget, iElement);
            }

        };


        std::function<QList<QWidget*> (QDomElement*, bool*, int)> oPreProc = [this,pButtonGroup] (QDomElement* pChildNode, bool* pBoolean, int iElement) {


            QDomText oSimpleTextNode = pChildNode->childNodes().at(0).toText();

            // default: only a simple radio/check
            if (!oSimpleTextNode.isNull())
            {
                // TODO this must create a qlabel based widget for the simple case!
                QAbstractButton* pBox = (QAbstractButton*) createComponent(pChildNode, pBoolean);
                QString sText = pBox->text();
                pBox->setText("");

                pButtonGroup->addButton(pBox, iElement);

                QList<QWidget*> vWidgets;
                vWidgets.append(pBox);
                vWidgets.append(new QLabel(sText));

                return vWidgets;
            }

            QAbstractButton* pBox = (QAbstractButton*) createComponent(pChildNode, pBoolean);
            pBox->setText("");
            pButtonGroup->addButton(pBox, iElement);

            QWidget* pContentTransporter = new QWidget();
            QHBoxLayout* pContentTransporterLayout = new QHBoxLayout();

            QDomNodeList oChildren = pChildNode->childNodes();

            for (size_t i = 0; i < oChildren.size(); ++i)
            {

                QDomElement oChildElement = pChildNode->childNodes().at(i).toElement();
                QWidget* pContent = createComponent(&oChildElement, pBoolean);

                pContentTransporterLayout->addWidget(pContent);

            }

            pContentTransporter->setLayout(pContentTransporterLayout);

            QList<QWidget*> vWidgets;
            vWidgets.append(pBox);
            vWidgets.append(pContentTransporter);

            return vWidgets;

        };


        QExclusiveGroupBox* pGroupBox = (QExclusiveGroupBox*) this->createGeneralGroup(pElement, pChildrenFinished, oLayoutFunc, oPreProc, oPostProc);

        this->addValueFetcher(pElement, [pButtonGroup, pGroupBox] () {


            bool bEvaluate = true;
            if (pGroupBox->isCheckable())
            {
                bEvaluate = pGroupBox->isChecked();
            }

            if (bEvaluate)
            {

                QAbstractButton* pButton = pButtonGroup->checkedButton();
                QAbstractButtonItem* pButtonItem = dynamic_cast<QAbstractButtonItem *>(pButton);

                if (pButtonItem)
                {
                    return pButtonItem->getValue().toStdString();
                }

            }

            return std::string("");

        });


        return pGroupBox;

    }

    QWidget* createGeneralGroup(QDomElement* pElement,
                                bool* pChildrenFinished,
                                std::function<QLayout* (QDomElement*, QDomNodeList*)> oLayoutFunc,
                                std::function<QList<QWidget*> (QDomElement*, bool*, int)> oPreProcFunc = [] (QDomElement* pChildNode, bool* pBoolean, int iElement) {return QList<QWidget*>();},
                                std::function<void (QExclusiveGroupBox*, QLayout*, QWidget*, QWidget*, QStringList*, int)> oPostProcFunc = [] (QExclusiveGroupBox* pBox, QLayout* pLay, QWidget* pWid1, QWidget* pWid2, QStringList* pList, int iElement) {})
    {
        (*pChildrenFinished) = true;

        QExclusiveGroupBox* pGroupBox = new QExclusiveGroupBox("");

        /*
         * Window Title

        QString sTitle = this->getAttribute(pElement, "title", "");
        pGroupBox->setTitle( sTitle );

        /*
         * checkable?

        std::string sCheckable = this->getAttribute(pElement, "checkable", "false").toUpper().toStdString();
        if (sCheckable.compare("TRUE") == 0)
        {
            pGroupBox->setCheckable(true);
            pGroupBox->setChecked(false);

            std::string sCheckedValue = this->getAttribute(pElement, "checked_value", "true").toStdString();
            std::string sUncheckedValue = this->getAttribute(pElement, "unchecked_value", "false").toStdString();

            this->addValueFetcher(pElement, [pGroupBox, sCheckedValue, sUncheckedValue] () {

                if (pGroupBox->isChecked())
                {
                    return sCheckedValue;
                } else {
                    return sUncheckedValue;
                }

            });

        }

        /*
         * exclusive?

        QString sExclusive = this->getAttribute(pElement, "exclusive", "false");
        if (sExclusive.compare("TRUE", Qt::CaseInsensitive) == 0)
        {
            pGroupBox->setExclusive(true);
        } else {
            pGroupBox->setExclusive(false);
        }

        /*
         * ordered
         *


        bool bOrdered = (this->getAttribute(pElement, "ordered", "false").compare("true", Qt::CaseInsensitive) == 0);
        pGroupBox->setOrdered(bOrdered);

        /*
         * state?
         *
         * selected = true/false may indicate that group is selected.
         *
         * selected = [ids] may indicate that group is selected and children are selected
         *

        QString sSelected = this->getAttribute(pElement, "selected", "");
        QStringList vSelected;

        if (pGroupBox->isCheckable())
        {
            if (sSelected.compare("TRUE", Qt::CaseInsensitive) == 0)
            {
                pGroupBox->setChecked(true);
            } else if (sSelected.compare("FALSE", Qt::CaseInsensitive) == 0)
            {
                pGroupBox->setChecked(false);
            }

        } else {

            pGroupBox->setChecked(true);

            if (sSelected.length() > 0)
            {
                vSelected = sSelected.split(";");
            }

        }

         //* LAYOUT


        QDomNodeList oChildren = pElement->childNodes();
        QLayout* pLayout = oLayoutFunc(pElement, &oChildren);

                 // CREATE AND ADD CHILDREN

        bool bBoolean = false;
        int iAdded = 0;

        for (size_t i = 0; i < oChildren.size(); ++i)
        {
            bBoolean = false;

            QDomElement oChildNode = oChildren.at(i).toElement();
            QList<QWidget*> vChildren = oPreProcFunc(&oChildNode, &bBoolean, iAdded);

            if (vChildren.size() == 0)
            {
                QWidget* pChildElement = this->createComponent(&oChildNode, &bBoolean);

                if (pChildElement != NULL)
                    vChildren.append(pChildElement);
            }


            if (vChildren.size() == 0)
            {
                std::cout << "error in creating groupbox components: " + pElement->text().toStdString() << std::endl;
                std::cout << "error in creating groupbox components: " + oChildNode.text().toStdString() << std::endl;
                //throw "error in creating groupbox components: " + oChildNode.text().toStdString();

                continue;
            }

            for (size_t i = 0; i < vChildren.size(); ++i)
            {

                QWidget* pChildElement = vChildren.at(i);

                // TODO this is not going to work with ordered radiobuttons ...
                QWidget* pTransformedChildElement = pGroupBox->addNextWidget(pChildElement);
                this->addToLayout(pLayout, pTransformedChildElement);
                this->setID(pTransformedChildElement, &oChildNode, true);

                oPostProcFunc(pGroupBox, pLayout, pChildElement, pTransformedChildElement, &vSelected, iAdded);
            }


            ++iAdded;

        }

        // Finally set Layout
        QWidget* pComponentWidget = new QWidget();

        pComponentWidget->setLayout(pLayout);

        QLayout* pComponentLayout = new QHBoxLayout();
        pComponentLayout->addWidget(pComponentWidget);

        pGroupBox->setLayout(pComponentLayout);

         // * AFTER SETTING LAYOUT CARE FOR VISIBILITY

        if (pGroupBox->isCheckable() == true)
        {


            std::string sVisible = this->getAttribute(pElement, "visible", "ALWAYS").toUpper().toStdString();

            if (sVisible.compare("ALWAYS") != 0)
            {
                QObject::connect( pGroupBox, &QExclusiveGroupBox::toggled, [pGroupBox, pComponentWidget] () {

                    if (pGroupBox->isCheckable())
                    {

                        if (pGroupBox->isChecked())
                        {
                            pComponentWidget->setVisible(true);
                        } else {
                            pComponentWidget->setVisible(false);
                        }

                    }

                } );

                if (sVisible.compare("FALSE") == 0)
                {
                    pGroupBox->setChecked(false);
                } else if (sVisible.compare("TRUE"))
                {
                    pGroupBox->setChecked(true);
                }

            }

        }

        pGroupBox->getConsistent();


        return pGroupBox;
    }

*/

    bioGUIapp* m_pApp;

    std::map<std::string, std::function< std::string() > >* m_pID2Value = NULL;
    std::map<std::string, WidgetFunctionNode*>* m_pInputID2FunctionWidget = NULL;
    std::vector<QWidget*> m_vWidgets;
    std::vector<QWidget*> m_vActions;

    WindowComponentFactory* m_pWidgetNodeFactory = NULL;

};


#endif //BIOGUI_PARSEXMLWINDOW_H

