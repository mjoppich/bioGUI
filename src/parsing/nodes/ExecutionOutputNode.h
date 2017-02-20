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
#include "ExecutionDeferredNode.h"

class ExecutionOutputNode : public ExecutionDeferredNode {

public:
    ExecutionOutputNode(QDomElement* pElement)
    : ExecutionDeferredNode(pElement)
    {

        m_sType = this->getDomElementAttribute(pElement, "TYPE", "STD").toUpper().toStdString();
        m_sColor = this->getDomElementAttribute(pElement, "COLOR", "black").toStdString();
        m_sTo = this->getDomElementAttribute(pElement, "TO", "").toStdString();
        m_sFrom = this->getDomElementAttribute(pElement, "FROM", "").toStdString();
        m_sLocation = this->getDomElementAttribute(pElement, "location", "").toStdString();

        m_sHost = this->getDomElementAttribute(pElement, "host", "").toStdString();
        m_iPort = std::stoi(this->getDomElementAttribute(pElement, "port", "25").toStdString());



        if (m_sTo.size() == 0)
        {
            //throw "no attribute TO given for node " + m_sID;
        }


    }

    virtual ~ExecutionOutputNode()
    {

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {
        return evaluateDeferred(pID2Node, pInputID2Value, pInputID2FunctionWidget, NULL, NULL, false);
    }

    std::string evaluateDeferred( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget,
                          QProcess* pProcess, ExecuteThread* pThread, bool bDeferred)
    {



        /*
         *
         * TYPE FILE(open file/folder)
         */
        if (m_sTo.size() == 0)
        {

            std::string sOpenLocation = this->getNodeValueOrValue(m_sLocation, m_sLocation, pID2Node, pInputID2Value, pInputID2FunctionWidget);


            if (m_sType.compare("FILE") == 0)
            {

                if (bDeferred)
                {
                    QDir oLocation = QDir(QString(sOpenLocation.c_str()));

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


                    QDir oLocation = QDir(QString(sOpenLocation.c_str()));

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
        std::map<std::string, WidgetFunctionNode*>::iterator oIt = pInputID2FunctionWidget->find( m_sTo );
        if ( oIt != pInputID2FunctionWidget->end() )
        {
            pWidget = oIt->second->getWidget();

        } else {
            LOGERROR("invalid TO id given for node id " + m_sID);
            return "";
        }

        /*
         *
         * TYPE cout/cerr/STD/file
         *
         *
         */
        AdvancedStreamBox* pTextEdit = dynamic_cast<AdvancedStreamBox*>( pWidget );

        if ( pTextEdit )
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

                    std::string sSaveTo = this->getNodeValueOrValue(m_sFrom, "", pID2Node, pInputID2Value, pInputID2FunctionWidget);

                    if (sSaveTo.size() != 0)
                    {

                        QFileInfo oInfo(QString(sSaveTo.c_str()));

                        std::cerr << "Writing output to: " << oInfo.absoluteFilePath().toStdString() << std::endl;

                        std::ofstream oOutputStream( oInfo.absoluteFilePath().toStdString(), std::ios_base::trunc );

                        oOutputStream << pTextEdit->getStreamContent( &m_sTo );

                        oOutputStream.flush();
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

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("TYPE");
        vAttributes.push_back("COLOR");
        vAttributes.push_back("TO");
        vAttributes.push_back("FROM");
        vAttributes.push_back("LOCATION");
        vAttributes.push_back("HOST");
        vAttributes.push_back("PORT");
        vAttributes.push_back("DEFERRED");

    }

    std::string m_sType;
    std::string m_sTo;
    std::string m_sFrom;
    std::string m_sColor;
    std::string m_sLocation;

    // for TCP connections
    std::string m_sHost;
    int m_iPort;





};


#endif //BIOGUI_EXECUTIONOUTPUTNODE_H
