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

#ifndef BIOGUI_BIOGUIAPP_H
#define BIOGUI_BIOGUIAPP_H

#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>
#include <QXmlQuery>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QXmlResultItems>
#include <QWidget>
#include <QDesktopWidget>
#include <QSplitter>
#include <QListWidget>

#include <iostream>
#include <src/app/TemplateListDelegate.h>
#include <src/parsing/XMLParserInfo.h>
#include <src/app/QDownloadTemplatesWindow.h>
#include <src/parsing/XMLParserExecution.h>
#include <src/parsing/XMLParserWindow.h>
#include <src/app/ExecutionRunThread.h>
#include <src/app/QVerticalScrollArea.h>

class FramelessWindow;

class bioGUIapp : public QApplication {
    Q_OBJECT
public:

    bioGUIapp(int& argc, char** argv);

    XMLParserWindow* getWindowParser()
    {
        return m_pWindowParser;
    }

    void setFLWindow(FramelessWindow* pWin)
    {
        this->m_pFramelessWindow = pWin;
    }

    QString getAppPath()
    {
        return m_oTemplatePath.path();
    }

    void reloadTemplates()
    {
        QDir oTemplatePath = this->getAppPath() + "/templates/";

        this->addTemplates( oTemplatePath );
    }

    QMainWindow* getMainWindow()
    {
        return m_pMainMainWindow;
    }

    QWidget* getTemplateWindow()
    {
        return m_pMainWindow;
    }

    void reloadAppWindow()
    {

        m_pWindow->setMinimumSize(m_pWindow->sizeHint());
        m_pApplicationWindowArea->update();
    }

    void loadInitFile(QDir oPath)
    {
        QString sConfigFilePath = oPath.absolutePath() + "/config.ini";

        QSettings* pSettings = new QSettings(sConfigFilePath, QSettings::IniFormat);

        QStringList allKeys = pSettings->allKeys();
        m_sDownloadServerLocation = pSettings->value("server/location", "localhost").toString();
        m_sWSLBashLocation = pSettings->value("bash/location", "C:\\Windows\\sysnative\\bash").toString();

        delete pSettings;

    }


    void enableActions()
    {
        this->setActionsState(true);
    }

    void disableActions()
    {
        this->setActionsState(false);
    }

    void downloadTemplatesDialogue()
    {
        this->disableActions();
        this->m_pMainMainWindow->setEnabled(false);

        QDownloadTemplatesWindow* pNewWindow = new QDownloadTemplatesWindow(m_oTemplatePath.path(), m_sDownloadServerLocation);
        pNewWindow->show();

        connect(pNewWindow, &QDownloadTemplatesWindow::closed, [this, pNewWindow] () {

            pNewWindow->deleteLater();
            std::cerr << "received closed signal" << std::endl;
            this->enableActions();
            this->m_pMainMainWindow->setEnabled(true);

            this->reloadTemplates();

        });

    }

    void addTemplates(QDir oDirectory)
    {

        while( m_pTemplateListWidget->count() > 0)
        {
            QListWidgetItem* pItem = m_pTemplateListWidget->takeItem(0);
            delete pItem;
        }

        m_pTemplateListWidget->clear();
        m_pTemplateListWidget->setItemDelegate(new TemplateListDelegate(m_pTemplateListWidget));

        std::cout << "adding templates from " << oDirectory.path().toStdString() << std::endl;

        QStringList vFilters;
        vFilters.append( "*.gui" );
        QFileInfoList vTemplateFiles = oDirectory.entryInfoList(vFilters, QDir::Files);

        for (size_t i = 0; i < vTemplateFiles.size(); ++i)
        {
            QFileInfo oFile = vTemplateFiles.at(i);

            if (!oFile.isFile())
                continue;

            std::cout << oFile.baseName().toStdString() << std::endl;

            QListWidgetItem *pItem = new QListWidgetItem();

            QString sDocumentPath = oFile.absoluteFilePath();


            XMLParserInfo oInfoParser(sDocumentPath.toStdString());

            if (oInfoParser.getDocument() == NULL)
            {

                delete pItem;
                continue;
            }

            QString sIconPath = oInfoParser.getIcon();

            if ((sIconPath.length() > 0))
            {
                //QIcon oIcon(sIconPath);
                QIcon oIcon = QIcon(sIconPath);
                pItem->setData(Qt::DecorationRole, oIcon.pixmap(QSize(200,200)));
            }

            pItem->setData(Qt::DisplayRole, oInfoParser.getTitle());
            pItem->setData(Qt::UserRole + 1, oInfoParser.getDescription());
            pItem->setData(Qt::UserRole + 2, sDocumentPath);

            m_pTemplateListWidget->addItem(pItem);


        }


    }

    void showTemplate( std::string sFileName )
    {

        if (m_pWindowParser)
            delete m_pWindowParser;

        m_pWindowParser = new XMLParserWindow( this );
        m_pWindowParser->initializeFile(sFileName);

        m_pWindow = m_pWindowParser->getWindow();

        if (m_pWindow == NULL)
        {
            QMessageBox oMsgBox;
            oMsgBox.setIcon( QMessageBox::Critical );

            std::string sMessage = std::string("Error loading template: ") + sFileName;

            oMsgBox.setText( QString(sMessage.c_str()) );
            oMsgBox.exec();


            return;
        }

        m_pWindow->setMinimumSize(m_pWindow->sizeHint());

        /*
        QRect rec = QApplication::desktop()->availableGeometry(m_pWindow);

        QSize oWinSize(rec.width(),rec.height());

        oWinSize.setWidth( std::min(oWinSize.width(), m_pWindow->minimumWidth()+10) );
        oWinSize.setHeight( std::min(oWinSize.height()-100, m_pWindow->minimumHeight()+10));

        m_pApplicationWindowArea->setMinimumSize(oWinSize);
         */


        //m_pMainWindow->setMinimumSize( m_pApplicationWindowArea->minimumWidth() + m_pTemplateListWidget->width(), std::max(m_pApplicationWindowArea->minimumHeight()+20, m_pTemplateListWidget->height()) );

        m_pApplicationWindowArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_pApplicationWindowArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_pWindow->setMinimumSize(m_pWindow->size());
        m_pWindow->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);


        //m_pWindow->setParent(m_pApplicationWindowArea);

        // this also destroys the current window!
        m_pApplicationWindowArea->setWidget(m_pWindow);
        m_pApplicationWindowArea->setWidgetResizable(true);
        m_pWindow->setBaseSize(m_pApplicationWindowArea->size());

        //m_pApplicationWindowArea->setStyleSheet("background-color:red;");
        //m_pWindow->setStyleSheet("background-color:green;");
        m_pMainMainWindow->setWindowTitle("bioGUI - " + m_pWindow->windowTitle());

        if (m_pFramelessWindow != NULL)
        {
            //m_pFramelessWindow->setWindowTitle("bioGUI - " + m_pWindow->windowTitle());
        }

        m_pMainMainWindow->update();

        QSize newSize = m_pApplicationWindowArea->size();
        newSize.setWidth(newSize.width()-1);

        m_pApplicationWindowArea->update();
        m_pApplicationWindowArea->resizeEvent(new QResizeEvent(newSize, m_pApplicationWindowArea->size()));


        std::string sTest = sFileName;

        QFileInfo oFileInfo(QString(sFileName.c_str()));
    }


    void runProgram(std::string& sProgramToRun);

    void addRunningProcess(QProcess* pProcess)
    {
        m_vProcessList.append(pProcess);
    }

    void programFinished()
    {

        this->enableActions();

    }

    void killAllProcesses()
    {
        std::cerr << "Trying to kill " << m_vProcessList.size() << " process" << std::endl;

        for (auto& val: m_vProcessList)
        {
            QProcess* pProc = val;

            std::cerr << "Process " << pProc << std::endl;

            if (pProc->state() == QProcess::Running)
            {
                std::cerr << "Killing process " << pProc << std::endl;
                //pProc->terminate();
                pProc->kill();
            }

        }

        //m_vProcessList.clear();

    }


public slots:
    void resizeContent(QResizeEvent* pEvent)
    {
        m_pWindow->setMinimumSize(pEvent->size());
    }

protected:


    void setActionsState(bool bState)
    {
        if (m_pWindowParser != NULL)
            m_pWindowParser->setActionsEnabled(bState);

        this->m_pSaveTemplate->setEnabled(bState);
        this->m_pTemplateListWidget->setEnabled(bState);
        this->m_pReloadTemplates->setEnabled(bState);
    }

    void saveCurrentTemplate(QDir oTemplatePath)
    {

        QString sFileName = QFileDialog::getSaveFileName(0, "Save Template", oTemplatePath.absolutePath(), "Template Files (*.gui)" );

        if ((sFileName.isNull()) || (sFileName.size() == 0))
            return;

        m_pWindowParser->saveTemplate(sFileName);
        this->reloadTemplates();

    }

    void filterTemplates(const QString& sText)
    {

        if (m_pTemplateListWidget == NULL)
            return;

        if (m_pTemplateListWidget->count() == 0)
            return;


        for (int i = 0; i < m_pTemplateListWidget->count(); ++i)
        {

            QListWidgetItem* pItem = m_pTemplateListWidget->item(i);

            QString sTitle = pItem->data(Qt::DisplayRole).toString();
            QString sDescription = pItem->data(Qt::UserRole + 1).toString();

            if (sText.size() == 0)
            {
                // only set it to true - "" matches everywhere
                pItem->setHidden(false);
                continue;
            } else {

                bool bShow = sTitle.contains(sText, Qt::CaseInsensitive);
                bShow |= sDescription.contains(sText, Qt::CaseInsensitive);

                //qDebug() << sTitle << " " << sDescription << " " << sText << " " << bShow;

                if ( bShow )
                {
                    pItem->setHidden(false);
                } else {
                    pItem->setHidden(true);
                }

            }

        }


    }



    QMainWindow* m_pMainMainWindow = NULL;

    QWidget* m_pMainWindow = NULL;
    QListWidget* m_pTemplateListWidget = NULL;
    QAbstractButton* m_pSaveTemplate = NULL;
    QAbstractButton* m_pReloadTemplates = NULL;
    QAbstractButton* m_pDownloadTemplates = NULL;
    QVerticalScrollArea* m_pApplicationWindowArea = NULL;

    QDir m_oTemplatePath;

    QList<QProcess*> m_vProcessList;

    QWidget* m_pWindow = NULL;
    XMLParserWindow* m_pWindowParser = NULL;

    QString m_sDownloadServerLocation = "";
    QString m_sWSLBashLocation = "";


    /** TEST PLAYGROUND **/
    ExecutionRunThread* m_pExecThread = NULL;
    FramelessWindow* m_pFramelessWindow = NULL;

};


#endif //BIOGUI_BIOGUIAPP_H
