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

#ifndef BIOGUI_WINDOWWIDGETSTREAMBOXNODE_H
#define BIOGUI_WINDOWWIDGETSTREAMBOXNODE_H

#include <QtWidgets/QGroupBox>
#include <src/app/AdvancedStreamBox.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include "WindowWidgetNode.h"
#include "WindowWidgetStreamNode.h"

class WindowWidgetStreamBoxNode : public WindowWidgetNode {

public:

    WindowWidgetStreamBoxNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetStreamBoxNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;
        oReturn.bHasChildrenFinished = true;

        QGroupBox* pGroupBox = new QGroupBox();

        AdvancedStreamBox* pStreamOut = new AdvancedStreamBox();
        // this could also be a hboxlayout or a grid layout
        QLayout* pLayout = new QVBoxLayout();

        pLayout->addWidget( pStreamOut );

        QPushButton* pClearButton = new QPushButton("Clear");

        QObject::connect(pClearButton, &QAbstractButton::clicked, [pStreamOut] {

            pStreamOut->clear();

        });

        pLayout->addWidget(pClearButton);

        QPushButton* pSaveLogButton = new QPushButton("Save Log");

        QObject::connect(pSaveLogButton, &QAbstractButton::clicked, [pStreamOut] {

            QString sFileName = QFileDialog::getSaveFileName(0, "Select Output File", QDir::currentPath(), "");

            pStreamOut->saveToFile(sFileName);

        });

        pLayout->addWidget(pSaveLogButton);


        QDomNodeList oChildren = pDOMElement->childNodes();

        WindowWidgetStreamNode oStreamNode(m_pFactory);

        for (size_t i = 0; i < oChildren.size(); ++i)
        {
            QDomElement oChildNode = oChildren.at(i).toElement();
            QWidget* pChildElement = oStreamNode.getWindowElement(&oChildNode).pElement;

            if (pChildElement == NULL)
            {
                LOGERROR("error in creating streambox components");
                continue;
            }


            if (QCheckBox* pButton = dynamic_cast<QCheckBox*>(pChildElement))
            {
                pButton->setChecked(true);

                std::string sID = this->getAttribute(&oChildNode, "id", "");
                if (sID.length() > 0)
                {

                    m_pFactory->getApp()->getWindowParser()->addID2Widget( sID, pStreamOut, true );
                    pStreamOut->addStream( sID, pButton );
                    pLayout->addWidget( pChildElement );

                } else {
                    delete pChildElement;
                }

            }

        }

        pGroupBox->setLayout(pLayout);

        oReturn.pElement = pGroupBox;

        bioGUIapp* pApp = m_pFactory->getApp();
        pLayout->setSizeConstraint(QLayout::SetFixedSize);

        QObject::connect(pStreamOut, &AdvancedStreamBox::sizeChanged, [pApp, pLayout] () {


            pLayout->activate();
            pLayout->update();

            //pApp->getMainWindow()->update();
            pApp->reloadAppWindow();

        });

        // must be done here because otherwise the groupbox is the id widget ...
        std::string sID = this->getAttribute(pDOMElement, "id", "");
        if (sID.length() > 0)
        {
            m_pFactory->getApp()->getWindowParser()->addID2Widget( sID, pStreamOut, true );

        }

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

    }


};

#endif //BIOGUI_WINDOWWIDGETSTREAMBOXNODE_H
