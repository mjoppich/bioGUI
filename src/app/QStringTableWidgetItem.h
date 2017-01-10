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

#ifndef BIOGUI_QSTRINGTABLEWIDGETITEM_H
#define BIOGUI_QSTRINGTABLEWIDGETITEM_H

#include <QTableWidgetItem>

class QStringTableWidgetItem : public QTableWidgetItem {

public:

    QStringTableWidgetItem(QString sText)
            : QTableWidgetItem()
    {

        this->setData(Qt::EditRole, sText);
        this->setFlags(this->flags() &  ~Qt::ItemIsEditable);

    }

    bool operator <(const QTableWidgetItem &other) const
    {

        QString sThisText = this->data(Qt::EditRole).toString();
        QString sOtherText = other.data(Qt::EditRole).toString();

        return sThisText < sOtherText;
    }

};


#endif //BIOGUI_QSTRINGTABLEWIDGETITEM_H
