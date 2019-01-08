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

#ifndef BIOGUI_QDOWNLOADTEMPLATESWINDOW_H
#define BIOGUI_QDOWNLOADTEMPLATESWINDOW_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHeaderView>
#include <iostream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "QStringTableWidgetItem.h"
#include "QNetworkReplyTimer.h"
#include <QAbstractItemView>
#include <QLineEdit>
#include <QtCore/QFileInfo>
#include <QTemporaryFile>
#include <QtCore/QTemporaryDir>

#include <QMessageBox>


class QDownloadTemplatesWindow : public QWidget {

    Q_OBJECT

public:

    QDownloadTemplatesWindow(QString sTemplateDir, QString& sServerLocation)
    : QWidget(), m_sServerLocation(sServerLocation)
    {

        m_sTargetDirectory = sTemplateDir;

        this->setMinimumSize(900,600);

        QLayout* pLayout = new QVBoxLayout();


        m_pDownloadButton = new QPushButton("Download");
        m_pCancelButton = new QPushButton("Cancel");
        QWidget* pButtonWidget = new QWidget();
        QLayout* pButtonLayout = new QHBoxLayout();
        pButtonLayout->addWidget( m_pDownloadButton );
        pButtonLayout->addWidget( m_pCancelButton );
        pButtonWidget->setLayout(pButtonLayout);

        pLayout->setAlignment(Qt::AlignLeft);

        m_pTable = this->setupTable();

        QLabel* pHints = new QLabel("Hint: Double-click header to sort by column!");
        pLayout->addWidget(pHints);
        pLayout->addWidget(m_pTable);

        QLabel* pLabel = new QLabel("Filter: ");
        QLineEdit* pFilterEdit = new QLineEdit();

        QWidget* pFilterWidget = new QWidget();
        QHBoxLayout* pFilterLayout = new QHBoxLayout();

        pFilterLayout->addWidget(pLabel);
        pFilterLayout->addWidget(pFilterEdit);
        pFilterWidget->setLayout(pFilterLayout);

        connect(pFilterEdit, &QLineEdit::textChanged, [this] (const QString& text) {
           this->filterTemplates(text);
        });

        pLayout->addWidget(pFilterWidget);

        pLayout->addWidget(pButtonWidget);

        this->setLayout(pLayout);


        /*
         *
         * ACTIONS
         *
         */

        connect(m_pCancelButton, &QAbstractButton::clicked, [this] () {
            this->close();
        });

        connect(m_pDownloadButton, &QAbstractButton::clicked, [this] () {

            QItemSelectionModel *pSelection = this->m_pTable->selectionModel();

            if (!pSelection->hasSelection())
                return;

            QModelIndexList oSelRows = pSelection->selectedRows();

            // Multiple rows can be selected
            for(int i=0; i< oSelRows.count(); i++)
            {
                QModelIndex index = oSelRows.at(i);
                qDebug() << index.row();

                int iRowTemplateID = m_pTable->item(index.row(), 4)->data(Qt::EditRole).toInt();

                this->downloadTemplate(iRowTemplateID);
            }

        });

    }

    QTableWidget* setupTable()
    {
        QTableWidget* pTable = new QTableWidget();

        pTable->setAlternatingRowColors(true);


        const int iColCount = 5;
        pTable->setColumnCount( iColCount );

        m_pSorting = (Qt::SortOrder*) calloc(iColCount, sizeof(Qt::SortOrder));

        QTableWidgetItem* pColumnHeader = NULL;


        pColumnHeader = new QTableWidgetItem("Type");
        pTable->setHorizontalHeaderItem(0, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("Template Name");
        pTable->setHorizontalHeaderItem(1, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("Template Author");
        pTable->setHorizontalHeaderItem(2, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("Category");
        pTable->setHorizontalHeaderItem(3, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("id");
        pTable->setHorizontalHeaderItem(4, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("filename");
        pTable->setHorizontalHeaderItem(5, pColumnHeader);



        pTable->setColumnHidden(4, true);
        pTable->setColumnHidden(5, true);

        QHeaderView* pHView = pTable->horizontalHeader();
        pHView->setSectionResizeMode(QHeaderView::Interactive);
        pHView->setStretchLastSection(true);

        pTable->verticalHeader()->setVisible(false);

        for (int i = 0; i < iColCount; ++i)
        {
            m_pSorting[i] = Qt::SortOrder::AscendingOrder;
        }

        Qt::SortOrder* pSorting = m_pSorting;

        connect(pHView, &QHeaderView::sectionDoubleClicked, [this, pTable, pSorting] (int iCol){

            pTable->sortItems(iCol, m_pSorting[iCol]);

            if (m_pSorting[iCol] == Qt::SortOrder::AscendingOrder)
            {
                m_pSorting[iCol] = Qt::SortOrder::DescendingOrder;
            } else {
                m_pSorting[iCol] = Qt::SortOrder::AscendingOrder;
            }

        } );

        /*
        connect(pTable, &QTableWidget::cellClicked, [this, pTable, pSorting] (int iRow, int iCol){

            std::cerr << iRow << " " << iCol << std::endl;

        } );
        */

        pTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        this->populateTable(pTable);

        return pTable;
    }

    virtual void closeEvent ( QCloseEvent * event )
    {
        QWidget::closeEvent(event);

        emit closed();
    }

signals:
    void closed();

protected:

    void filterTemplates(const QString& sText)
    {
        if (this->m_pTable == NULL)
            return;

        int iRows = m_pTable->rowCount();
        int iCols = m_pTable->columnCount();

        for (int i = 0; i < iRows; ++i)
        {
            bool bKeep = false;

            for (int j = 0; j < iCols; ++j)
            {

                QString sData = m_pTable->item(i,j)->data(Qt::EditRole).toString();

                if (sData.contains(sText, Qt::CaseInsensitive))
                {
                    bKeep = true;
                    break;
                }

            }

            if (!bKeep)
            {

                m_pTable->hideRow(i);

            } else {

                m_pTable->showRow(i);

            }



        }

    }

    void insertRow(QTableWidget* pTable, QString sLine)
    {
        const int iCurrentRow = pTable->rowCount();
        pTable->insertRow( iCurrentRow );

        QStringList vElems = sLine.split("\t");

        qDebug() << vElems;

        if (((sLine.size() == 0) || vElems.size() == 0))
            return;

        QString sTypeID = vElems.at(1);
        QString sType = "";
        int iType = 0;
        if (sTypeID.compare("0") == 0)
        {
            sType = "GUI";
        } else {
            sType = "Install";
            iType = 1;
        }

        pTable->setItem(iCurrentRow, 0, new QStringTableWidgetItem( sType ));
        pTable->setItem(iCurrentRow, 1, new QStringTableWidgetItem( vElems.at(2) ));
        pTable->setItem(iCurrentRow, 2, new QStringTableWidgetItem( vElems.at(4) ));

        //id
        pTable->setItem(iCurrentRow, 4, new QStringTableWidgetItem( vElems.at(0) ));
        //filename
        pTable->setItem(iCurrentRow, 5, new QStringTableWidgetItem( "" ));


        /*
         * CATEGORIES
         */
        QString sCategories = "";

        QString sAllCats = vElems.at(5);
        QStringList aCats = sAllCats.split(",");

        for (int i = 0; i < aCats.size(); ++i)
        {
            int iCatID = aCats.at(i).toInt();

            std::map<int, QString>::iterator oIt = m_mID2Category.find( iCatID );

            if ((oIt != m_mID2Category.end()) && (i > 0))
            {
                sCategories += ", ";
            }

            if (oIt != m_mID2Category.end())
            {

                sCategories += oIt->second;

            }
        }

        pTable->setItem(iCurrentRow, 3, new QStringTableWidgetItem( sCategories ));

        QString sID = vElems.at(0);
        int iID = sID.toInt();

        m_mID2Type.insert(std::pair<int,int>(iID, iType));
    }

    void populateTable(QTableWidget* pTable)
    {

        this->getAvailableCategories();
        this->fetchAvailableTemplates(pTable);

    }

    void getAvailableCategories()
    {


        m_mID2Category.clear();

        QNetworkAccessManager* pNetworkManager = new QNetworkAccessManager(this);

        connect(pNetworkManager, &QNetworkAccessManager::finished, [this] (QNetworkReply* pReply) {

            qDebug() << pReply->errorString();
            qDebug() << pReply->isRunning();
            if (! pReply->isReadable() )
            {
                qDebug() << "not readable";
                return;
            }

            QByteArray oReplyData = pReply->readAll();

            QString oReplyLines = QString(oReplyData);

            if (oReplyLines.size() == 0)
            {
                QMessageBox::critical(this, "The server does not respond", "The application received an empty result. Error code: " + pReply->errorString());

                return;
            }

            qDebug() << oReplyLines;

            QStringList vLines = oReplyLines.split("\n");

            for (int i = 0; i < vLines.size(); ++i)
            {

                QString sLine = vLines.at(i);
                QStringList aLine = sLine.split("\t");

                if (aLine.length() != 2)
                {
                    continue;
                }

                int iID = aLine[0].toInt();
                QString sCategory = aLine[1];

                m_mID2Category.insert(std::pair<int, QString>(iID, sCategory));
            }

        });

        QString sQueryURL = m_sServerLocation + "/list_categories.php";

        qDebug() << sQueryURL;

        this->querySSLURL(sQueryURL, pNetworkManager);

    }

    void fetchAvailableTemplates( QTableWidget* pTable )
    {
        m_mID2Type.clear();

        QNetworkAccessManager* pNetworkManager = new QNetworkAccessManager(this);

        connect(pNetworkManager, &QNetworkAccessManager::finished, [this, pTable] (QNetworkReply* pReply) {

            qDebug() << pReply->errorString();
            qDebug() << pReply->isRunning();
            if (! pReply->isReadable() )
            {
                qDebug() << "not readable";
                return;
            }

            QByteArray oReplyData = pReply->readAll();

            QString oReplyLines = QString(oReplyData);

            if (oReplyLines.size() == 0)
            {
                QMessageBox::critical(this, "The server does not respond", "The application received an empty result. Error code: " + pReply->errorString());

                return;
            }

            qDebug() << oReplyLines;

            QStringList vLines = oReplyLines.split("\n");

            for (int i = 0; i < vLines.size(); ++i)
            {

                QString sLine = vLines.at(i);

                if (sLine.length() > 0)
                    this->insertRow(pTable, sLine);
            }

        });

        QString sQueryURL = m_sServerLocation + "/list_templates.php";

        qDebug() << sQueryURL;

        this->querySSLURL(sQueryURL, pNetworkManager);
    }

    void querySSLURL(QString& sURL, QNetworkAccessManager* pNetworkManager)
    {
        QNetworkRequest oRequest = QNetworkRequest(QUrl(sURL));
        QSslConfiguration oSSLConfig = oRequest.sslConfiguration();
        oSSLConfig.setProtocol(QSsl::AnyProtocol);
        oSSLConfig.setPeerVerifyMode(QSslSocket::VerifyNone);

        oRequest.setSslConfiguration(oSSLConfig);
        QNetworkReply* pReply = pNetworkManager->get( oRequest );

        QList<QSslError> expectedSslErrors;
        expectedSslErrors.append( QSslError(QSslError::SelfSignedCertificate) );
        pReply->ignoreSslErrors(expectedSslErrors);

        QNetworkReplyTimer::set(pReply, 2000, [this] () {
            QMessageBox::critical(this, "A timeout has occurred.", "The application failed to reach the template server.");
        });
    }

    void downloadTemplate(int iID)
    {
        QString sDownloadDir = QString(m_sTargetDirectory);

        std::map<int, int>::iterator oIt = m_mID2Type.find(iID);

        if (oIt == m_mID2Type.end())
            return;

        int iType = oIt->second;
        QString sFileExtension = "";

        switch (iType)
        {

        case 0:
            sDownloadDir.append("/templates/");
            sFileExtension = ".gui";
            break;

        case 1:

            sDownloadDir.append("/install_templates/");
            sFileExtension = ".igui";
            break;

        }

        QTemporaryFile oUniqueFileName( sDownloadDir + "biogui_template.XXXXXX" + sFileExtension);
        oUniqueFileName.open();

        qDebug() << sDownloadDir;
        qDebug() << QDir::current();
        qDebug() << oUniqueFileName.fileName();

        QString sFilename = oUniqueFileName.fileName();
        oUniqueFileName.close();

        QFileInfo oInfo(sFilename);

        QNetworkAccessManager* pNetworkManager = new QNetworkAccessManager(this);

        connect(pNetworkManager, &QNetworkAccessManager::finished, [this, sFilename] (QNetworkReply* pReply) {

            qDebug() << pReply->url();

            QByteArray oReplyData = pReply->readAll();

            QString oReplyLines = QString(oReplyData);

            qDebug() << oReplyLines;
            qDebug() << "trying to save to " << sFilename;

            QFile file ( sFilename ) ;

            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                std::cerr << "could not open " << sFilename.toStdString() << " for writing template" << std::endl;

                return;
            }


            QTextStream oSaveStream(&file);
            oSaveStream << oReplyLines;
            oSaveStream.flush();

            QMessageBox* msgBox = new QMessageBox();
            msgBox->setWindowTitle("Template Installed");

            QString sMessage = "Template downloaded into file:\n" + sFilename;

            msgBox->setText(sMessage);
            msgBox->setWindowFlags(Qt::WindowStaysOnTopHint);
            msgBox->show();

        });


        QString sURL = m_sServerLocation + "/get_template.php?templid=";
        sURL.append( std::to_string(iID).c_str() );

        this->querySSLURL(sURL, pNetworkManager);

    }

    QPushButton* m_pDownloadButton = NULL;
    QPushButton* m_pCancelButton = NULL;
    QTableWidget* m_pTable = NULL;

    Qt::SortOrder* m_pSorting = NULL;
    QString m_sTargetDirectory;

    QString m_sServerLocation = "";

    std::map<int, int> m_mID2Type;
    std::map<int, QString> m_mID2Category;



};


#endif //BIOGUI_QDOWNLOADTEMPLATESWINDOW_H
