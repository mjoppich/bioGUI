//
// Created by joppich on 4/14/16.
//

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
            :QGridLayout()
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

        this->addWidget(pAddWidget, iRow, iCol);

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
