//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONOUTPUTNODE_H
#define BIOGUI_EXECUTIONOUTPUTNODE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QProcess>
#include <src/app/AdvancedStreamBox.h>

#include "ExecutionNode.h"
#include "../../app/StreamTextEdit.h"

class ExecutionOutputNode : public ExecutionNode {

public:
    ExecutionOutputNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {

        m_sType = this->getDomElementAttribute(pElement, "TYPE", "STD").toUpper().toStdString();
        m_sColor = this->getDomElementAttribute(pElement, "COLOR", "black").toStdString();
        m_sTo = this->getDomElementAttribute(pElement, "TO", "").toStdString();
        m_sLocation = this->getDomElementAttribute(pElement, "location", "").toStdString();

        m_bDefferred = (this->getDomElementAttribute(pElement, "DEFERRED", "true").toUpper().compare("TRUE", Qt::CaseInsensitive) == 0);


        if (m_sTo.size() == 0)
        {
            throw "no attribute TO given for node " + m_sID;
        }


    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {
        return evaluateDeferred(pID2Node, pInputID2Value, pInputID2Widget, NULL, false);
    }

    std::string evaluateDeferred( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget,
                          QProcess* pProcess, bool bDeferred)
    {

        if (!(pInputID2Widget->find( m_sTo ) != pInputID2Widget->end()))
        {
            throw "invalid TO id given for node id " + m_sID;
        }

        QWidget* pWidget = pInputID2Widget->find( m_sTo )->second;

        if (AdvancedStreamBox* pTextEdit = dynamic_cast<AdvancedStreamBox*>( pWidget ))
        {

            QColor oColor = QColor(QString(m_sColor.c_str()));

            if (m_sType.compare("STD") == 0)
            {
                pTextEdit->addBuffer( pProcess, QProcess::StandardOutput, QString(m_sTo.c_str()), oColor );
                pTextEdit->addBuffer( pProcess, QProcess::StandardError, QString(m_sTo.c_str()), oColor );
            }

            if (m_sType.compare("COUT") == 0)
            {
                pTextEdit->addBuffer( pProcess, QProcess::StandardOutput, QString(m_sTo.c_str()), oColor );
            }

            if (m_sType.compare("CERR") == 0)
            {
                pTextEdit->addBuffer( pProcess, QProcess::StandardError, QString(m_sTo.c_str()), oColor );
            }

        }

        if (QGraphicsView* pImageView = dynamic_cast<QGraphicsView*>( pWidget ))
        {

            pImageView->scene()->clear();

            QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(QPixmap( QString(m_sLocation.c_str()) ));

            pImageView->scene()->addItem( pItem );

        }

        return "";

    }

protected:

    std::string m_sType;
    std::string m_sTo;
    std::string m_sColor;
    std::string m_sLocation;

    bool m_bDefferred;


};


#endif //BIOGUI_EXECUTIONOUTPUTNODE_H
