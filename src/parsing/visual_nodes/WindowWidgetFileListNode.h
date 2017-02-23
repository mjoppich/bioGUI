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

#ifndef BIOGUI_WINDOWWIDGETFILELISTNODE_H
#define BIOGUI_WINDOWWIDGETFILELISTNODE_H

#include <src/app/QComboItem.h>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtWidgets/QComboBox>
#include <QtCore/QDir>
#include <iostream>
#include <QtCore/QDirIterator>
#include <QtCore/QTextStream>
#include "WindowWidgetNode.h"

class WindowWidgetFileListNode : public WindowWidgetNode {

public:

    WindowWidgetFileListNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetFileListNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QComboBox *pComboBox = new QComboBox();

        QString sCurrentPath = this->m_pFactory->getApp()->getAppPath();
        QString sSearchPath = this->getQAttribute(pDOMElement, "path", sCurrentPath + "/install_templates/");

        bool bHasPathSet = this->hasAttribute(pDOMElement, "path");

        if (bHasPathSet)
        {

            QFileInfo oSetPath(sSearchPath);

            if (oSetPath.isRelative())
            {
                std::cerr << "Changed path from " << sSearchPath.toStdString();
                sSearchPath = sCurrentPath + "/" + sSearchPath;

                std::cerr << " to " << sSearchPath.toStdString() << std::endl;
            }

        }

        QStringList vFileEnding = this->getQAttribute(pDOMElement, "ext", "*.igui").split(",");

        std::cerr << "starting in " << sCurrentPath.toStdString() << std::endl;
        std::cerr << "searching in " << sSearchPath.toStdString() << std::endl;

        bool bAllowEmpty = (this->getQAttribute(pDOMElement, "allowempty", "false").compare("True", Qt::CaseInsensitive) == 0);

        if (bAllowEmpty)
        {
            QComboItem* pNewItem = new QComboItem( "", "" );
            pComboBox->addItem( pNewItem->getValue(), pNewItem->getData() );
        }

        QDirIterator oDirIterator( sSearchPath,
                                   vFileEnding,
                                   QDir::NoSymLinks | QDir::Files,
                                   QDirIterator::NoIteratorFlags );

        while (oDirIterator.hasNext()) {

            QString sFoundFile = oDirIterator.next();

            QFileInfo oFileInfo(sFoundFile);

            QString sFilePath = sFoundFile;
            QString sFileName = oFileInfo.completeBaseName();

            /*
             * if this is an install template, now only show filename but read first line ...
             * */
            if (!bHasPathSet)
            {
                QFile oFile( sFoundFile );
                oFile.open(QFile::ReadOnly);
                QTextStream in(&oFile);
                QString sFirstLine = in.readLine();

                if (sFirstLine.startsWith("#!"))
                {

                    QString sName = sFirstLine.right(sFirstLine.size()-2);

                    sName = sName.trimmed();

                    sFileName = sName;
                }


            }

            QComboItem* pNewItem = new QComboItem( sFileName, sFilePath);
            pComboBox->addItem( pNewItem->getValue(), pNewItem->getData() );

        }


        oReturn.bHasChildrenFinished = true;

        oReturn.addRetriever(this->getDomID(pDOMElement) , [pComboBox] () {


            QVariant oSelected = pComboBox->currentData();

            if (oSelected.isNull())
                return std::string("");

            return oSelected.toString().toStdString();

        });

        oReturn.pElement = pComboBox;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "path" );
        vAttributes.push_back( "ext" );
        vAttributes.push_back( "allowempty" );

    }


};

#endif //BIOGUI_WINDOWWIDGETFILELISTNODE_H
