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
#include <src/app/ExtendedProcFileBuffer.h>
#include <src/app/ExtendedThreadFileBuffer.h>
#include <QObject>
#include <fstream>

#include "ExecutionNode.h"
#include "ExecutionDeferredNode.h"

class ExecutionOutputNode : public  ExecutionDeferredNode {
Q_OBJECT
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

    std::vector<std::string> inputs()
    {
        std::vector<std::string> vBase;
        vBase.push_back(this->getID());

        for (int i = 0; i < m_vChildren.size(); ++i)
        {

            ExecutionNode* pChild = m_vChildren.at(i);

            std::vector<std::string> vChildOuts = pChild->outputs();

            vBase.insert(vBase.end(), vChildOuts.begin(), vChildOuts.end());

        }

        return vBase;
    }

    virtual ~ExecutionOutputNode()
    {


        LOGERROR("DESTRUCTOR EXECUTION OUTPUT NODE")

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {
        return evaluateDeferred(pID2Node, pInputID2Value, pInputID2FunctionWidget, NULL, NULL, false);
    }


    ExtendedProcFileBuffer* createProcFileBuffer(QProcess* pProcess, QProcess::ProcessChannel eChannel)
    {

        ExtendedProcFileBuffer* pBuffer = new ExtendedProcFileBuffer(pProcess, eChannel);



        QObject::connect(pBuffer, SIGNAL(sendText(QString)), this , SLOT(receiveText(QString)), Qt::QueuedConnection );

        return pBuffer;
    }

    ExtendedThreadFileBuffer* createThreadFileBuffer(ExecuteThread* pThread, QProcess::ProcessChannel eChannel)
    {

        ExtendedThreadFileBuffer* pBuffer = new ExtendedThreadFileBuffer(pThread, eChannel);



        QObject::connect(pBuffer, SIGNAL(sendText(QString)), this , SLOT(receiveText(QString)), Qt::QueuedConnection );

        return pBuffer;
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

                    if (pTextEdit == NULL)
                    {
                        LOGERROR("invalid TO id given for node id " + m_sID);
                        return "";
                    }

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
                    std::cerr << "Adding TCP Buffer for thread " << pThread << " on port " << m_iPort << std::endl;
                    pTextEdit->addTCPBuffer( pThread, m_sHost, m_iPort, QString(m_sTo.c_str()), oColor );
                }

            } else {

                LOGERROR("Deferred Output Node");



                this->finishProcessThread(pProcess, pThread, pTextEdit);


                if (m_sType.compare("TCP") == 0)
                {
                    pTextEdit->finishThread(0);
                }

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


        if (bDeferred == false)
        {
            if (pProcess != NULL)
            {
                if (m_sType.compare("FILE_STD") == 0)
                {

                    std::string sLocation = this->getNodeValueOrValue(m_sLocation, m_sLocation, pID2Node, pInputID2Value, pInputID2FunctionWidget);
                    LOGERROR("Opening File")
                    LOGERROR(sLocation)
                    m_pOutputStream = new std::ofstream(sLocation);

                    ExtendedProcFileBuffer* pBuffer = this->createProcFileBuffer(pProcess, QProcess::StandardOutput);
                    this->addProcBuffer(pProcess, pBuffer);
                }
            } else if (pThread != NULL)
            {
                if (m_sType.compare("FILE_STD") == 0)
                {

                    std::string sLocation = this->getNodeValueOrValue(m_sLocation, m_sLocation, pID2Node, pInputID2Value, pInputID2FunctionWidget);
                    LOGERROR("Opening File")
                    LOGERROR(sLocation)
                    m_pOutputStream = new std::ofstream(sLocation);

                    ExtendedThreadFileBuffer* pBuffer = this->createThreadFileBuffer(pThread, QProcess::StandardOutput);
                    this->addThreadBuffer(pThread, pBuffer);
                }

            }
        } else {

            if (m_sType.compare("FILE_STD") == 0)
            {
                this->finishProcessThread(pProcess, pThread, pTextEdit);

                if (m_pOutputStream)
                {
                    m_pOutputStream->flush();
                    m_pOutputStream->close();

                    m_pOutputStream = NULL;

                }
            }



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

    void addProcBuffer(QProcess* pProcess, ExtendedProcFileBuffer* pBuffer)
    {

        std::map<QProcess*, std::vector<ExtendedProcFileBuffer*> >::iterator oIt = m_mProcToBuffer.find(pProcess);

        if (oIt != m_mProcToBuffer.end())
        {
            oIt->second.push_back(pBuffer);
        } else {
            std::pair<QProcess*, std::vector<ExtendedProcFileBuffer*>> oPair(pProcess, std::vector<ExtendedProcFileBuffer*>());

            oPair.second.push_back(pBuffer);
            m_mProcToBuffer.insert( oPair );

        }


    }

    void addThreadBuffer(ExecuteThread* pThread, ExtendedThreadFileBuffer* pBuffer)
    {

        std::map<ExecuteThread*, std::vector<ExtendedThreadFileBuffer*> >::iterator oIt = m_mThreadToBuffer.find(pThread);

        if (oIt != m_mThreadToBuffer.end())
        {
            oIt->second.push_back(pBuffer);
        } else {
            std::pair<ExecuteThread*, std::vector<ExtendedThreadFileBuffer*>> oPair(pThread, std::vector<ExtendedThreadFileBuffer*>());

            oPair.second.push_back(pBuffer);
            m_mThreadToBuffer.insert( oPair );

        }


    }

    void finishProcess(QProcess* pProcess)
    {
        std::map<QProcess*, std::vector<ExtendedProcFileBuffer*> >::iterator oIt = m_mProcToBuffer.find(pProcess);

        if (oIt != m_mProcToBuffer.end())
        {

            for (size_t i = 0; i < oIt->second.size(); ++i)
            {

                ExtendedProcFileBuffer* pBuffer = oIt->second.at(i);

                LOGERROR("Deleting PROC Buffer: " + std::to_string((uint64_t) pBuffer));

                pBuffer->transferText("\n");
                pBuffer->deleteLater();

            }

            m_mProcToBuffer.erase(oIt);


        }
    }

    void finishThread( ExecuteThread* pThread )
    {

        LOGERROR("Finishing thread: " + std::to_string((uint64_t) pThread));

        /*
        if (pThread == NULL)
            return;
        */

        std::map<ExecuteThread*, std::vector<ExtendedThreadFileBuffer*> >::iterator oJt = m_mThreadToBuffer.find(pThread);

        if (oJt != m_mThreadToBuffer.end())
        {

            for (size_t i = 0; i < oJt->second.size(); ++i)
            {

                ExtendedThreadFileBuffer* pBuffer = oJt->second.at(i);
                pBuffer->transferText("\n");
                pBuffer->stopTransmissions();
                pBuffer->deleteLater();

            }

            m_mThreadToBuffer.erase(oJt);

        }

    }


    void finishProcessThread(QProcess* pProcess, ExecuteThread* pThread, AdvancedStreamBox* pTextEdit)
    {
        if (pTextEdit != NULL)
        {
            if (pProcess != NULL)
            {
                pTextEdit->finishProcess(pProcess);
            }

            if (pThread != NULL)
            {
                pTextEdit->finishThread(pThread);
            }
        }



        if (pProcess != NULL)
        {
            std::map<QProcess*, std::vector<ExtendedProcFileBuffer*> >::iterator oIt = m_mProcToBuffer.find(pProcess);

            if (oIt != m_mProcToBuffer.end())
            {

                for (size_t i = 0; i < oIt->second.size(); ++i)
                {

                    ExtendedProcFileBuffer* pBuffer = oIt->second.at(i);

                    LOGERROR("Deleting PROC Buffer: " + std::to_string((uint64_t) pBuffer));

                    pBuffer->transferText("\n");
                    pBuffer->deleteLater();

                }

                m_mProcToBuffer.erase(oIt);


            }
        }

        if (pThread != NULL)
        {
            std::map<ExecuteThread*, std::vector<ExtendedThreadFileBuffer*> >::iterator oJt = m_mThreadToBuffer.find(pThread);

            if (oJt != m_mThreadToBuffer.end())
            {

                for (size_t i = 0; i < oJt->second.size(); ++i)
                {

                    ExtendedThreadFileBuffer* pBuffer = oJt->second.at(i);
                    pBuffer->transferText("\n");
                    pBuffer->stopTransmissions();
                    pBuffer->deleteLater();

                }

                m_mThreadToBuffer.erase(oJt);

            }
        }

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

    std::map<QProcess*, std::vector<ExtendedProcFileBuffer*>> m_mProcToBuffer;
    std::map<ExecuteThread*, std::vector<ExtendedThreadFileBuffer*>> m_mThreadToBuffer;

    std::ofstream* m_pOutputStream=NULL;

protected slots:

    void receiveText(QString sString)
    {

        //std::cerr << sString.toStdString();

        if (m_pOutputStream)
        {
            (*m_pOutputStream) << sString.toStdString();
            m_pOutputStream->flush();
        } else {
            std::cerr << "attempt to print after close";
        }

    }


};


#endif //BIOGUI_EXECUTIONOUTPUTNODE_H
