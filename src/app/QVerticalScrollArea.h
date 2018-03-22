//
// Created by joppich on 3/22/18.
//

#ifndef BIOGUI_QVERTICALSCROLLAREA_H
#define BIOGUI_QVERTICALSCROLLAREA_H

#include <QtWidgets/QScrollArea>
#include <iostream>

class QVerticalScrollArea : public QScrollArea {
    Q_OBJECT
public:

    QVerticalScrollArea()
            : QScrollArea()
    {


    }

    void resizeEvent(QResizeEvent* event)
    {
        QScrollArea::resizeEvent(event);

        QWidget* pContent = this->widget();

        if (pContent != NULL)
        {
            QSize newSize = event->size();
            int newWidth = newSize.width();
            int contentMinWidth = pContent->sizeHint().width();

            // use max of both widths
            int minWidth = newWidth > contentMinWidth ? newWidth : contentMinWidth;

            //std::cerr << newWidth << " " << contentMinWidth << " " << minWidth << std::endl;

            pContent->setMinimumWidth(minWidth);
            pContent->update();

        }
    }


protected:


};
#endif //BIOGUI_QVERTICALSCROLLAREA_H
