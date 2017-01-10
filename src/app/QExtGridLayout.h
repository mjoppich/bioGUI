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

#ifndef BIOGUI_QEXTGRIDLAYOUT_H
#define BIOGUI_QEXTGRIDLAYOUT_H

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QWidget>
#include <iterator>
#include <vector>
#include <map>
#include "QOrderedLayout.h"

class QExtGridLayout : public QGridLayout, public QOrderedLayout {

    Q_OBJECT
public:

    QExtGridLayout(int iRows, int iCols)
            :QGridLayout(), QOrderedLayout()
    {


        m_iRows = iRows;
        m_iCols = iCols;

        m_iAddedWidgets = 0;


    }

    int getRows()
    {
        return m_iRows;
    }

    int getCols()
    {
        return m_iCols;
    }

    void addNextWidget(QWidget* pWidget)
    {
        QWidget* pAddWidget = pWidget;

        int iCol = m_iAddedWidgets % m_iCols;
        int iRow = m_iAddedWidgets / m_iCols;

        this->addWidget(pAddWidget, iRow, iCol); // , Qt::AlignLeft

        ++m_iAddedWidgets;
    }

    void removeWidget(QWidget* pWidget)
    {
        QGridLayout::removeWidget(pWidget);
    }

protected:

    void addWidgetAtPosition(QWidget* pWidget, size_t iPosition)
    {
        int iCol = iPosition % m_iCols;
        int iRow = iPosition / m_iCols;

        pWidget->setParent(this->parentWidget());
        this->addWidget(pWidget, iRow, iCol);
    }

    int m_iAddedWidgets = 0;
    int m_iCols = 0;
    int m_iRows = 0;
};


#endif //BIOGUI_QEXTGRIDLAYOUT_H
