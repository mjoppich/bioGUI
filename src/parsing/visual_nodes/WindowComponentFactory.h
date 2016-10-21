//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWLAYOUTCREATOR_H
#define BIOGUI_WINDOWLAYOUTCREATOR_H


#include <string>
#include <src/bioGUIapp.h>
#include "WindowLayoutNode.h"
#include "WindowWidgetNode.h"
#include "WindowWidgetLabelNode.h"

class WindowComponentFactory {

public:

    WindowComponentFactory(bioGUIapp* pApp)
    : m_pApp(pApp)
    {

        this->initializeLayouts();
        this->initializeWidgets();

    }


    WindowLayoutNode getCreatorForLayout(QString& sTag)
    {

        std::string sUpperTag = sTag.toUpper().toStdString();
        std::map<std::string, std::function<WindowLayoutNode()>>::iterator oFind = m_mLayoutNodeMap.find(sUpperTag);
        if (!(oFind != m_mLayoutNodeMap.end()))
            throw XMLParserException("Invalid Layout Tag: " + sUpperTag);

        return (oFind->second)();

    }


    WindowWidgetNode getCreatorForWidget(QString& sTag)
    {

        std::string sUpperTag = sTag.toUpper().toStdString();
        std::map<std::string, std::function<WindowWidgetNode()>>::iterator oFind = m_mWidgetNodeMap.find(sUpperTag);
        if (!(oFind != m_mWidgetNodeMap.end()))
            throw XMLParserException("Invalid Widget Tag: " + sUpperTag);

        return (oFind->second)();

    }

    WindowNode<QLayout>::CreatedElement createLayoutElement(QDomElement* pElement)
    {
        QString sTagName = pElement->tagName();
        return this->createLayoutElement(sTagName, pElement);
    }

    WindowNode<QLayout>::CreatedElement createLayoutElement(QString& sTag, QDomElement* pElement)
    {
        return this->getCreatorForLayout(sTag).getWindowElement(pElement);
    }

    WindowNode<QWidget>::CreatedElement createWidgetElement(QString& sTag, QDomElement* pElement)
    {
        return this->getCreatorForWidget(sTag).getWindowElement(pElement);
    }

protected:

    void initializeWidgets()
    {
        this->insertWidgetNode("label", [] (QDomElement* pElement) {
            return WindowWidgetLabelNode();
        });
    }

    void initializeLayouts()
    {

        /**
         * ADD LAYOUTS
         */
        this->insertLayoutNode("hgroup", [] () {
            return WindowLayoutHorizontalNode();
        });

        this->insertLayoutNode("vgroup", [] () {
            return WindowLayoutVerticalNode();
        });

        this->insertLayoutNode("grid", [] () {
            return WindowLayoutGridNode();
        });

    }

    void insertWidgetNode(std::string sType, std::function< WindowWidgetNode ( ) > oFunc)
    {
        std::pair<std::string, std::function< WindowWidgetNode( )> > oPair( sType, oFunc );

        m_mWidgetNodeMap.insert(oPair);

    }

    void insertLayoutNode(std::string sType, std::function< WindowLayoutNode ( ) > oFunc)
    {
        std::pair<std::string, std::function< WindowLayoutNode( )> > oPair( sType, oFunc );

        m_mWidgetNodeMap.insert(oPair);

    }

    std::map< std::string, std::function< WindowLayoutNode ( ) > > m_mLayoutNodeMap;
    std::map< std::string, std::function< WindowWidgetNode ( ) > > m_mWidgetNodeMap;

    std::map< std::string, std::function< WindowNode<QLayout>::CreatedElement( QDomElement*) > > m_mCreateLayoutMap;
    std::map< std::string, std::function< WindowNode<QWidget>::CreatedElement( QDomElement*)> > m_mCreateWidgetMap;


    bioGUIapp* m_pApp;

};


#endif //BIOGUI_WINDOWLAYOUTCREATOR_H
