//
// Created by mjopp on 15.04.2016.
//

#ifndef BIOGUI_QORDEREDLAYOUT_H
#define BIOGUI_QORDEREDLAYOUT_H

#include <vector>
#include <stddef.h>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QOrderedHBoxLayout : public QHBoxLayout {
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

class QOrderedVBoxLayout : public QVBoxLayout {
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


    virtual void addNextWidget(QWidget* pWidget) = 0;
    virtual void addWidgetAtPosition(QWidget* pWidget, size_t iPosition) = 0;
    virtual void removeWidget(QWidget* pWidget) = 0;
};


#endif //BIOGUI_QORDEREDLAYOUT_H
