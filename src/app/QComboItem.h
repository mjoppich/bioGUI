//
// Created by joppich on 4/5/16.
//

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
