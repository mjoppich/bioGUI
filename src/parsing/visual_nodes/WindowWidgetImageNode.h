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

#ifndef BIOGUI_WINDOWWIDGETIMAGENODE_H
#define BIOGUI_WINDOWWIDGETIMAGENODE_H

#include <QtWidgets/QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include "WindowWidgetNode.h"
#include <iostream>
#include <src/app/AdvancedStreamBox.h>
#include <QDir>
#include <QGroupBox>
#include <QPushButton>
#include <src/bioGUIapp.h>

class QZoomableGraphicsView : public QGraphicsView
{
Q_OBJECT
public:

    QZoomableGraphicsView(QGraphicsScene* pScene)
            : QGraphicsView(pScene)
    {
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        this->setSizeAdjustPolicy(SizeAdjustPolicy::AdjustIgnored);

        AdvancedCornerWidget* pCornerWidget = new AdvancedCornerWidget(this);

        this->connect(pCornerWidget, &AdvancedCornerWidget::sizeChanged, this, &QZoomableGraphicsView::sizeChanged);

        this->setCornerWidget( pCornerWidget );

    }


    void wheelEvent(QWheelEvent *event){

        this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        // Scale the view / do the zoom
        double scaleFactor = 1.15;
        if(event->delta() > 0) {
            // Zoom in
            this-> scale(scaleFactor, scaleFactor);

        } else {
            // Zooming out
            this->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }

    }

    virtual QSize sizeHint() {
        return QSize(this->minimumWidth(), this->minimumHeight());
    }

    signals:

    void sizeChanged();


};

class WindowWidgetImageNode : public WindowWidgetNode {

public:

    WindowWidgetImageNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetImageNode()
    {

    }

    QZoomableGraphicsView* createImage(QString sFileName)
    {

        QDir oDirectory(sFileName);
        std::cerr << "Loading image " << oDirectory.absolutePath().toStdString() << std::endl;

        QGraphicsScene* pScene = new QGraphicsScene();
        QZoomableGraphicsView* pView = new QZoomableGraphicsView(pScene);
        QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(QPixmap( sFileName ));

        pScene->addItem( pItem );

        return pView;
    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement)
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;
        oReturn.bHasChildrenFinished = true;

        QGroupBox* pGroupBox = new QGroupBox();


        QString sFileName = this->getQAttribute(pDOMElement, "src", "");
        QZoomableGraphicsView* pView = this->createImage(sFileName);

        QString sWidth =  this->getQAttribute(pDOMElement, "width" , "");
        QString sHeight = this->getQAttribute(pDOMElement, "height", "");

        if ((sWidth.length() > 0) && (sHeight.length() > 0))
        {
            int iWidth = sWidth.toInt();
            int iHeight = sHeight.toInt();

            pView->setFixedSize(iWidth, iHeight);
        }

        // this could also be a hboxlayout or a grid layout
        QLayout* pLayout = new QVBoxLayout();
        pLayout->addWidget( pView );

        QPushButton* pClearButton = new QPushButton("Clear");

        QObject::connect(pClearButton, &QAbstractButton::clicked, [pView] {

            // do something here

        });

        pLayout->addWidget(pClearButton);

        QPushButton* pSaveLogButton = new QPushButton("Full View");

        QObject::connect(pSaveLogButton, &QAbstractButton::clicked, [pView] {

            // do something here

        });

        pLayout->addWidget(pSaveLogButton);

        pGroupBox->setLayout(pLayout);

        oReturn.pElement = pGroupBox;

        bioGUIapp* pApp = m_pFactory->getApp();
        pLayout->setSizeConstraint(QLayout::SetFixedSize);

        QObject::connect(pView, &QZoomableGraphicsView::sizeChanged, [pApp, pLayout] () {

            pLayout->activate();
            pLayout->update();

            //pApp->getMainWindow()->update();
            pApp->reloadAppWindow();

        });

        QObject::connect(pView, &QZoomableGraphicsView::sizeChanged, [pGroupBox] () {

            pGroupBox->activateWindow();

            pGroupBox->parentWidget()->activateWindow();
            pGroupBox->parentWidget()->layout()->activate();
            pGroupBox->parentWidget()->layout()->update();

        });

        // must be done here because otherwise the groupbox is the id widget ...
        std::string sID = this->getAttribute(pDOMElement, "id", "");
        if (sID.length() > 0)
        {

            WidgetFunctionNode* pWidgetFuncNode = new WidgetFunctionNode(pView, [pView] (const QWidget* pWidget, std::string key, std::string value) {

                QString sKey(key.c_str());

                if (sKey.compare("src", Qt::CaseInsensitive) == 0)
                {

                    QFileInfo oFile(QString(value.c_str()));


                    if (!oFile.exists())
                    {
                        LOGLVL("Image Upd src: File does not exist: " + value, Logging::ERR)

                        return;
                    }

                    LOGLVL("Image Upd src: Loading Image: " + value, Logging::INFO)

                    QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(QPixmap( oFile.absoluteFilePath() ));
                    QGraphicsScene* pScene = pView->scene();

                    pScene->clear();
                    pScene->addItem( pItem );

                }

                return;
            });
            m_pFactory->getApp()->getWindowParser()->addID2WidgetFunction( sID, pWidgetFuncNode, true );

        }

        return oReturn;


    }

    virtual CreatedElement getWindowElementImg( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QString sFileName = this->getQAttribute(pDOMElement, "src", "");

        QDir oDirectory(sFileName);
        std::cerr << "Loading image " << oDirectory.absolutePath().toStdString() << std::endl;

        QGraphicsScene* pScene = new QGraphicsScene();
        QZoomableGraphicsView* pView = new QZoomableGraphicsView(pScene);
        QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(QPixmap( sFileName ));

        pScene->addItem( pItem );

        /*
         * apply sizes here
         *
         */

        QString sWidth =  this->getQAttribute(pDOMElement, "width" , "");
        QString sHeight = this->getQAttribute(pDOMElement, "height", "");

        if ((sWidth.length() > 0) && (sHeight.length() > 0))
        {
            int iWidth = sWidth.toInt();
            int iHeight = sHeight.toInt();

            pView->setFixedSize(iWidth, iHeight);
            //pView->setFixedSize(iWidth, iHeight);
        }
        bioGUIapp* pApp = m_pFactory->getApp();

        QObject::connect(pView, &QZoomableGraphicsView::sizeChanged, [pView, pApp] () {

            LOGLVL(std::to_string(pView->sizeHint().height()) + " " + std::to_string(pView->sizeHint().width()), Logging::DEBUG);

            pView->setFixedSize(pView->sizeHint());

            QWidget* pParent = pView->parentWidget();


            if (pParent != NULL)
            {
                QLayout* pLayout = pParent->layout();

                if (pLayout != NULL)
                {
                    pLayout->activate();
                    pLayout->update();
                }
            }

            pApp->reloadAppWindow();

        });

        QWidget* pImageWidget = new QWidget();
        QLayout* pLayout = new QHBoxLayout();

        pLayout->addWidget(pView);
        pLayout->setSizeConstraint(QLayout::SetFixedSize);
        pView->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        pImageWidget->setLayout(pLayout);

        oReturn.bHasChildrenFinished = true;
        oReturn.pElement = pImageWidget;

        WidgetFunctionNode* pWidgetNode = new WidgetFunctionNode(pImageWidget, [pScene] (const QWidget* pWidget, std::string key, std::string value) {

            QString sKey(key.c_str());

            if (sKey.compare("src", Qt::CaseInsensitive) == 0)
            {

                QFileInfo oFile(QString(value.c_str()));


                if (!oFile.exists())
                {
                    LOGLVL("Image Upd src: File does not exist: " + value, Logging::ERR)

                    return;
                }

                LOGLVL("Image Upd src: Loading Image: " + value, Logging::INFO)

                QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(QPixmap( oFile.absoluteFilePath() ));

                pScene->clear();
                pScene->addItem( pItem );

            }

            return;
        });

        oReturn.pWidgetFuncNode = pWidgetNode;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "width" );
        vAttributes.push_back( "height" );
        vAttributes.push_back( "src" );

    }


};

#endif //BIOGUI_WINDOWWIDGETIMAGENODE_H
