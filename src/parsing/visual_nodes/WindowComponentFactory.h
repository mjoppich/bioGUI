//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWLAYOUTCREATOR_H
#define BIOGUI_WINDOWLAYOUTCREATOR_H


#include <string>
#include <functional>
#include <src/parsing/XMLParser.h>
#include <src/app/QOrderedLayout.h>
#include "WindowLayoutNode.h"
#include "WindowWidgetNode.h"
#include "WindowWidgetLabelNode.h"


class bioGUIapp;

class WindowComponentFactory {

public:

    WindowComponentFactory(bioGUIapp* pApp);


    WindowLayoutNode* getCreatorForLayout(QString& sTag);
    WindowWidgetNode* getCreatorForWidget(QString& sTag);

    WindowNode<QLayout>::CreatedElement createLayoutElement(QDomElement* pElement)
    {
        QString sTagName = pElement->tagName().toUpper();
        return this->createLayoutElement(sTagName, pElement);
    }

    WindowNode<QLayout>::CreatedElement createLayoutElement(QString& sTag, QDomElement* pElement)
    {

        WindowLayoutNode* pLayoutNode = this->getCreatorForLayout(sTag);
        WindowNode<QLayout>::CreatedElement oReturn = pLayoutNode->getWindowElement(pElement);

        delete pLayoutNode;
        return oReturn;
    }

    WindowNode<QWidget>::CreatedElement createWidgetElement(QDomElement* pElement)
    {
        QString sTagName = pElement->tagName().toUpper();
        return this->createWidgetElement(sTagName, pElement);
    }

    WindowNode<QWidget>::CreatedElement createWidgetElement(QString& sTag, QDomElement* pElement)
    {
        WindowWidgetNode* pLayoutNode = this->getCreatorForWidget(sTag);
        WindowNode<QWidget>::CreatedElement oReturn = pLayoutNode->getWindowElement(pElement);

        delete pLayoutNode;
        return oReturn;    }

    std::vector<std::string>* getKnownTags()
    {
        std::vector<std::string>* pTags = new std::vector<std::string>();

        std::map<std::string, std::function< WindowLayoutNode* () > >::iterator oIt = m_mLayoutNodeMap.begin();
        while (oIt != m_mLayoutNodeMap.end())
        {
            pTags->push_back( oIt->first );
            ++oIt;
        }
        std::map<std::string, std::function< WindowWidgetNode*()> >::iterator oJt = m_mWidgetNodeMap.begin();
        while (oJt != m_mWidgetNodeMap.end())
        {
            pTags->push_back( oJt->first );
            ++oJt;
        }

        // manually add master node!
        pTags->push_back("WINDOW");

        return pTags;
    }

    bioGUIapp* getApp()
    {
        return m_pApp;
    }

    bool isLayout(QDomElement* pElement)
    {
        return isLayout(pElement->tagName());
    }

    bool isWidget(QDomElement* pElement)
    {
        return isWidget(pElement->tagName());
    }

    bool isLayout(QString sTag)
    {
        std::string sStdTag = sTag.toUpper().toStdString();

        std::map<std::string, std::function<WindowLayoutNode*()>>::iterator oFind = m_mLayoutNodeMap.find(sStdTag);

        if (!(oFind != m_mLayoutNodeMap.end()))
        {
            return false;
        }

        return true;

    }

    bool isWidget(QString sTag)
    {
        std::string sStdTag = sTag.toUpper().toStdString();

        std::map<std::string, std::function<WindowWidgetNode*()>>::iterator oFind = m_mWidgetNodeMap.find(sStdTag);

        if (!(oFind != m_mWidgetNodeMap.end()))
        {
            return false;
        }

        return true;

    }

    void addToLayout(QLayout* pLayout, QWidget* pWidget)
    {
        if (QOrderedLayout* pOrderedLayout = dynamic_cast<QOrderedLayout*>( pLayout ))
        {
            pOrderedLayout->addNextWidget(pWidget);
        } else {

            pLayout->addWidget( pWidget );

        }
    }

protected:

    void initializeWidgets();

    void initializeLayouts();

    void insertWidgetNode(std::string sType, std::function< WindowWidgetNode* ( ) > oFunc)
    {
        std::pair<std::string, std::function< WindowWidgetNode*( )> > oPair( sType, oFunc );

        m_mWidgetNodeMap.insert(oPair);

    }

    void insertLayoutNode(std::string sType, std::function< WindowLayoutNode* ( ) > oFunc)
    {
        std::pair<std::string, std::function< WindowLayoutNode*( )> > oPair( sType, oFunc );

        m_mLayoutNodeMap.insert(oPair);

    }

    std::map< std::string, std::function< WindowLayoutNode* ( ) > > m_mLayoutNodeMap;
    std::map< std::string, std::function< WindowWidgetNode* ( ) > > m_mWidgetNodeMap;

    bioGUIapp* m_pApp;

};




#endif //BIOGUI_WINDOWLAYOUTCREATOR_H
