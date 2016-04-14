//
// Created by joppich on 4/14/16.
//

#ifndef BIOGUI_QEXTGRIDLAYOUT_H
#define BIOGUI_QEXTGRIDLAYOUT_H

#include <QGridLayout>
#include <QLabel>
#include <QtWidgets/qcombobox.h>
#include <iterator>

class QExtGridLayout : public QGridLayout {

    Q_OBJECT
public:

    QExtGridLayout(int iRows, int iCols, bool bOrdering = false)
            :QGridLayout(), m_bOrdering(bOrdering)
    {


        m_iRows = iRows;
        m_iCols = iCols;


    }

    void addNextWidget(QWidget* pWidget)
    {
        QWidget* pAddWidget = pWidget;

        if (m_bOrdering)
        {

            pAddWidget = new QWidget();

            QGridLayout* pLayout = new QGridLayout();
            pLayout->addWidget(new QLabel("Position"), 0,0);
            QComboBox* pOrdering = new QComboBox();
            pLayout->addWidget(pOrdering, 0, 1);
            pLayout->addWidget(pWidget, 1, 0, 1, 2);

            pAddWidget->setLayout(pLayout);

            m_mWidgetToSelBox.insert(std::pair<QWidget*, QComboBox*>(pAddWidget, pOrdering));

            //connect(pOrdering,static_cast< void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [] (int) {});

            QObject::connect(pOrdering,static_cast< void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [pAddWidget, this] (int iNewPos) {


                this->switchOrdering(pAddWidget, iNewPos);

            });
        }

        int iCol = this->currentElements() % m_iCols;
        int iRow = this->currentElements() / m_iCols;

        this->addWidget(pAddWidget, iRow, iCol);

        m_vWidgets.push_back(pAddWidget);

        if (m_bOrdering)
        {

            m_bSwitchOrdering = false;
            this->updateOrderBoxes();
            m_bSwitchOrdering = true;
        }

    }


protected:

    void switchOrdering(QWidget* pWidget, int iNewPos)
    {

        if (!m_bSwitchOrdering)
            return;

        std::vector<QWidget*>::iterator oIt = std::find(m_vWidgets.begin(), m_vWidgets.end(), pWidget);

        QWidget* pPutBefore = NULL;
        pPutBefore = m_vWidgets.at(iNewPos);

        if (oIt != m_vWidgets.end())
        {

            m_vWidgets.erase(oIt);

            if (pPutBefore != NULL)
            {

                oIt = std::find(m_vWidgets.begin(), m_vWidgets.end(), pPutBefore);

                int iDist = std::distance(m_vWidgets.begin(), oIt);

                if (iDist < iNewPos)
                    ++oIt;


                m_vWidgets.insert(oIt, pWidget);

            } else {

                m_vWidgets.push_back(pWidget);

            }



        }

        m_bSwitchOrdering = false;
        this->updateOrderBoxes();
        this->updateWidgets();
        m_bSwitchOrdering = true;
    }

    void updateWidgets()
    {

        for (size_t i = 0; i < m_vWidgets.size(); ++i)
        {
            m_vWidgets.at(i)->setParent(0);
            this->removeWidget( m_vWidgets.at(i) );
        }



        for (size_t i = 0; i < m_vWidgets.size(); ++i)
        {

            int iCol = i % m_iCols;
            int iRow = i / m_iCols;

            m_vWidgets.at(i)->setParent(this->parentWidget());
            this->addWidget(m_vWidgets.at(i), iRow, iCol);



        }

    }

    void updateOrderBoxes()
    {

        std::map<QWidget*, QComboBox*>::iterator oIt = m_mWidgetToSelBox.begin();

        while (oIt != m_mWidgetToSelBox.end())
        {

            QComboBox* pBox = oIt->second;

            pBox->clear();

            for (size_t i = 0; i < m_vWidgets.size(); ++i)
            {

                QString sItemNum = QString(std::to_string(i+1).c_str());

                pBox->addItem( sItemNum, (int)i );

                if (m_vWidgets.at(i) == oIt->first)
                {
                    pBox->setCurrentIndex(i);
                }

            }

            ++oIt;

        }



    }

    int currentElements()
    {
        return m_vWidgets.size();
    }

    std::map<QWidget*, QComboBox*> m_mWidgetToSelBox;
    std::vector<QWidget*> m_vWidgets;
    bool m_bOrdering = false;
    bool m_bSwitchOrdering = true;

    int m_iCols = 0;
    int m_iRows = 0;
};


#endif //BIOGUI_QEXTGRIDLAYOUT_H
