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

#include "QExclusiveGroupBox.h"


void QExclusiveGroupBox::elementChangedAButton(QAbstractButton* pElement)
{

    if (this->m_bExclusive == false)
        return;

    if (pElement->isChecked() == false)
    {
        this->getConsistent();
        return;
    }


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

void QExclusiveGroupBox::elementChangedBox(QExclusiveGroupBox* pElement)
{

    if (this->m_bExclusive == false)
        return;

    if (pElement->isChecked() == false)
    {
        this->getConsistent();
        return;
    }

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

void QExclusiveGroupBox::addChild(QWidget* pChild)
{

    // try to cast to (checkbox/radiobutton)qabstractbutton/exclusivegroupbox
    if ( QAbstractButton* pButton = dynamic_cast<QAbstractButton*> (pChild))
    {

        if (pButton->isCheckable())
        {

            this->connect(pButton, &QAbstractButton::toggled, [pButton, this](){

                this->elementChangedAButton(pButton);

            });

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

            /*
            if (m_vChildren.size() == 0)
            {
                pBox->setChecked(true);
            } else {
                pBox->setChecked(false);
            }
             */
        }

        m_vChildren.push_back(pChild);
    }



}