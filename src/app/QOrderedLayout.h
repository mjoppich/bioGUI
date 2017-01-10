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

#ifndef BIOGUI_QORDEREDLAYOUT_H
#define BIOGUI_QORDEREDLAYOUT_H

#include <vector>
#include <stddef.h>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QOrderedLayout {

public:

    QOrderedLayout()
    {

    }

    void updateWidgets(std::vector<QWidget*>* pWidgets)
    {

        for (size_t i = 0; i < pWidgets->size(); ++i)
        {
            pWidgets->at(i)->setParent(0);
            this->removeWidget( pWidgets->at(i) );
        }



        for (size_t i = 0; i < pWidgets->size(); ++i)
        {

            this->addWidgetAtPosition(pWidgets->at(i), i);


        }

    }

    virtual ~QOrderedLayout()
    {

    }


    virtual void addNextWidget(QWidget* pWidget) = 0;
    virtual void addWidgetAtPosition(QWidget* pWidget, size_t iPosition) = 0;
    virtual void removeWidget(QWidget* pWidget) = 0;
};

class QOrderedHBoxLayout : public QHBoxLayout, public QOrderedLayout {
public:
    void addNextWidget(QWidget* pWidget)
    {
        QWidget* pAddWidget = pWidget;

        this->addWidget(pWidget);

    }

    void removeWidget(QWidget* pWidget)
    {
        QHBoxLayout::removeWidget(pWidget);
    }

protected:

    void addWidgetAtPosition(QWidget* pWidget, size_t iPosition)
    {
        pWidget->setParent(this->parentWidget());
        this->insertWidget(iPosition, pWidget);
    }
};

class QOrderedVBoxLayout : public QVBoxLayout, public QOrderedLayout {
public:
    void addNextWidget(QWidget* pWidget)
    {
        QWidget* pAddWidget = pWidget;

        this->addWidget(pWidget);

    }

    void removeWidget(QWidget* pWidget)
    {
        QVBoxLayout::removeWidget(pWidget);
    }

protected:

    void addWidgetAtPosition(QWidget* pWidget, size_t iPosition)
    {
        pWidget->setParent(this->parentWidget());
        this->insertWidget(iPosition, pWidget);
    }
};




#endif //BIOGUI_QORDEREDLAYOUT_H
