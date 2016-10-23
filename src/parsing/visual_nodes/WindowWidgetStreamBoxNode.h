//
// Created by joppich on 10/21/16.
//

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
                throw "error in creating streambox components";

            if (QCheckBox* pButton = dynamic_cast<QCheckBox*>(pChildElement))
            {
                pButton->setChecked(true);

                std::string sID = this->getAttribute(&oChildNode, "id", "");
                if (sID.length() > 0)
                {

                    // TODO m_pID2Widget->find(sID)->second = pStreamOut;
                    pStreamOut->addStream( sID, pButton );
                    pLayout->addWidget( pChildElement );

                } else {
                    delete pChildElement;
                }

            }

        }

        pGroupBox->setLayout(pLayout);

        // must be done here because otherwise the groupbox is the id widget ...
        std::string sID = this->getAttribute(pDOMElement, "id", "");
        if (sID.length() > 0)
        {
            // TODO m_pID2Widget->insert( std::pair<std::string, QWidget*>(sID, pStreamOut));
        }

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

    }


};

#endif //BIOGUI_WINDOWWIDGETSTREAMBOXNODE_H
