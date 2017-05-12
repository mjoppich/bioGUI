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

#ifndef BIOGUI_WINDOWLAYOUTCREATOR_H
#define BIOGUI_WINDOWLAYOUTCREATOR_H


#include <string>
#include <functional>
#include <src/parsing/XMLParser.h>
#include <src/app/QOrderedLayout.h>
#include <sstream>
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
        WindowNode<QLayout>::CreatedElement oReturn;

        if (pLayoutNode != NULL)
        {
            oReturn = pLayoutNode->getWindowElement(pElement);
            delete pLayoutNode;
        }

        return oReturn;
    }

    WindowNode<QWidget>::CreatedElement createWidgetElement(QDomElement* pElement)
    {
        QString sTagName = pElement->tagName().toUpper();
        return this->createWidgetElement(sTagName, pElement);
    }

    WindowNode<QWidget>::CreatedElement createWidgetElement(QString& sID, QString& sWidgetDescription)
    {

        QDomDocument oSmallDoc;
        oSmallDoc.setContent(sWidgetDescription, false);

        QDomElement oWidgetElem = oSmallDoc.firstChild().toElement();
        // overwrite any id
        oWidgetElem.setAttribute("id", sID);

        return createWidgetElement( &oWidgetElem );
    }

    WindowNode<QWidget>::CreatedElement createWidgetElement(QString& sTag, QDomElement* pElement)
    {
        WindowWidgetNode* pWidgetNode = this->getCreatorForWidget(sTag);

        WindowNode<QWidget>::CreatedElement oReturn;

        if (pWidgetNode != NULL)
        {
            oReturn = pWidgetNode->getWindowElement(pElement);
            delete pWidgetNode;
        }
        return oReturn;
    }

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

    void printAvailableNodes()
    {


        std::map<std::string, std::function< WindowLayoutNode* () > >::iterator oIt = m_mLayoutNodeMap.begin();
        while (oIt != m_mLayoutNodeMap.end())
        {
            WindowLayoutNode* pNode = oIt->second();
            std::string sTag = oIt->first;

            this->handleAttributeNode(sTag, pNode);

            delete pNode;
            ++oIt;
        }
        std::map<std::string, std::function< WindowWidgetNode*()> >::iterator oJt = m_mWidgetNodeMap.begin();
        while (oJt != m_mWidgetNodeMap.end())
        {
            WindowWidgetNode* pNode = oJt->second();
            std::string sTag = oJt->first;

            this->handleAttributeNode(sTag, pNode);

            delete pNode;
            ++oJt;
        }

    }

protected:

    void handleAttributeNode(std::string& sTag, WindowBaseNode* pNode)
    {
        if (pNode == NULL)
            return;

        std::vector<std::string> vAttributes = pNode->getAcceptedAttributes();

        std::stringstream oSS;
        for (size_t j = 0; j < vAttributes.size(); ++j)
            oSS << ", " << vAttributes[j];

        std::string sJoined = oSS.str();

        if (sJoined.size() > 2)
            sJoined = sJoined.substr(2, -1);

        std::cout << "<" << sTag << ">" << "\t[" << sJoined << "]" << std::endl;


    }

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
