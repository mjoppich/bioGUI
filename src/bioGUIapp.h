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
#include <QDesktopWidget>
#include <iostream>
#include <src/app/TemplateListDelegate.h>
#include <src/parsing/XMLParserInfo.h>
#include <QtWidgets/qsplitter.h>
#include "parsing/XMLParserExecution.h"
#include "parsing/XMLParserWindow.h"
#include "app/ExecutionRunThread.h"

class bioGUIapp : public QApplication {
    Q_OBJECT
public:

    bioGUIapp(int argc, char* argv[])
    : QApplication(argc, argv)
    {

        std::cerr << "Main Application" << std::endl;
        std::cerr << QThread::currentThreadId() << std::endl;

        qDebug() << QStyleFactory::keys();

#ifdef __linux

#else
        QStyle* pStyle = NULL;
        pStyle = QStyleFactory::create("Fusion");
        QApplication::setStyle(pStyle);
#endif

        //m_sGUIFile = "/cygdrive/c/libraries/bioGUI/example.gui";
        //m_sGUIFile = "/home/users/joppich/cpp/bioGUI/example.gui";

        //m_pWindowParser = new XMLParserWindow( this, "/home/users/joppich/cpp/bioGUI/example.gui" );

        QDir oTemplatePath = QDir::currentPath() + "/templates/";

        m_pMainWindow = new QWidget();

        //QHBoxLayout* pMainLayout = new QHBoxLayout();
        QSplitter* pSplitter = new QSplitter(Qt::Horizontal);

        QVBoxLayout* pLeftLayout = new QVBoxLayout();

        // this listwidget shows all available items
        m_pTemplates = new QListWidget();
        //m_pTemplates->setMaximumWidth(220);

        this->connect(m_pTemplates, &QListWidget::itemSelectionChanged, [this] () {

            QList<QListWidgetItem*> vSelected = m_pTemplates->selectedItems();

            if (vSelected.size() == 1)
            {
                QString oFileName = qvariant_cast<QString>(vSelected.at(0)->data(Qt::UserRole+2));

                std::string sFileName = oFileName.toStdString();

                this->showTemplate(sFileName);
            }



        });

        pLeftLayout->addWidget(m_pTemplates);


        QGroupBox* pOptionsGroup = new QGroupBox("Options");
        QGridLayout* pOptionsLayout = new QGridLayout();

        // opens save template dialogue
        m_pReloadTemplates = new QPushButton("Reload");

        connect(m_pReloadTemplates, &QAbstractButton::clicked, [this] () {

            this->reloadTemplates();

        });

        pOptionsLayout->addWidget(m_pReloadTemplates, 0, 0);

        m_pSaveTemplate = new QPushButton("Save Template");

        connect(m_pSaveTemplate, &QAbstractButton::clicked, [oTemplatePath, this] () {

            this->saveCurrentTemplate(oTemplatePath);

        });

        pOptionsLayout->addWidget(m_pSaveTemplate, 0, 1);
        pOptionsGroup->setLayout(pOptionsLayout);

        pLeftLayout->addWidget(pOptionsGroup);

        QWidget* pLeft = new QWidget;
        pLeft->setLayout(pLeftLayout);

        pSplitter->addWidget(pLeft);

        //pMainLayout->addLayout(pLeftLayout);

        m_pApplicationWindowArea = new QScrollArea();
        //pMainLayout->addWidget(m_pApplicationWindowArea);
        pSplitter->addWidget(m_pApplicationWindowArea);

        QGridLayout* pGridLayout = new QGridLayout();
        pGridLayout->addWidget(pSplitter, 0,0);
        m_pMainWindow->setLayout(pGridLayout);

        //m_pMainWindow->setLayout(pMainLayout);
        //m_pMainWindow->show();

        m_pMainMainWindow = new QMainWindow();
        m_pMainMainWindow->setCentralWidget(pSplitter);
        m_pMainMainWindow->show();

        // For testing purposes only
        //this->runProgram();

        this->reloadTemplates();

    }

    void reloadTemplates()
    {

        QDir oTemplatePath = QDir::currentPath() + "/templates/";

        this->addTemplates( oTemplatePath );

    }

    void enableActions()
    {
        m_pWindowParser->setActionsEnabled(true);

        this->m_pSaveTemplate->setEnabled(true);
        this->m_pTemplates->setEnabled(true);
    }

    void disableActions()
    {
        m_pWindowParser->setActionsEnabled(false);

        this->m_pSaveTemplate->setEnabled(false);
        this->m_pTemplates->setEnabled(false);
    }

    void addTemplates(QDir oDirectory)
    {

        m_pTemplates->clear();

        m_pTemplates->setItemDelegate(new TemplateListDelegate(m_pTemplates));

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

            QListWidgetItem *item = new QListWidgetItem();

            XMLParserInfo oInfoParser(oFile.absoluteFilePath().toStdString());

            if (oInfoParser.isValid() == false)
            {

                std::cerr << "Error loading template: " << oFile.absoluteFilePath().toStdString() << std::endl;

                continue;
            }


            QString sIconPath = oInfoParser.getIcon();

            if ((sIconPath.length() > 0))
            {

                //QIcon oIcon(sIconPath);
                QIcon oIcon = QIcon(sIconPath);

                item->setData(Qt::DecorationRole, oIcon.pixmap(QSize(200,200)));

            }


            item->setData(Qt::DisplayRole, oInfoParser.getTitle());
            item->setData(Qt::UserRole + 1, oInfoParser.getDescription());
            item->setData(Qt::UserRole + 2, QString(oInfoParser.getTemplateFile().c_str()));

            m_pTemplates->addItem(item);


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

        QWidget* pWindow = m_pMainWindow;

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




    void saveCurrentTemplate(QDir oTemplatePath)
    {

        QString sFileName = QFileDialog::getSaveFileName(0, "Save Template", oTemplatePath.absolutePath(), "Template Files (*.gui)" );

        if ((sFileName.isNull()) || (sFileName.size() == 0))
            return;

        m_pWindowParser->saveTemplate(sFileName);
        this->reloadTemplates();

    }



    QMainWindow* m_pMainMainWindow = NULL;

    QWidget* m_pMainWindow = NULL;
    QListWidget* m_pTemplates = NULL;
    QAbstractButton* m_pSaveTemplate = NULL;
    QAbstractButton* m_pReloadTemplates = NULL;
    QScrollArea* m_pApplicationWindowArea = NULL;



    QWidget* m_pWindow = NULL;
    XMLParserWindow* m_pWindowParser = NULL;


};


#endif //BIOGUI_BIOGUIAPP_H
