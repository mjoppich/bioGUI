//
// Created by joppich on 4/7/16.
//

#ifndef BIOGUI_QABSTRACTBUTTONITEM_H
#define BIOGUI_QABSTRACTBUTTONITEM_H

#include <QLabel>
#include <QAbstractButton>

class QAbstractButtonItem {
public:

    QAbstractButtonItem(QString sValue)
    {
        m_sValue = sValue;
    }


    void setValue(QString sString)
    {
        m_sValue = sString;
    }

    QString getValue()
    {
        return m_sValue;
    }

protected:

    QString m_sValue;

};

#endif //BIOGUI_QABSTRACTBUTTONITEM_H
