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

#include "AdvancedStreamBox.h"


AdvancedCornerWidget::AdvancedCornerWidget(QWidget* pParentBox)
{
    m_oMouseStart = QPoint(-1,-1);
    m_oMouseEnd = QPoint(-1,-1);

    m_pParentBox = pParentBox;

    m_oCurrentSize = m_pParentBox->size();
}

void AdvancedCornerWidget::updateWidgetGeometry() {
    if (m_oMouseStart == QPoint(-1,-1)) {
        return;
    }

    if (m_oMouseStart == m_oMouseEnd)
        return;

    int w = m_oMouseEnd.x() - m_oMouseStart.x();
    int h = m_oMouseEnd.y() - m_oMouseStart.y();

    //m_pParentBox->setMinimumHeight( m_oCurrentSize.height() + h );
    //m_pParentBox->setMinimumWidth( m_oCurrentSize.width() + w );

    QSize oNewMin(m_oCurrentSize.width() + w, m_oCurrentSize.height() + h);

    if ((oNewMin.height() < 0) || (oNewMin.width() < 0))
    {
        qDebug() << m_oMouseStart;
        qDebug() << m_oMouseEnd;
        qDebug() << m_oCurrentSize;
        qDebug() << oNewMin;

        return;
    }

    m_pParentBox->setMinimumSize(oNewMin);
    //m_pParentBox->resize(oNewMin);

    emit sizeChanged();

}

void AdvancedCornerWidget::mousePressEvent(QMouseEvent *pEvent) {
    m_oMouseStart = pEvent->pos();
    m_oMouseEnd = pEvent->pos();

    m_oCurrentSize = m_pParentBox->size();

    updateWidgetGeometry();
    //m_pParentBox->show();
}

void AdvancedCornerWidget::mouseReleaseEvent(QMouseEvent *pEvent) {

    updateWidgetGeometry();

    m_oMouseStart = QPoint(-1,-1);
    m_oMouseEnd = QPoint(-1,-1);

}

void AdvancedCornerWidget::mouseMoveEvent(QMouseEvent *pEvent) {
    m_oMouseEnd = pEvent->pos();
    //updateWidgetGeometry();
}
