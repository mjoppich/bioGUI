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
        if ( QAbstractButton* pButton = dynamic_cast<QAbstractButton*> (pChild))
        {

            if (pButton->isCheckable())
            {

                this->connect(pButton, &QAbstractButton::toggled, [pButton, this](){

                    this->elementChangedAButton(pButton);

                });

                if (m_vChildren.size() == 0)
                {
                    pButton->setChecked(true);
                } else {
                    pButton->setChecked(false);
                }

            }

            m_vChildren.push_back(pChild);
        }


        if ( QExclusiveGroupBox* pBox = dynamic_cast<QExclusiveGroupBox*> (pChild))
        {

            if (pBox->isCheckable())
            {

                this->connect(pBox, &QExclusiveGroupBox::toggled, [pBox, this](){

                    this->elementChangedBox(pBox);

                });

                if (m_vChildren.size() == 0)
                {
                    pBox->setChecked(true);
                } else {
                    pBox->setChecked(false);
                }
            }

            m_vChildren.push_back(pChild);
        }



    }

    void elementChangedBox(QExclusiveGroupBox* pElement)
    {

        if (this->m_bExclusive == false)
            return;

        if (pElement->isChecked() == false)
            return;

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            QWidget* pChild = m_vChildren.at(i);

            if (pChild == pElement)
                continue;

            if ( QExclusiveGroupBox* pBox = dynamic_cast<QExclusiveGroupBox*>( pChild ))
            {

                if (pBox->isCheckable())
                    pBox->setChecked(false);

            }


        }


    }

    void elementChangedAButton(QAbstractButton* pElement)
    {

        if (this->m_bExclusive == false)
            return;

        if (pElement->isChecked() == false)
            return;

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            QWidget* pChild = m_vChildren.at(i);

            if (pChild == pElement)
                continue;

            if ( QAbstractButton* pButton = dynamic_cast<QAbstractButton*>( pChild ))
            {

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
