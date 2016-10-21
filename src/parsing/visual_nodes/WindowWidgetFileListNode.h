//
// Created by joppich on 10/21/16.
//

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

    WindowWidgetFileListNode()
            : WindowWidgetNode()
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

        QString sCurrentPath = QDir::current().absolutePath();
        QString sSearchPath = this->getAttribute(pDOMElement, "path", sCurrentPath + "/install_templates/");

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

        QStringList vFileEnding = this->getAttribute(pDOMElement, "ext", "*.igui").split(",");

        std::cerr << "starting in " << sCurrentPath.toStdString() << std::endl;
        std::cerr << "searching in " << sSearchPath.toStdString() << std::endl;

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
        oReturn.bHasRetriever = true;
        oReturn.oRetriever = [pComboBox] () {


            QVariant oSelected = pComboBox->currentData();

            if (oSelected.isNull())
                return std::string("");

            return oSelected.toString().toStdString();

        };

        oReturn.pElement = pComboBox;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "width" );
        vAttributes.push_back( "height" );
        vAttributes.push_back( "title" );

    }


};

#endif //BIOGUI_WINDOWWIDGETFILELISTNODE_H
