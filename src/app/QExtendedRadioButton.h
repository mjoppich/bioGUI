//
// Created by joppich on 4/7/16.
//

#ifndef BIOGUI_QEXTENDEDRADIOBUTTON_H
#define BIOGUI_QEXTENDEDRADIOBUTTON_H

#include <QRadioButton>
#include "QAbstractButtonItem.h"

class QExtendedRadioButton : public QRadioButton, public QAbstractButtonItem
{

public:

    QExtendedRadioButton(QString sLabel, QString sValue)
    : QRadioButton(sLabel), QAbstractButtonItem(sValue)
    {

    }


};

#endif //BIOGUI_QEXTENDEDRADIOBUTTON_H
