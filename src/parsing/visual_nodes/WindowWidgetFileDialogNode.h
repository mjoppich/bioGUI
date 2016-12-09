//
// Created by joppich on 10/21/16.
//

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

            if (bFolder)
            {

                QString sFolder = QFileDialog::getExistingDirectory(0, ("Select Output Folder"), sPathHint);
                pLineEdit->setText(sFolder);

            } else {


                if (bOutput)
                {

                    if (bMultiples)
                    {
                        LOGERROR("Unsupported: output and multiples");

                    } else {

                        QString sFileName = QFileDialog::getSaveFileName(0, "Select Input File", sPathHint, sFileFilter);
                        pLineEdit->setText(sFileName);

                    }

                } else {


                    if (bMultiples)
                    {
                        QStringList vSelectedFiles = QFileDialog::getOpenFileNames(0, "Select Input Files", sPathHint, sFileFilter);
                        QString sFiles = vSelectedFiles.join(sFileDelim);
                        pLineEdit->setText(sFiles);

                    } else {

                        QString sFileName = QFileDialog::getOpenFileName(0, "Select Input File", sPathHint, sFileFilter);
                        pLineEdit->setText(sFileName);

                    }

                }

            }



        });

        pLayout->addWidget( pLineEdit, 0, Qt::AlignLeft );
        pLayout->addWidget( pFileButton, 0, Qt::AlignLeft );
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
