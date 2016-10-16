//
// Created by joppich on 10/11/16.
//

#ifndef BIOGUI_TEMPLATELISTWIDGET_H
#define BIOGUI_TEMPLATELISTWIDGET_H

#include <QListWidget>
#include <QMenu>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <QFile>

class TemplateListWidget : public QListWidget {

public:

    TemplateListWidget()
            : QListWidget()
    {

        this->setSelectionMode(QAbstractItemView::SingleSelection);
        this->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(this, &TemplateListWidget::customContextMenuRequested, this, &TemplateListWidget::showContextMenu);


    }

    void showContextMenu(const QPoint &pos)
    {
        // Handle global position
        QPoint globalPos = this->mapToGlobal(pos);

        this->clearSelection();

        QListWidgetItem* pItemUnderCursor = this->itemAt(pos);
        pItemUnderCursor->setSelected(true);

        // Create menu and insert some actions
        QMenu myMenu;

QAction* pInfoAction = new QAction(QIcon(":/images/open.png"), tr("&Info"), this);
pInfoAction->setStatusTip(tr("Get Template Info"));
connect(pInfoAction, SIGNAL(triggered()), this, SLOT(infoTemplate));

        myMenu.addAction( pInfoAction );



QAction* pDeleteAction = new QAction(QIcon(":/images/open.png"), tr("&Delete"), this);
pInfoAction->setStatusTip(tr("Delete Template"));
connect(pDeleteAction, SIGNAL(triggered()), this, SLOT(deleteTemplate));
        myMenu.addAction(pDeleteAction);



        // Show context menu at handling position
        myMenu.exec(globalPos);
    }

public slots:

    void deleteTemplate()
    {
        // If multiple selection is on, we need to erase all selected items
        for (int i = 0; i < this->selectedItems().size(); ++i) {
            // Get curent item on selected row
            QListWidgetItem *item = this->selectedItems().at(i);
            // And remove it
            QString sFile = item->data(Qt::UserRole + 2).toString();

            QString sQuestionString = "Do you really want to delete the template located at:\n" + sFile + "\n ? This action can not be undone.";

            QMessageBox::StandardButton oDelQuestAnswer;
            oDelQuestAnswer = QMessageBox::question(this, "Delete Template", sQuestionString, QMessageBox::Yes|QMessageBox::No);

            if (oDelQuestAnswer == QMessageBox::Yes)
            {

                std::cerr << "deleting " << sFile.toStdString() << std::endl;

                QFile oDeleteFile (sFile);

                bool bRemoved = oDeleteFile.remove();

                if (!bRemoved)
                {
                    QString sTheError = oDeleteFile.errorString();

                    QMessageBox::critical(this, "Error Deleting Template", "Error Deleting Template located at:\n" + sFile + "\n Reason: " + sTheError);
                }

            } else {
                std::cerr << "user selected no " << sFile.toStdString() << std::endl;
            }

        }
    }

    void infoTemplate()
    {
        QMessageBox::information(this, "No information", "There is currently no information to display.");
    }

};


#endif //BIOGUI_TEMPLATELISTWIDGET_H
