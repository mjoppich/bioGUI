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

#include "WindowComponentFactory.h"
#include "WindowWidgetLabelNode.h"
#include "WindowLayoutHorizontalNode.h"
#include "WindowLayoutVerticalNode.h"
#include "WindowLayoutGridNode.h"
#include "WindowWidgetActionNode.h"
#include "WindowWidgetCheckBoxNode.h"
#include "WindowWidgetComboboxNode.h"
#include "WindowWidgetFileListNode.h"
#include "WindowWidgetFileSelectBoxNode.h"
#include "WindowWidgetFileDialogNode.h"
#include "WindowWidgetGroupNode.h"
#include "WindowWidgetGroupBoxNode.h"
#include "WindowWidgetImageNode.h"
#include "WindowWidgetInputNode.h"
#include "WindowWidgetRadioButtonNode.h"
#include "WindowWidgetSlideritemNode.h"
#include "WindowWidgetSliderNode.h"
#include "WindowWidgetStreamBoxNode.h"
#include "WindowWidgetWindowNode.h"

#include <src/parsing/XMLParser.h>
#include <src/bioGUIapp.h>


WindowComponentFactory::WindowComponentFactory(bioGUIapp *pApp)
        : m_pApp(pApp)
{

    this->initializeLayouts();
    this->initializeWidgets();

}

void WindowComponentFactory::initializeLayouts() {

    /**
     * ADD LAYOUTS
     */
    this->insertLayoutNode("HGROUP", [this] () {
        return new WindowLayoutHorizontalNode(this);
    });

    this->insertLayoutNode("VGROUP", [this] () {
        return new WindowLayoutVerticalNode(this);
    });

    this->insertLayoutNode("GRID", [this] () {
        return new WindowLayoutGridNode(this);
    });

}


void WindowComponentFactory::initializeWidgets() {
    this->insertWidgetNode("ACTION", [this] () {
        return new WindowWidgetActionNode(this);
    });

    this->insertWidgetNode("CHECKBOX", [this] () {
        return new WindowWidgetCheckBoxNode(this);
    });

    this->insertWidgetNode("COMBOBOX", [this] () {
        return new WindowWidgetComboboxNode(this);
    });

    this->insertWidgetNode("COMBOITEM", [this] () {
        return new WindowWidgetComboitemNode(this);
    });

    this->insertWidgetNode("FILEDIALOG", [this] () {
        return new WindowWidgetFileDialogNode(this);
    });

    this->insertWidgetNode("FILELIST", [this] () {
        return new WindowWidgetFileListNode(this);
    });

    this->insertWidgetNode("FILESELECTBOX", [this] () {
        return new WindowWidgetFileSelectBoxNode(this);
    });


    this->insertWidgetNode("GROUPBOX", [this] () {
        return new WindowWidgetGroupBoxNode(this);
    });


    this->insertWidgetNode("GROUP", [this] () {
        return new WindowWidgetGroupNode(this);
    });

    this->insertWidgetNode("IMAGE", [this] () {
        return new WindowWidgetImageNode(this);
    });

    this->insertWidgetNode("INPUT", [this] () {
        return new WindowWidgetInputNode(this);
    });

    this->insertWidgetNode("LABEL", [this] () {
        return new WindowWidgetLabelNode(this);
    });



    this->insertWidgetNode("RADIOBUTTON", [this] () {
        return new WindowWidgetRadioButtonNode(this);
    });

    this->insertWidgetNode("SLIDERITEM", [this] () {
        return new WindowWidgetSlideritemNode(this);
    });

    this->insertWidgetNode("SLIDER", [this] () {
        return new WindowWidgetSliderNode(this);
    });

    this->insertWidgetNode("STREAMBOX", [this] () {
        return new WindowWidgetStreamBoxNode(this);
    });

    this->insertWidgetNode("STREAM", [this] () {
        return new WindowWidgetStreamNode(this);
    });

    this->insertWidgetNode("WINDOW", [this] () {
        return new WindowWidgetWindowNode(this);
    });
}


WindowLayoutNode* WindowComponentFactory::getCreatorForLayout(const QString &sTag) {

    std::string sUpperTag = sTag.toUpper().toStdString();
    std::map<std::string, std::function<WindowLayoutNode*()>>::iterator oFind = m_mLayoutNodeMap.find(sUpperTag);
    if (!(oFind != m_mLayoutNodeMap.end()))
    {
        LOGERROR("Invalid Layout Tag: " + sUpperTag);
        return NULL;
    }

    return (oFind->second)();

}

WindowWidgetNode* WindowComponentFactory::getCreatorForWidget(const QString &sTag) {

    std::string sUpperTag = sTag.toUpper().toStdString();
    std::map<std::string, std::function<WindowWidgetNode*()>>::iterator oFind = m_mWidgetNodeMap.find(sUpperTag);
    if (!(oFind != m_mWidgetNodeMap.end()))
    {
        LOGERROR("Invalid Widget Tag: " + sUpperTag);
        return NULL;
    }

    return (oFind->second)();

}
