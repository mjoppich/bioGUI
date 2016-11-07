//
// Created by joppich on 4/5/16.
//

#include "AdvancedStreamBox.h"


AdvancedCornerWidget::AdvancedCornerWidget(AdvancedStreamBox* pParentBox)
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

    int w = m_oMouseEnd.x() - m_oMouseStart.x();
    int h = m_oMouseEnd.y() - m_oMouseStart.y();

    m_pParentBox->setMinimumHeight( m_oCurrentSize.height() + h );
    m_pParentBox->setMinimumWidth( m_oCurrentSize.width() + w );

    QSize oNewMin(m_oCurrentSize.width() + w, m_oCurrentSize.height() + h);

    m_pParentBox->setMinimumSize(oNewMin);
    m_pParentBox->resize(oNewMin);

    emit sizeChanged();

}

void AdvancedCornerWidget::mousePressEvent(QMouseEvent *pEvent) {
    m_oMouseStart = pEvent->pos();
    m_oMouseEnd = pEvent->pos();

    m_oCurrentSize = m_pParentBox->size();

    updateWidgetGeometry();
    m_pParentBox->show();
}

void AdvancedCornerWidget::mouseReleaseEvent(QMouseEvent *pEvent) {

    m_oMouseStart = QPoint(-1,-1);
    m_oMouseEnd = QPoint(-1,-1);

}
