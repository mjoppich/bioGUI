//
// Created by mjopp on 26.03.2016.
//

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

#include <iostream>
#include <src/app/TemplateListDelegate.h>
#include <src/parsing/XMLParserInfo.h>
#include <src/app/QDownloadTemplatesWindow.h>
#include <src/parsing/XMLParserExecution.h>
#include <src/parsing/XMLParserWindow.h>
#include <src/app/ExecutionRunThread.h>

class bioGUIapp : public QApplication {
    Q_OBJECT
public:

    bioGUIapp(int& argc, char** argv);

    void reloadTemplates()
    {
        QDir oTemplatePath = QDir::currentPath() + "/templates/";

        this->addTemplates( oTemplatePath );
    }

    void loadInitFile(QDir oPath)
    {
        QString sConfigFilePath = oPath.absolutePath() + "/config.ini";

        QSettings* pSettings = new QSettings(sConfigFilePath, QSettings::IniFormat);

        QStringList allKeys = pSettings->allKeys();
        m_sDownloadServerLocation = pSettings->value("server/location", "localhost").toString();

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

        });

    }

    void addTemplates(QDir oDirectory)
    {

        m_pTemplates->clear();
        m_pTemplates->setItemDelegate(new TemplateListDelegate(m_pTemplates));

        // TODO do i have to delete them first?
        m_vTemplateItems.clear();

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

            XMLParserInfo oInfoParser(oFile.absoluteFilePath().toStdString());

            if (oInfoParser.isValid() == false)
            {

                delete pItem;
                std::cerr << "Error loading template: " << oFile.absoluteFilePath().toStdString() << std::endl;

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
            pItem->setData(Qt::UserRole + 2, QString(oInfoParser.getTemplateFile().c_str()));

            m_vTemplateItems.append(pItem);

            m_pTemplates->addItem(pItem);


        }


    }

    void showTemplate( std::string sFileName )
    {

        if (m_pWindowParser)
            delete m_pWindowParser;

        m_pWindowParser = new XMLParserWindow( this, sFileName);

        m_pWindow = m_pWindowParser->getWindow();

        QRect rec = QApplication::desktop()->availableGeometry(m_pWindow);

        QSize oWinSize(rec.width(),rec.height());

        oWinSize.setWidth( std::min(oWinSize.width(), m_pWindow->minimumWidth()+10) );
        oWinSize.setHeight( std::min(oWinSize.height()-100, m_pWindow->minimumHeight()+10));


        m_pApplicationWindowArea->setMinimumSize(oWinSize);
        m_pMainWindow->setMinimumSize( m_pApplicationWindowArea->minimumWidth() + m_pTemplates->width(), std::max(m_pApplicationWindowArea->minimumHeight()+20, m_pTemplates->height()) );

        m_pApplicationWindowArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        m_pApplicationWindowArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        // this also destroys the current window!
        m_pApplicationWindowArea->setWidget(m_pWindow);
        m_pApplicationWindowArea->setWidgetResizable(true);

        m_pMainMainWindow->setWindowTitle("bioGUI - " + m_pWindow->windowTitle());

        //this->connect(m_pApplicationWindowArea, SIGNAL(resizeEvent(QResizeEvent*)), this, SLOT(resizeContent(QResizeEvent*)));

        /*
        std::cout << "spongebob " << m_pWindow->minimumWidth() << " " << m_pWindow->minimumHeight() << std::endl;
        std::cout << "spongebob " << m_pMainWindow->minimumWidth() << " " << m_pMainWindow->minimumHeight() << std::endl;
        std::cout << "spongebob " << m_pMainWindow->width() << " " << m_pMainWindow->height() << std::endl;
        */

        std::string sTest = sFileName;

        QFileInfo oFileInfo(QString(sFileName.c_str()));
        //m_pWindowParser->saveTemplate(oFileInfo.absoluteDir().absolutePath() + "/bla.gui");

    }


    void runProgram()
    {

        ExecutionNetwork* pNetwork = NULL;
        ExecutionRunThread* pThread = NULL;

        this->disableActions();

        XMLParserExecution* pParseExecution = new XMLParserExecution( m_pWindowParser->getTemplateFile() );

        pThread = new ExecutionRunThread(m_pWindowParser, pParseExecution);

        this->connect(pThread, &QThread::started, pThread, &ExecutionRunThread::startExecution);
        this->connect(pThread, &ExecutionRunThread::executionFinished, this, &bioGUIapp::programFinished);
        this->connect(pThread, &ExecutionRunThread::executionFinished, pThread, &ExecutionRunThread::deleteLater);

        pThread->start();

    }

    void programFinished()
    {

        this->enableActions();

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
        this->m_pTemplates->setEnabled(bState);
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

        if (m_pTemplates == NULL)
            return;

        if (m_pTemplates->count() == 0)
            return;


        for (int i = 0; i < m_vTemplateItems.size(); ++i)
        {

            QListWidgetItem* pItem = m_vTemplateItems.at(i);

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
    QListWidget* m_pTemplates = NULL;
    QAbstractButton* m_pSaveTemplate = NULL;
    QAbstractButton* m_pReloadTemplates = NULL;
    QAbstractButton* m_pDownloadTemplates = NULL;
    QScrollArea* m_pApplicationWindowArea = NULL;

    QDir m_oTemplatePath;

    QWidget* m_pWindow = NULL;
    XMLParserWindow* m_pWindowParser = NULL;

    QList<QListWidgetItem*> m_vTemplateItems;


    QString m_sDownloadServerLocation = "";


};


#endif //BIOGUI_BIOGUIAPP_H
