//
// Created by joppich on 4/7/16.
//

#ifndef BIOGUI_QEXTENDEDCHECKBOX_H
#define BIOGUI_QEXTENDEDCHECKBOX_H

#include <QCheckBox>
#include "QAbstractButtonItem.h"

class QExtendedCheckBox : public QCheckBox, public QAbstractButtonItem
{

public:

    QExtendedCheckBox(QString sLabel, QString sValue)
            : QCheckBox(sLabel), QAbstractButtonItem(sValue)
    {

    }


};

#endif //BIOGUI_QEXTENDEDCHECKBOX_H
