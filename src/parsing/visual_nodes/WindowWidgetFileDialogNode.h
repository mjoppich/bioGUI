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

#ifndef BIOGUI_WINDOWWIDGETFILEDIALOGNODE_H
#define BIOGUI_WINDOWWIDGETFILEDIALOGNODE_H

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtCore/QDir>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include "WindowWidgetNode.h"

class WindowWidgetFileDialogNode : public WindowWidgetNode {

public:

    WindowWidgetFileDialogNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetFileDialogNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QWidget* pLineButton = new QWidget();
        QHBoxLayout* pLayout = new QHBoxLayout();


        QLineEdit* pLineEdit = new QLineEdit();
        QString sPathHint = QDir::currentPath();

        QString sLineEditLocation = this->getQAttribute(pDOMElement, "location", "");
        if (sLineEditLocation.length() > 0)
        {
            pLineEdit->setText(sLineEditLocation);

            QFileInfo oLineEditInfo( sLineEditLocation );

            sPathHint = oLineEditInfo.absoluteDir().path();
        }

        QPushButton* pFileButton = new QPushButton(sValue);

        bool bMultiples = (this->getQAttribute(pDOMElement, "multiples", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);
        bool bOutput = (this->getQAttribute(pDOMElement, "output", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);
        bool bFolder = (this->getQAttribute(pDOMElement, "folder", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);
        QString sFileDelim = this->getQAttribute(pDOMElement, "multiples_delim", ";");
        QString sFileFilter = this->getQAttribute(pDOMElement, "filter", "");

        pFileButton->connect(pFileButton,&QAbstractButton::clicked,[pLineEdit, bMultiples, bOutput, bFolder, sFileDelim, sFileFilter, sPathHint] (bool bChecked){


            QString sLocalPathHint = sPathHint;

            QString sLineContent = "";

            if (pLineEdit != NULL)
            {
                sLineContent = pLineEdit->text();

                if (sLineContent.contains(','))
                {
                    QStringList vLines = sLineContent.split(',');
                    sLineContent = vLines[0];
                }

                QFileInfo oLineEditInfo(sLineContent);

                if (oLineEditInfo.isDir())
                {
                    sLocalPathHint = sLineContent;
                } else {

                    if (oLineEditInfo.isFile())
                    {
                        sLocalPathHint = oLineEditInfo.dir().absolutePath();
                    }

                }

            }

            if (bFolder)
            {

                QString sFolder = QFileDialog::getExistingDirectory(0, ("Select Output Folder"), sLocalPathHint);

                if (sFolder.size() != 0)
                    pLineEdit->setText(sFolder);

            } else {


                if (bOutput)
                {

                    if (bMultiples)
                    {
                        LOGERROR("Unsupported: output and multiples");

                    } else {

                        QString sFileName = QFileDialog::getSaveFileName(0, "Select Input File", sLocalPathHint, sFileFilter);

                        if (sFileName.size() != 0)
                            pLineEdit->setText(sFileName);

                    }

                } else {


                    if (bMultiples)
                    {
                        QStringList vSelectedFiles = QFileDialog::getOpenFileNames(0, "Select Input Files", sLocalPathHint, sFileFilter);
                        QString sFiles = vSelectedFiles.join(sFileDelim);

                        if (sFiles.size() != 0)
                            pLineEdit->setText(sFiles);

                    } else {

                        QString sFileName = QFileDialog::getOpenFileName(0, "Select Input File", sLocalPathHint, sFileFilter);

                        if (sFileName.size() != 0)
                            pLineEdit->setText(sFileName);

                    }

                }

            }



        });

        /*
        pLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        pFileButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //pLineButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        pLayout->addWidget( pLineEdit, 2, Qt::AlignLeft );
        pLayout->addWidget( pFileButton, 1, Qt::AlignRight );
*/
        pLayout->addWidget( pLineEdit);
        pLayout->addWidget( pFileButton);

        pLineButton->setLayout( pLayout );

        oReturn.addRetriever(this->getDomID(pDOMElement) , [pLineEdit] () {return pLineEdit->text().toStdString();});

        oReturn.pElement = pLineButton;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {

        vAttributes.push_back( "multiples" );
        vAttributes.push_back( "location" );
        vAttributes.push_back( "output" );
        vAttributes.push_back( "multiples_delim" );
        vAttributes.push_back( "folder" );
        vAttributes.push_back( "filter" );

    }


};

#endif //BIOGUI_WINDOWWIDGETFILEDIALOGNODE_H
