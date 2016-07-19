//
// Created by joppich on 7/19/16.
//

#ifndef BIOGUI_QSTRINGTABLEWIDGETITEM_H
#define BIOGUI_QSTRINGTABLEWIDGETITEM_H

#include <QTableWidgetItem>

class QStringTableWidgetItem : public QTableWidgetItem {

public:

    QStringTableWidgetItem(QString sText)
            : QTableWidgetItem()
    {

        this->setData(Qt::EditRole, sText);

    }

    bool operator <(const QTableWidgetItem &other) const
    {

        QString sThisText = this->data(Qt::EditRole).toString();
        QString sOtherText = other.data(Qt::EditRole).toString();

        return sThisText < sOtherText;
    }

};


#endif //BIOGUI_QSTRINGTABLEWIDGETITEM_H
