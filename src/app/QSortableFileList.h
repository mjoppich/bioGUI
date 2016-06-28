//
// Created by joppich on 5/25/16.
//

#ifndef BIOGUI_QSORTABLEFILELIST_H
#define BIOGUI_QSORTABLEFILELIST_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

class QSortableFileList : public QWidget {

public:

    QSortableFileList(QString sPathHint, QString sFileFilter, QString sDelimeter)
    : QWidget()
    {


        m_sFileFilter = sFileFilter;
        m_sPathHint = sPathHint;
        m_sDelimeter = sDelimeter;


        m_pUp = new QPushButton("Up");
        QObject::connect(m_pUp, &QAbstractButton::clicked, this, &QSortableFileList::up);

        m_pAddFiles = new QPushButton("Add Files");
        QObject::connect(m_pAddFiles, &QAbstractButton::clicked, this, &QSortableFileList::addFiles);

        m_pRemove = new QPushButton("Remove");
        QObject::connect(m_pRemove, &QAbstractButton::clicked, this, &QSortableFileList::removeFile);

        m_pDown = new QPushButton("Down");
        QObject::connect(m_pDown, &QAbstractButton::clicked, this, &QSortableFileList::down);


        m_pListWidget = new QListWidget(this);


        /*
         * ARRANGE ELEMENTS
         *
         */

        QHBoxLayout* pBottomLayout = new QHBoxLayout(this);
        pBottomLayout->addWidget(m_pUp, 0, Qt::AlignLeft);
        pBottomLayout->addWidget(m_pAddFiles, 0, Qt::AlignLeft);
        pBottomLayout->addWidget(m_pRemove, 0, Qt::AlignRight);
        pBottomLayout->addWidget(m_pDown, 0, Qt::AlignRight);

        QWidget* pBottom = new QWidget(this);
        pBottom->setLayout(pBottomLayout);

        QVBoxLayout* pLayout = new QVBoxLayout(this);
        pLayout->addWidget(m_pListWidget);
        pLayout->addWidget(pBottom);

        this->setLayout(pLayout);

    }

    QString evaluate()
    {
        QString sReturn = "";

        for (size_t i = 0; i < m_pListWidget->count(); ++i)
        {

            QListWidgetItem* pItem = m_pListWidget->item(i);
            QString sItem = pItem->text();

            if (i > 0)
                sReturn.append(m_sDelimeter);

            sReturn.append(sItem);
        }

        return sReturn;

    }

protected:

    void addFiles()
    {

        QStringList vSelectedFiles = QFileDialog::getOpenFileNames(0, "Select Input Files", m_sPathHint, m_sFileFilter);

        for (size_t i = 0; i < vSelectedFiles.size(); ++i)
        {

            QString sFileName = vSelectedFiles.at(i);

            this->addFile(sFileName);

        }



    }

    void addFile(QString& sFile)
    {

        m_pListWidget->addItem(sFile);

    }

    void up()
    {

        int iCurrentRow = m_pListWidget->currentRow();
        QListWidgetItem* pSelected = m_pListWidget->takeItem( iCurrentRow );

        if (pSelected == NULL)
            return;

        int iNewRow = iCurrentRow-1;
        m_pListWidget->insertItem(iNewRow, pSelected);
        m_pListWidget->setCurrentRow(iNewRow);

    }

    void down()
    {

        int iCurrentRow = m_pListWidget->currentRow();
        QListWidgetItem* pSelected = m_pListWidget->takeItem( iCurrentRow );

        if (pSelected == NULL)
            return;

        int iNewRow = iCurrentRow+1;
        m_pListWidget->insertItem(iNewRow, pSelected);
        m_pListWidget->setCurrentRow(iNewRow);
    }

    void removeFile()
    {
        int iCurrentRow = m_pListWidget->currentRow();
        QListWidgetItem* pSelected = m_pListWidget->takeItem( iCurrentRow );

        if (pSelected == NULL)
            return;

        delete pSelected;
    }





    QListWidget* m_pListWidget;
    QPushButton* m_pUp;
    QPushButton* m_pDown;
    QPushButton* m_pRemove;
    QPushButton* m_pAddFiles;

    QString m_sPathHint;
    QString m_sFileFilter;
    QString m_sDelimeter;

};


#endif //BIOGUI_QSORTABLEFILELIST_H
