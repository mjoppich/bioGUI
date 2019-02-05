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

#include "bioGUIapp.h"

#include <src/app/TemplateListWidget.h>

bioGUIapp::bioGUIapp(int& argc, char** argv)
        : QApplication(argc, (char**)argv)
{

    QCoreApplication::setApplicationName("bioGUI");
    std::cerr << "Launching " << QCoreApplication::applicationName().toStdString() << std::endl;
    std::cerr << "Main Application currently in dir: " << std::endl;

    qDebug() <<"ssl:" << QSslSocket::sslLibraryBuildVersionString();


    QString sApplicationDir = QCoreApplication::applicationDirPath() + QDir::separator();
    QDir::setCurrent(sApplicationDir);

    m_oTemplatePath = QDir::currentPath();

    if (argc > 1)
    {
        QString sGivenPath = QString(argv[1]);
        QDir oGivenPath (sGivenPath);

        if (oGivenPath.exists() == true)
        {
            m_oTemplatePath = oGivenPath;
        } else {

            std::cerr << "Given path does not exist: " << oGivenPath.path().toStdString() << std::endl;

        }
    }

    //QMessageBox::warning(NULL, sApplicationDir, sApplicationDir); 
    //QMessageBox::warning(NULL, sCurrentDir, sCurrentDir);

    std::cerr << m_oTemplatePath.path().toStdString() << std::endl;

    this->loadInitFile(m_oTemplatePath);

    /*
     *
     * PRINT HELP
     *
     *
     */

    std::cout << "Visual Elements" << std::endl;

    XMLParserWindow oXMLParserWindow(this);
    oXMLParserWindow.printAvailableNodes();

    std::cout << "Execution Nodes" << std::endl;

    XMLParserExecution oXMLParserExecution;
    oXMLParserExecution.printAvailableNodes();

    /*
     *
     *
     *
     *
     *
     */


    //qDebug() << QStyleFactory::keys();

#ifdef __linux

#else
    QStyle* pStyle = NULL;
        pStyle = QStyleFactory::create("Fusion");
        QApplication::setStyle(pStyle);
#endif

    m_pMainWindow = new QWidget();

    //QHBoxLayout* pMainLayout = new QHBoxLayout();
    QSplitter* pSplitter = new QSplitter(Qt::Horizontal);

    QVBoxLayout* pLeftLayout = new QVBoxLayout();


    QHBoxLayout* pFilterLayout = new QHBoxLayout();
    QLabel* pFilterLabel = new QLabel("Filter:");
    QLineEdit* pFilterLine = new QLineEdit();

    m_pTemplateListWidget = new TemplateListWidget();

    connect(pFilterLine, &QLineEdit::textChanged, [this] (const QString& text) {

        this->filterTemplates(text);

    });

    pFilterLayout->addWidget(pFilterLabel);
    pFilterLayout->addWidget(pFilterLine);

    QWidget* pFilterWidget = new QWidget();
    pFilterWidget->setLayout(pFilterLayout);

    pLeftLayout->addWidget(pFilterWidget);



    // this listwidget shows all available items
    //m_pTemplateListWidget->setMaximumWidth(220);

    this->connect(m_pTemplateListWidget, &QListWidget::itemSelectionChanged, [this] () {

        QList<QListWidgetItem*> vSelected = m_pTemplateListWidget->selectedItems();

        if (vSelected.size() == 1)
        {
            QString oFileName = qvariant_cast<QString>(vSelected.at(0)->data(Qt::UserRole+2));

            std::string sFileName = oFileName.toStdString();

            this->showTemplate(sFileName);
        }



    });

    pLeftLayout->addWidget(m_pTemplateListWidget);


    QGroupBox* pOptionsGroup = new QGroupBox("Options");
    QGridLayout* pOptionsLayout = new QGridLayout();

    // opens save template dialogue
    m_pReloadTemplates = new QPushButton("Reload");

    connect(m_pReloadTemplates, &QAbstractButton::clicked, [this] () {

        this->reloadTemplates();

    });

    pOptionsLayout->addWidget(m_pReloadTemplates, 0, 0);

    // saves current template
    m_pSaveTemplate = new QPushButton("Save Template");

    connect(m_pSaveTemplate, &QAbstractButton::clicked, [this] () {

        this->saveCurrentTemplate(this->m_oTemplatePath.path() + "/templates/");

    });
    pOptionsLayout->addWidget(m_pSaveTemplate, 0, 1);


    // opens download dialog
    m_pDownloadTemplates = new QPushButton("Download Templates");

    connect(m_pDownloadTemplates, &QAbstractButton::clicked, [this] () {

        this->downloadTemplatesDialogue();

    });
    pOptionsLayout->addWidget(m_pDownloadTemplates, 1, 0, 1, 2);



    pOptionsGroup->setLayout(pOptionsLayout);

    pLeftLayout->addWidget(pOptionsGroup);

    QWidget* pLeft = new QWidget;
    pLeft->setLayout(pLeftLayout);
    pLeft->setMinimumWidth(250);
    pLeft->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    pSplitter->addWidget(pLeft);

    //pMainLayout->addLayout(pLeftLayout);

    m_pApplicationWindowArea = new QVerticalScrollArea();

    //pMainLayout->addWidget(m_pApplicationWindowArea);
    pSplitter->addWidget(m_pApplicationWindowArea);

    pSplitter->setCollapsible(0, false);
    pSplitter->setCollapsible(1, false);

    QGridLayout* pGridLayout = new QGridLayout();
    pGridLayout->addWidget(pSplitter, 0,0);
    m_pMainWindow->setLayout(pGridLayout);
    m_pMainMainWindow = new QMainWindow();
    m_pMainMainWindow->setCentralWidget(pSplitter);
    m_pMainMainWindow->show();


    QList<int> sizeList;
    sizeList << 250 << m_pMainMainWindow->width()-250;
    pSplitter->setSizes(sizeList);
    // For testing purposes only
    //this->runProgram();

    this->reloadTemplates();

}

void bioGUIapp::runProgram(std::string& sProgramToRun)
{

    if (m_pExecThread != NULL)
    {
        LOGERROR("QUIT");
        m_pExecThread->quit();
        LOGERROR("WAIT");
        m_pExecThread->wait();
        LOGERROR("DELETE");
        m_pExecThread->deleteLater();

        m_pExecThread = NULL;
    }


    this->disableActions();

    XMLParserExecution* pParseExecution = new XMLParserExecution();
    std::string sCurrentFilePath = m_pWindowParser->getCurrentDocumentPath();
    pParseExecution->initializeFile(sCurrentFilePath);

    m_pExecThread = new ExecutionRunThread(m_pWindowParser, pParseExecution, sProgramToRun, this);
    bioGUIapp* pApp = this;

    this->connect(m_pExecThread, &QThread::started, m_pExecThread, &ExecutionRunThread::startExecution);
    this->connect(m_pExecThread, &ExecutionRunThread::executionFinished, [pApp] () {

        pApp->programFinished();

    });
    //this->connect(pThread, &ExecutionRunThread::executionFinished, pThread, &ExecutionRunThread::deleteLater);

    m_pExecThread->start();

}
