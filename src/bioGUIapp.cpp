//
// Created by mjopp on 26.03.2016.
//

#include "bioGUIapp.h"

#include <src/app/TemplateListWidget.h>

bioGUIapp::bioGUIapp(int& argc, char** argv)
        : QApplication(argc, (char**)argv)
{

    std::cerr << "Main Application currently in dir: " << std::endl;
    std::cerr << QDir::currentPath().toStdString() << std::endl;

    this->loadInitFile(QDir::currentPath());

    /*
     *
     * PRINT HELP
     *
     *
     */

    XMLParserExecution oXMLParser;
    oXMLParser.printAvailableNodes();


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

    m_oTemplatePath = QDir::currentPath();
    m_pMainWindow = new QWidget();

    //QHBoxLayout* pMainLayout = new QHBoxLayout();
    QSplitter* pSplitter = new QSplitter(Qt::Horizontal);

    QVBoxLayout* pLeftLayout = new QVBoxLayout();


    QHBoxLayout* pFilterLayout = new QHBoxLayout();
    QLabel* pFilterLabel = new QLabel("Filter:");
    QLineEdit* pFilterLine = new QLineEdit();

    m_pTemplates = new TemplateListWidget();

    connect(pFilterLine, &QLineEdit::textChanged, [this] (const QString& text) {

        this->filterTemplates(text);

    });

    pFilterLayout->addWidget(pFilterLabel);
    pFilterLayout->addWidget(pFilterLine);

    QWidget* pFilterWidget = new QWidget();
    pFilterWidget->setLayout(pFilterLayout);

    pLeftLayout->addWidget(pFilterWidget);



    // this listwidget shows all available items
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
