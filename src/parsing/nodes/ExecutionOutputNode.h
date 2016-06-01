//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONOUTPUTNODE_H
#define BIOGUI_EXECUTIONOUTPUTNODE_H

#include <QUrl>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QProcess>
#include <src/app/AdvancedStreamBox.h>
#include <QtGui/qdesktopservices.h>
#include <QtCore/qdir.h>
#include <src/app/ExecuteThread.h>
#include <fstream>

#include "ExecutionNode.h"

class ExecutionOutputNode : public ExecutionNode {

public:
    ExecutionOutputNode(QDomElement* pElement)
    : ExecutionNode(pElement)
    {

        m_sType = this->getDomElementAttribute(pElement, "TYPE", "STD").toUpper().toStdString();
        m_sColor = this->getDomElementAttribute(pElement, "COLOR", "black").toStdString();
        m_sTo = this->getDomElementAttribute(pElement, "TO", "").toStdString();
        m_sFrom = this->getDomElementAttribute(pElement, "FROM", "").toStdString();
        m_sLocation = this->getDomElementAttribute(pElement, "location", "").toStdString();

        m_sHost = this->getDomElementAttribute(pElement, "host", "").toStdString();
        m_iPort = std::stoi(this->getDomElementAttribute(pElement, "port", "25").toStdString());

        m_bDefferred = (this->getDomElementAttribute(pElement, "DEFERRED", "true").toUpper().compare("TRUE", Qt::CaseInsensitive) == 0);


        if (m_sTo.size() == 0)
        {
            //throw "no attribute TO given for node " + m_sID;
        }


    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {
        return evaluateDeferred(pID2Node, pInputID2Value, pInputID2Widget, NULL, NULL, false);
    }

    std::string evaluateDeferred( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget,
                          QProcess* pProcess, ExecuteThread* pThread, bool bDeferred)
    {



        /*
         *
         * TYPE FILE(open file/folder)
         */
        if (m_sTo.size() == 0)
        {


            if (m_sType.compare("FILE") == 0)
            {

                if (bDeferred)
                {
                    QDir oLocation = QDir(QString(m_sLocation.c_str()));

                    if (!oLocation.isAbsolute())
                    {
                        oLocation = QDir::currentPath() + oLocation.currentPath();
                    }



                    QDesktopServices::openUrl( QUrl(oLocation.absolutePath()) );
                }
                return "";
            }


            if (m_sType.compare("FOLDER") == 0)
            {

                if (bDeferred)
                {


                    QDir oLocation = QDir(QString(m_sLocation.c_str()));

                    if (!oLocation.isAbsolute())
                    {
                        oLocation = QDir::currentPath() + oLocation.currentPath();
                    }

                    QFileInfo oDirInfo(oLocation.absolutePath());

                    if ( oDirInfo.isFile() )
                    {

                        oLocation = oDirInfo.absoluteDir().absolutePath();

                    }

                    QDesktopServices::openUrl( QUrl(oLocation.absolutePath()) );

                }

                return "";
            }

        }

        /*
         * ANY TYPE WHERE 'TO' IS NEEDED
         *
         */

        QWidget* pWidget = NULL;
        std::map<std::string, QWidget*>::iterator oIt = pInputID2Widget->find( m_sTo );
        if ( oIt != pInputID2Widget->end() )
        {
            pWidget = oIt->second;

        } else {
            throw "invalid TO id given for node id " + m_sID;
        }

        /*
         *
         * TYPE cout/cerr/STD/file
         *
         *
         */
        if (AdvancedStreamBox* pTextEdit = dynamic_cast<AdvancedStreamBox*>( pWidget ))
        {

            if ( bDeferred == false)
            {

                QColor oColor = QColor(QString(m_sColor.c_str()));


                if (pProcess != NULL)
                {
                    if (m_sType.compare("STD") == 0)
                    {
                        pTextEdit->addProcessBuffer( pProcess, QProcess::StandardOutput, QString(m_sTo.c_str()), oColor );
                        pTextEdit->addProcessBuffer( pProcess, QProcess::StandardError, QString(m_sTo.c_str()), oColor );
                    }

                    if (m_sType.compare("COUT") == 0)
                    {
                        pTextEdit->addProcessBuffer( pProcess, QProcess::StandardOutput, QString(m_sTo.c_str()), oColor );
                    }

                    if (m_sType.compare("CERR") == 0)
                    {
                        pTextEdit->addProcessBuffer( pProcess, QProcess::StandardError, QString(m_sTo.c_str()), oColor );
                    }
                }

                if (pThread != NULL)
                {

                    if (m_sType.compare("STD") == 0)
                    {
                        pTextEdit->addThreadBuffer( pThread, QProcess::StandardOutput, QString(m_sTo.c_str()), oColor );
                        pTextEdit->addThreadBuffer( pThread, QProcess::StandardError, QString(m_sTo.c_str()), oColor );
                    }

                    if (m_sType.compare("COUT") == 0)
                    {
                        pTextEdit->addThreadBuffer( pThread, QProcess::StandardOutput, QString(m_sTo.c_str()), oColor );
                    }

                    if (m_sType.compare("CERR") == 0)
                    {
                        pTextEdit->addThreadBuffer( pThread, QProcess::StandardError, QString(m_sTo.c_str()), oColor );
                    }

                }


                if (m_sType.compare("TCP") == 0)
                {
                    pTextEdit->addTCPBuffer( pThread, m_sHost, m_iPort, QString(m_sTo.c_str()), oColor );
                }

            } else {

                pTextEdit->finishProcess(pProcess);
                pTextEdit->finishThread(pThread);

                if (m_sType.compare("FILE") == 0)
                {

                    std::string sSaveTo = this->getNodeValueOrValue(m_sFrom, "", pID2Node, pInputID2Value, pInputID2Widget);

                    if (sSaveTo.size() != 0)
                    {
                        std::ofstream oOutputStream( m_sFrom );

                        oOutputStream << pTextEdit->getStreamContent( &m_sTo );

                        oOutputStream.close();
                    }

                }

            }

            return "";

        }


        /*
         * TYPE FILE(image)
         *
         */

        if (QGraphicsView* pImageView = dynamic_cast<QGraphicsView*>( pWidget ))
        {

            if (bDeferred)
            {
                pImageView->scene()->clear();

                QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(QPixmap( QString(m_sLocation.c_str()) ));

                pImageView->scene()->addItem( pItem );
            }


            return "";
        }


        return "";

    }



protected:

    std::string m_sType;
    std::string m_sTo;
    std::string m_sFrom;
    std::string m_sColor;
    std::string m_sLocation;

    // for TCP connections
    std::string m_sHost;
    int m_iPort;

    bool m_bDefferred;




};


#endif //BIOGUI_EXECUTIONOUTPUTNODE_H
