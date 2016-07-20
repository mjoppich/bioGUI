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
#include <QAbstractItemView>
#include <QLineEdit>


class QDownloadTemplatesWindow : public QWidget {

    Q_OBJECT

public:

    QDownloadTemplatesWindow()
    : QWidget()
    {
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

        connect(pTable, &QTableWidget::cellClicked, [this, pTable, pSorting] (int iRow, int iCol){

            std::cerr << iRow << " " << iCol << std::endl;

        } );

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

    void insertRow(QTableWidget* pTable, QString sLine)
    {
        const int iCurrentRow = pTable->rowCount();
        pTable->insertRow( iCurrentRow );

        QStringList vElems = sLine.split("\t");

        qDebug() << vElems;

        QString sTypeID = vElems.at(1);
        QString sType = "";
        if (sTypeID.compare("0") == 0)
        {
            sType = "GUI";
        } else {
            sType = "Install";
        }

        pTable->setItem(iCurrentRow, 0, new QStringTableWidgetItem( sType ));
        pTable->setItem(iCurrentRow, 1, new QStringTableWidgetItem( vElems.at(2) ));
        pTable->setItem(iCurrentRow, 2, new QStringTableWidgetItem( vElems.at(4) ));

        //id
        pTable->setItem(iCurrentRow, 3, new QStringTableWidgetItem( vElems.at(0) ));
        //filename
        pTable->setItem(iCurrentRow, 4, new QStringTableWidgetItem( "" ));
    }

    void populateTable(QTableWidget* pTable)
    {

        QNetworkAccessManager* pNetworkManager = new QNetworkAccessManager(this);

        connect(pNetworkManager, &QNetworkAccessManager::finished, [this, pTable] (QNetworkReply* pReply) {

            QByteArray oReplyData = pReply->readAll();

            QString oReplyLines = QString(oReplyData);

            qDebug() << oReplyLines;

            QStringList vLines = oReplyLines.split("\n");

            for (int i = 0; i < vLines.size(); ++i)
            {
                this->insertRow(pTable, vLines.at(i));
            }

        });

        pNetworkManager->get( QNetworkRequest(QUrl("http://mjoppich.ddns.net:81/biogui/list_templates.php")) );


    }


    QPushButton* m_pDownloadButton = NULL;
    QPushButton* m_pCancelButton = NULL;
    QTableWidget* m_pTable = NULL;

    Qt::SortOrder* m_pSorting = NULL;

};


#endif //BIOGUI_QDOWNLOADTEMPLATESWINDOW_H
