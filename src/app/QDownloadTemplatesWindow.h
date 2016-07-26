//
// Created by joppich on 7/19/16.
//

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

    QDownloadTemplatesWindow(QString sTemplateDir)
    : QWidget()
    {

        m_sTargetDirectory = sTemplateDir;

        this->setMinimumSize(200,200);

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

                int iRowTemplateID = m_pTable->item(index.row(), 3)->data(Qt::EditRole).toInt();

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

        pColumnHeader = new QTableWidgetItem("Name");
        pTable->setHorizontalHeaderItem(1, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("Author");
        pTable->setHorizontalHeaderItem(2, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("id");
        pTable->setHorizontalHeaderItem(3, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("filename");
        pTable->setHorizontalHeaderItem(4, pColumnHeader);

        pTable->setColumnHidden(3, true);
        pTable->setColumnHidden(4, true);

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
            bool bKeep = False;

            for (int j = 0; j < iCols; ++j)
            {



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
        pTable->setItem(iCurrentRow, 3, new QStringTableWidgetItem( vElems.at(0) ));
        //filename
        pTable->setItem(iCurrentRow, 4, new QStringTableWidgetItem( "" ));

        QString sID = vElems.at(0);
        int iID = sID.toInt();

        m_mID2Type.insert(std::pair<int,int>(iID, iType));
    }

    void populateTable(QTableWidget* pTable)
    {

        m_mID2Type.clear();

        QNetworkAccessManager* pNetworkManager = new QNetworkAccessManager(this);

        connect(pNetworkManager, &QNetworkAccessManager::finished, [this, pTable] (QNetworkReply* pReply) {

            QByteArray oReplyData = pReply->readAll();

            QString oReplyLines = QString(oReplyData);

            if (oReplyLines.size() == 0)
                return;

            qDebug() << oReplyLines;

            QStringList vLines = oReplyLines.split("\n");

            for (int i = 0; i < vLines.size(); ++i)
            {
                this->insertRow(pTable, vLines.at(i));
            }

        });

        QNetworkRequest oRequest = QNetworkRequest(QUrl("http://mjoppich.ddns.net:81/biogui/list_templates.php"));
        QNetworkReply* pReply = pNetworkManager->get( oRequest );

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

        QDir oCurrentWorkDir = QDir::current();
        QDir::setCurrent( sDownloadDir );

        QTemporaryFile oUniqueFileName("biogui_template.XXXXXX" + sFileExtension);
        oUniqueFileName.open();

        qDebug() << sDownloadDir;
        qDebug() << QDir::current();
        qDebug() << oUniqueFileName.fileName();

        QString sFilename = oUniqueFileName.fileName();
        oUniqueFileName.close();

        QFileInfo oInfo(sFilename);

        QDir::setCurrent(oCurrentWorkDir.path());

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
        });


        QString sURL = "http://mjoppich.ddns.net:81/biogui/get_template.php?templid=";
        sURL.append( std::to_string(iID).c_str() );

        QNetworkReply* pReply = pNetworkManager->get( QNetworkRequest(QUrl( sURL )) );
        QNetworkReplyTimer::set(pReply, 2000, [this] () {

            QMessageBox::critical(this, "A timeout has occurred.", "The application failed to reach the template server.");

        });


    }


    QPushButton* m_pDownloadButton = NULL;
    QPushButton* m_pCancelButton = NULL;
    QTableWidget* m_pTable = NULL;

    Qt::SortOrder* m_pSorting = NULL;
    QString m_sTargetDirectory;

    std::map<int, int> m_mID2Type;



};


#endif //BIOGUI_QDOWNLOADTEMPLATESWINDOW_H
