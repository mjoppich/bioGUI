//
// Created by mjopp on 26.03.2016.
//

#ifndef BIOGUI_BIOGUIAPP_H
#define BIOGUI_BIOGUIAPP_H

#include <QApplication>
#include <QXmlQuery>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QXmlResultItems>
#include <iostream>
#include <src/app/TemplateListDelegate.h>
#include <src/parsing/XMLParserInfo.h>
#include "parsing/XMLParserExecution.h"
#include "parsing/XMLParserWindow.h"
#include "app/ExecutionRunThread.h"

class bioGUIapp : public QApplication {
    Q_OBJECT
public:

    bioGUIapp(int argc, char* argv[])
    : QApplication(argc, argv)
    {

        //m_sGUIFile = "/cygdrive/c/libraries/bioGUI/example.gui";
        //m_sGUIFile = "/home/users/joppich/cpp/bioGUI/example.gui";

        //m_pWindowParser = new XMLParserWindow( this, "/home/users/joppich/cpp/bioGUI/example.gui" );

        QDir oTemplatePath = QDir::currentPath() + "/templates/";

        m_pMainWindow = new QWidget();

        QHBoxLayout* pMainLayout = new QHBoxLayout();

        QVBoxLayout* pLeftLayout = new QVBoxLayout();


        // this listwidget shows all available items
        m_pTemplates = new QListWidget();
        m_pTemplates->setMaximumWidth(220);

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



        // opens save template dialogue
        m_pSaveTemplate = new QPushButton("Save Template");

        connect(m_pSaveTemplate, &QAbstractButton::clicked, [oTemplatePath, this] () {

            this->saveCurrentTemplate(oTemplatePath);

        });

        pLeftLayout->addWidget(m_pSaveTemplate);


        pMainLayout->addLayout(pLeftLayout);

        m_pApplicationWindowArea = new QScrollArea();
        pMainLayout->addWidget(m_pApplicationWindowArea);

        m_pMainWindow->setLayout(pMainLayout);
        m_pMainWindow->show();

        // For testing purposes only
        //this->runProgram();

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

            QString sIconPath = oInfoParser.getIcon();
            if ((sIconPath.length() > 0) || (true))
            {

                //QIcon oIcon(sIconPath);
                QIcon oIcon = QIcon::fromTheme("edit-undo");

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

        m_pApplicationWindowArea->setMinimumSize( m_pWindow->minimumWidth()+10, m_pWindow->minimumHeight()+10);
        m_pMainWindow->setMinimumSize( m_pApplicationWindowArea->minimumWidth() + m_pTemplates->width(), std::max(m_pApplicationWindowArea->minimumHeight()+20, m_pTemplates->height()) );

        m_pApplicationWindowArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        m_pApplicationWindowArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        // this also destroys the current window!
        m_pApplicationWindowArea->setWidget(m_pWindow);

        m_pMainWindow->setWindowTitle(m_pWindow->windowTitle());

        std::cout << "spongebob " << m_pWindow->minimumWidth() << " " << m_pWindow->minimumHeight() << std::endl;
        std::cout << "spongebob " << m_pMainWindow->minimumWidth() << " " << m_pMainWindow->minimumHeight() << std::endl;
        std::cout << "spongebob " << m_pMainWindow->width() << " " << m_pMainWindow->height() << std::endl;


        std::string sTest = sFileName;

        QFileInfo oFileInfo(QString(sFileName.c_str()));
        //m_pWindowParser->saveTemplate(oFileInfo.absoluteDir().absolutePath() + "/bla.gui");

    }


    void runProgram()
    {

        this->disableActions();

        XMLParserExecution oParseExecution( m_pWindowParser->getTemplateFile() );
        ExecutionNetwork* pNetwork = oParseExecution.getExecutionNetwork();

        ExecutionRunThread* pThread = new ExecutionRunThread(m_pWindowParser, pNetwork);

        this->connect(pThread, &QThread::finished, this, &bioGUIapp::programFinished);

        pThread->start();


    }

    void programFinished()
    {

        this->enableActions();

    }

protected:

    void saveCurrentTemplate(QDir oTemplatePath)
    {

        QString sFileName = QFileDialog::getSaveFileName(0, "Save Template", oTemplatePath.absolutePath(), "Template Files (*.gui)" );

        if ((sFileName.isNull()) || (sFileName.size() == 0))
            return;

        m_pWindowParser->saveTemplate(sFileName);

    }




    QWidget* m_pMainWindow;
    QListWidget* m_pTemplates;
    QAbstractButton* m_pSaveTemplate;
    QScrollArea* m_pApplicationWindowArea;



    QWidget* m_pWindow;
    XMLParserWindow* m_pWindowParser;


};


#endif //BIOGUI_BIOGUIAPP_H
