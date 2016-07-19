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

        m_pTable = this->setupTable();
        pLayout->addWidget(m_pTable);

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

        const int iColCount = 3;
        pTable->setColumnCount( iColCount );

        m_pSorting = (Qt::SortOrder*) calloc(iColCount, sizeof(Qt::SortOrder));

        QTableWidgetItem* pColumnHeader = new QTableWidgetItem("Type");
        pTable->setHorizontalHeaderItem(0, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("Name");
        pTable->setHorizontalHeaderItem(1, pColumnHeader);

        pColumnHeader = new QTableWidgetItem("Author");
        pTable->setHorizontalHeaderItem(2, pColumnHeader);

        QHeaderView* pHView = pTable->horizontalHeader();
        pHView->setSectionResizeMode(QHeaderView::Interactive);
        pHView->setStretchLastSection(true);

        pTable->verticalHeader()->setVisible(false);

        for (int i = 0; i < iColCount; ++i)
        {
            m_pSorting[i] = Qt::SortOrder::AscendingOrder;
        }

        Qt::SortOrder* pSorting = m_pSorting;

        connect(pTable, &QTableWidget::cellClicked, [this, pTable, pSorting] (int iRow, int iCol){

            pTable->sortItems(iCol, m_pSorting[iCol]);

            if (m_pSorting[iCol] == Qt::SortOrder::AscendingOrder)
            {
                m_pSorting[iCol] = Qt::SortOrder::DescendingOrder;
            } else {
                m_pSorting[iCol] = Qt::SortOrder::AscendingOrder;
            }

        } );

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

        pTable->setItem(iCurrentRow, 0, new QStringTableWidgetItem( vElems.at(0) ));
        pTable->setItem(iCurrentRow, 1, new QStringTableWidgetItem( vElems.at(1) ));
        pTable->setItem(iCurrentRow, 2, new QStringTableWidgetItem( vElems.at(2) ));

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

        pNetworkManager->get( QNetworkRequest(QUrl("http://google.de")) );


    }


    QPushButton* m_pDownloadButton = NULL;
    QPushButton* m_pCancelButton = NULL;
    QTableWidget* m_pTable = NULL;

    Qt::SortOrder* m_pSorting = NULL;

};


#endif //BIOGUI_QDOWNLOADTEMPLATESWINDOW_H
