//
// Created by joppich on 4/13/16.
//

#ifndef BIOGUI_QEXCLUSIVEGROUPBOX_H
#define BIOGUI_QEXCLUSIVEGROUPBOX_H

#include <QGroupBox>
#include <QAbstractButton>

class QExclusiveGroupBox : public QGroupBox {

    Q_OBJECT

public:

    QExclusiveGroupBox(QString sTitle, QWidget* pParent = 0)
    :QGroupBox(sTitle, pParent)
    {


    }

    void setExclusive(bool bValue)
    {
        m_bExclusive = bValue;
    }

    bool getExclusive()
    {
        return m_bExclusive;
    }

    void addChild(QWidget* pChild)
    {

        // try to cast to (checkbox/radiobutton)qabstractbutton/exclusivegroupbox

        // if is checkable

        // add connection -> checked -> this


        m_vChildren.push_back(pChild);
    }

    void elementChanged(QAbstractButton* pElement)
    {

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            QWidget* pChild = m_vChildren.at(i);

            if (pChild == pElement)
                continue;

            if ( dynamic_cast<QAbstractButton*>( pChild ) != NULL)
            {

                QAbstractButton* pButton = dynamic_cast<QAbstractButton*>( pChild );

                if (pButton->isCheckable())
                    pButton->setChecked(false);
                
            }


        }



    }

protected:

    bool m_bExclusive = false;

    std::vector<QWidget*> m_vChildren;

};


#endif //BIOGUI_QEXCLUSIVEGROUPBOX_H
