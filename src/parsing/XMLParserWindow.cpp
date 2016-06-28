//
// Created by joppich on 3/29/16.
//

#include <src/app/QComboItem.h>
#include <src/app/AdvancedStreamBox.h>
#include <src/app/QAbstractButtonItem.h>
#include <src/app/QExtendedRadioButton.h>
#include <src/app/QExtendedCheckBox.h>
#include <src/app/QExclusiveGroupBox.h>
#include "XMLParserWindow.h"

#include <QGridLayout>
#include <src/app/QExtGridLayout.h>
#include <src/app/QClickableLabel.h>
#include <QTextEdit>
#include <src/app/QSortableFileList.h>

#include "../bioGUIapp.h"

QLayout* XMLParserWindow::createLayout(QDomElement* pElement)
{
    QString sTag = pElement->tagName();


    if (sTag.compare("hgroup", Qt::CaseInsensitive) == 0)
    {
        QHBoxLayout *pLayout = new QOrderedHBoxLayout();

        pLayout->setAlignment(Qt::AlignHorizontal_Mask);

        return (QLayout*) pLayout;
    }

    if (sTag.compare("vgroup", Qt::CaseInsensitive) == 0)
    {
        QVBoxLayout *pLayout = new QOrderedVBoxLayout();

        pLayout->setAlignment(Qt::AlignVertical_Mask);

        return (QLayout*) pLayout;
    }


    if (sTag.compare("grid", Qt::CaseInsensitive) == 0)
    {

        int iRows = this->getAttribute(pElement, "rows", 0).toInt();
        int iCols = this->getAttribute(pElement, "cols", 0).toInt();

        QExtGridLayout *pLayout = new QExtGridLayout(iRows, iCols);

        pLayout->setAlignment(Qt::AlignVertical_Mask);


        return (QLayout*) pLayout;
    }

    return NULL;
}

QWidget* XMLParserWindow::createComponent(QDomElement* pElement, bool* pChildrenFinished)
{

    if (pElement->isElement() != true)
    {
        // this should not happen though ...
        return NULL;
    }

    QString sTag = pElement->tagName();
    QString sValue = pElement->text();

    QWidget* pWidget = NULL;

    if (sTag.compare("window", Qt::CaseInsensitive) == 0)
    {
        QWidget *pWindow = new QWidget();
        pWidget = pWindow;

        /*
         * apply sizes here
         *
         */

        QString sWidth =  this->getAttribute(pElement, "width" , "400");
        QString sHeight = this->getAttribute(pElement, "height", "400");

        int iWidth = sWidth.toInt();
        int iHeight = sHeight.toInt();

        pWidget->setMinimumSize(iWidth, iHeight);

        /*
         * Window Title
         */

        QString sTitle = this->getAttribute(pElement, "title", "bioGUI");
        if (sTitle.length() > 0)
        {
            pWidget->setWindowTitle( sTitle );
        }

    }

    if (sTag.compare("label", Qt::CaseInsensitive) == 0)
    {
        QLabel *pLabel = new QLabel( sValue );

        this->addValueFetcher(pElement, [sValue] () {return sValue.toStdString();});

        pWidget = pLabel;
    }

    if (sTag.compare("link", Qt::CaseInsensitive) == 0)
    {
        QString sText = this->getAttribute(pElement, "text", sValue);

        QClickableLabel *pLabel = new QClickableLabel( sText );
        pLabel->setStyleSheet("QLabel { color : blue; }");

        this->addValueFetcher(pElement, [sValue] () {return sValue.toStdString();});

        QObject::connect(pLabel,&QClickableLabel::clicked,[pLabel] (){

            QDesktopServices::openUrl(QUrl(pLabel->text()));

        });



        pWidget = pLabel;
    }


    if (sTag.compare("fileselectbox", Qt::CaseInsensitive) == 0)
    {

        QString sFileFilter = this->getAttribute(pElement, "filter", "");
        QString sFilePath = this->getAttribute(pElement, "location", "");
        QString sDelimeter = this->getAttribute(pElement, "delim", " ");



        QSortableFileList *pList = new QSortableFileList( sFilePath, sFileFilter, sDelimeter );

        this->addValueFetcher(pElement, [pList] () {return pList->evaluate().toStdString();});

        pWidget = pList;
    }

    if (sTag.compare("input", Qt::CaseInsensitive) == 0)
    {

        bool bMultiLine = ( this->getAttribute(pElement, "multi", "false").compare("true", Qt::CaseInsensitive) == 0);

        if (!bMultiLine)
        {
            QLineEdit *pLineEdit = new QLineEdit( sValue );

            QString sType = this->getAttribute(pElement, "type", "");

            if (sType.size() != 0)
            {

                if (sType.compare("string", Qt::CaseInsensitive) == 0)
                {
                    // nothing
                }

                if (sType.compare("int", Qt::CaseInsensitive) == 0)
                {
                    pLineEdit->setValidator( new QIntValidator() );
                }

                if (sType.compare("float", Qt::CaseInsensitive) == 0)
                {
                    pLineEdit->setValidator( new QDoubleValidator() );
                }

            }

            this->addValueFetcher(pElement, [pLineEdit] () {return pLineEdit->text().toStdString();});

            pWidget = pLineEdit;
        } else {

            QTextEdit* pTextEdit = new QTextEdit(sValue);

            this->addValueFetcher(pElement, [pTextEdit] () {return pTextEdit->toPlainText().toStdString();});

            pWidget = pTextEdit;

        }



    }


    if (sTag.compare("filedialog", Qt::CaseInsensitive) == 0)
    {

        QWidget* pLineButton = new QWidget();
        QHBoxLayout* pLayout = new QHBoxLayout();


        QLineEdit* pLineEdit = new QLineEdit();
        QString sPathHint = QDir::currentPath();

        QString sLineEditLocation = this->getAttribute(pElement, "location", "");
        if (sLineEditLocation.length() > 0)
        {
            pLineEdit->setText(sLineEditLocation);

            QFileInfo oLineEditInfo( sLineEditLocation );

            sPathHint = oLineEditInfo.absoluteDir().path();
        }



        QPushButton* pFileButton = new QPushButton(sValue);

        bool bMultiples = (this->getAttribute(pElement, "multiples", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);
        bool bOutput = (this->getAttribute(pElement, "output", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);
        bool bFolder = (this->getAttribute(pElement, "folder", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);
        QString sFileDelim = this->getAttribute(pElement, "multiples_delim", ";");
        QString sFileFilter = this->getAttribute(pElement, "filter", "");

        pFileButton->connect(pFileButton,&QAbstractButton::clicked,[pLineEdit, bMultiples, bOutput, bFolder, sFileDelim, sFileFilter, sPathHint] (bool bChecked){

            if (bFolder)
            {

                QString sFolder = QFileDialog::getExistingDirectory(0, ("Select Output Folder"), sPathHint);
                pLineEdit->setText(sFolder);

            } else {


                if (bOutput)
                {

                    if (bMultiples)
                    {
                        throw "unsupported option: output and mutliples";

                    } else {

                        QString sFileName = QFileDialog::getSaveFileName(0, "Select Input File", sPathHint, sFileFilter);
                        pLineEdit->setText(sFileName);

                    }

                } else {


                    if (bMultiples)
                    {
                        QStringList vSelectedFiles = QFileDialog::getOpenFileNames(0, "Select Input Files", sPathHint, sFileFilter);
                        QString sFiles = vSelectedFiles.join(sFileDelim);
                        pLineEdit->setText(sFiles);

                    } else {

                        QString sFileName = QFileDialog::getOpenFileName(0, "Select Input File", sPathHint, sFileFilter);
                        pLineEdit->setText(sFileName);

                    }

                }

            }



        });

        pLayout->addWidget( pLineEdit, 0, Qt::AlignLeft );
        pLayout->addWidget( pFileButton, 0, Qt::AlignLeft );
        pLineButton->setLayout( pLayout );

        this->addValueFetcher(pElement, [pLineEdit] () {return pLineEdit->text().toStdString();});

        pWidget = pLineButton;
    }

    if (sTag.compare("action", Qt::CaseInsensitive) == 0)
    {
        QPushButton *pAction = new QPushButton( sValue );

        bioGUIapp* pApp = this->m_pApp;

        pAction->connect(pAction,&QAbstractButton::clicked,[pApp] (bool bChecked){

            pApp->runProgram();

        });

        m_vActions.push_back(pAction);

        pWidget = pAction;
    }


    if (sTag.compare("radiobutton", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QString sButtonValue = this->getAttribute(pElement, "value", sValue);
        QExtendedRadioButton* pButtonItem = new QExtendedRadioButton(sValue, sButtonValue);

        pWidget = pButtonItem;

    }

    if (sTag.compare("checkbox", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QString sButtonValue = this->getAttribute(pElement, "value", sValue);
        QExtendedCheckBox* pButtonItem = new QExtendedCheckBox(sValue, sButtonValue);

        bool bSelectOnWindows = (this->getAttribute(pElement, "selectonwindows", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);
        bool bSelected = (this->getAttribute(pElement, "selected", "FALSE").compare("TRUE", Qt::CaseInsensitive) == 0);

        if ((!bSelected) && (bSelectOnWindows))
        {
            // get current OS
            if (QSysInfo::windowsVersion() != QSysInfo::WV_None)
            {
                bSelected = true;
            }
        }

        pButtonItem->setChecked(bSelected);

        this->addValueFetcher(pElement, [pButtonItem] () {

            if (pButtonItem->isChecked())
                return pButtonItem->getValue().toStdString();

            return std::string("");

        });

        pWidget = pButtonItem;

    }

    bool bBoolean = true;

    if (sTag.compare("groupbox", Qt::CaseInsensitive) == 0)
    {

        pWidget = this->createGroupBox(pElement, pChildrenFinished);
    }

    if (sTag.compare("group", Qt::CaseInsensitive) == 0)
    {


        pWidget = this->createGroup(pElement, pChildrenFinished);

    }

    if (sTag.compare("comboitem", Qt::CaseInsensitive) == 0) {


        QComboItem* pItem =  new QComboItem("", "");

        pItem->setValue( sValue );
        QString sData = this->getAttribute(pElement, "value", sValue);
        pItem->setData(sData);

        pWidget = pItem;
    }


    if (sTag.compare("combobox", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QString sSelected = this->getAttribute(pElement, "selected", "");
        QStringList vSelected;

        if (sSelected.length() > 0)
        {
            vSelected = sSelected.split(";");
        }

        QComboBox *pComboBox = new QComboBox();

        QDomNodeList oChildren = pElement->childNodes();
        for (size_t i = 0; i < oChildren.size(); ++i)
        {
            QDomElement oChildNode = oChildren.at(i).toElement();
            QWidget* pChildElement = createComponent(&oChildNode, &bBoolean);

            if (pChildElement == NULL)
                throw "error in creating groupbox components";

            if (QComboItem* pComboItem = dynamic_cast<QComboItem *>(pChildElement))
            {

                pComboBox->addItem( pComboItem->getValue(), pComboItem->getData() );

                if (i == 0)
                    pComboBox->setCurrentIndex(0);

                if ( vSelected.contains( pComboItem->getData().toString() ) )
                    pComboBox->setCurrentIndex(i);

            }
        }

        this->addValueFetcher(pElement, [pComboBox] () {

            return pComboBox->currentData().toString().toStdString();

        });

        pWidget = pComboBox;

    }

    if (sTag.compare("filelist", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QComboBox *pComboBox = new QComboBox();

        QString sCurrentPath = QDir::current().absolutePath();
        QString sSearchPath = sCurrentPath + "/install_templates/";
        QStringList vFileEnding;
        vFileEnding << "*.igui";

        std::cerr << "starting in " << sCurrentPath.toStdString() << std::endl;
        std::cerr << "searching in " << sSearchPath.toStdString() << std::endl;

        QDirIterator oDirIterator( sSearchPath,
                          vFileEnding,
                         QDir::NoSymLinks | QDir::Files,
                         QDirIterator::NoIteratorFlags );

        while (oDirIterator.hasNext()) {

            QString sFoundFile = oDirIterator.next();

            QFileInfo oFileInfo(sFoundFile);

            QString sFilePath = sFoundFile;
            QString sFileName = oFileInfo.baseName();

            QComboItem* pNewItem = new QComboItem( sFileName, sFilePath);
            pComboBox->addItem( pNewItem->getValue(), pNewItem->getData() );

        }

        this->addValueFetcher(pElement, [pComboBox] () {


            QVariant oSelected = pComboBox->currentData();

            if (oSelected.isNull())
                return std::string("");

            return oSelected.toString().toStdString();

        });

        pWidget = pComboBox;

    }

    if (sTag.compare("image", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QString sFileName = this->getAttribute(pElement, "src", "");

        QGraphicsScene* pScene = new QGraphicsScene();
        QGraphicsView* pView = new QGraphicsView(pScene);
        QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(QPixmap( sFileName ));

        pScene->addItem( pItem );

        /*
         * apply sizes here
         *
         */

        QString sWidth =  this->getAttribute(pElement, "width" , "");
        QString sHeight = this->getAttribute(pElement, "height", "");

        if ((sWidth.length() > 0) && (sHeight.length() > 0))
        {
            int iWidth = sWidth.toInt();
            int iHeight = sHeight.toInt();

            pView->setFixedSize(iWidth, iHeight);

        }

        pWidget = pView;

    }

    if (sTag.compare("stream", Qt::CaseInsensitive) == 0)
    {
        QCheckBox* pCheckbox = new QCheckBox(sValue);

        pWidget = pCheckbox;
    }

    if (sTag.compare("streambox", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QGroupBox* pGroupBox = new QGroupBox();

        AdvancedStreamBox* pStreamOut = new AdvancedStreamBox();

        // this could also be a hboxlayout or a grid layout
        QLayout* pLayout = new QVBoxLayout();

        pLayout->addWidget( pStreamOut );

        QPushButton* pClearButton = new QPushButton("Clear");

        this->connect(pClearButton, &QAbstractButton::clicked, [pStreamOut] {

            pStreamOut->clear();

        });

        pLayout->addWidget(pClearButton);

        QPushButton* pSaveLogButton = new QPushButton("Save Log");

        this->connect(pSaveLogButton, &QAbstractButton::clicked, [pStreamOut] {

            QString sFileName = QFileDialog::getSaveFileName(0, "Select Output File", QDir::currentPath(), "");

            pStreamOut->saveToFile(sFileName);

        });

        pLayout->addWidget(pSaveLogButton);


        QDomNodeList oChildren = pElement->childNodes();
        for (size_t i = 0; i < oChildren.size(); ++i)
        {
            QDomElement oChildNode = oChildren.at(i).toElement();
            QWidget* pChildElement = createComponent(&oChildNode, &bBoolean);

            if (pChildElement == NULL)
                throw "error in creating streambox components";

            if (QCheckBox* pButton = dynamic_cast<QCheckBox*>(pChildElement))
            {
                pButton->setChecked(true);

                QString sID = this->getAttribute(&oChildNode, "id", "");
                if (sID.length() > 0)
                {

                    m_pID2Widget->find(sID.toStdString())->second = pStreamOut;
                    pStreamOut->addStream( sID.toStdString(), pButton );
                    pLayout->addWidget( pChildElement );

                } else {
                    delete pChildElement;
                }

            }

        }

        pGroupBox->setLayout(pLayout);

        // must be done here because otherwise the groupbox is the id widget ...
        QString sID = this->getAttribute(pElement, "id", "");
        if (sID.length() > 0)
        {
            m_pID2Widget->insert( std::pair<std::string, QWidget*>(sID.toStdString(), pStreamOut));
        }

        pWidget = pGroupBox;


    }


    // some general things

    if (pWidget != NULL)
    {

        m_vWidgets.push_back(pWidget);

        QString sToolTip = this->getAttribute(pElement, "hint", "");
        if (sToolTip.length() > 0)
        {
            pWidget->setToolTip( sToolTip );
        }


        this->setID(pWidget, pElement, false);

    }

    return pWidget;
}
