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

#ifndef BIOGUI_QCOMBOITEM_H
#define BIOGUI_QCOMBOITEM_H

#include <QLabel>
#include <QString>
#include <QVariant>
#include <QtWidgets/qlabel.h>

class QComboItem : public QLabel {
Q_OBJECT
public:

    QComboItem(QString sValue, QVariant oData)
    : QLabel(sValue)
    {

        m_sValue = sValue;
        m_oData = oData;

    }

    QString getValue()
    {
        return m_sValue;
    }

    QVariant getData()
    {
        return m_oData;
    }

    void setValue(QString sString)
    {
        m_sValue = sString;
    }

    void setData(QVariant oData)
    {
        m_oData = oData;
    }

protected:

    QString m_sValue;
    QVariant m_oData;


};


#endif //BIOGUI_QCOMBOITEM_H
