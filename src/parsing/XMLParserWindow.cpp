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

#include <src/Maths.h>
#include <iomanip>
#include <sstream>
#include <src/parsing/visual_nodes/WindowNode.h>
#include <src/parsing/visual_nodes/WindowWidgetInputNode.h>

#include "../bioGUIapp.h"

QLayout* XMLParserWindow::createLayout(QDomElement* pElement)
{
    WindowNode<QLayout>::CreatedElement oLayout = m_pWidgetNodeFactory->createLayoutElement(pElement);

    return oLayout.pElement;
}

QWidget* XMLParserWindow::createComponent(QDomElement* pElement, bool* pChildrenFinished)
{

    if (pElement->isElement() != true)
    {
        // this should not happen though ...
        return NULL;
    }

    WindowNode<QWidget>::CreatedElement oWidget = m_pWidgetNodeFactory->createWidgetElement(pElement);

    if (oWidget.hasRetriever())
    {
        for (Retriever& oRet : oWidget.vRetriever)
        {
            this->addValueFetcher(oRet.sElementID, oRet.oRetriever);
        }

    }

    *pChildrenFinished = oWidget.bHasChildrenFinished;

    QWidget* pReturn = oWidget.pElement;

    if (pReturn != NULL)
    {

        m_vWidgets.push_back(pReturn);

        QString sToolTip = this->getAttribute(pElement, "hint", "");
        if (sToolTip.length() > 0)
        {
            pReturn->setToolTip( sToolTip );
        }


        this->setID(pReturn, pElement, false);

    }

    return oWidget.pElement;



/*
 *


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

    if (sTag.compare("slideritem", Qt::CaseInsensitive) == 0) {


        QComboItem* pItem =  new QComboItem("", "");

        pItem->setValue( sValue );
        QString sData = this->getAttribute(pElement, "value", sValue);
        pItem->setData(sData);

        pWidget = pItem;
    }


    if (sTag.compare("slider", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QSlider* pSlider = new QSlider(Qt::Orientation::Horizontal);
        pSlider->setFocusPolicy(Qt::StrongFocus);
        pSlider->setTickPosition(QSlider::TicksBelow);

        if (pElement->hasChildNodes())
        {

            std::vector<std::pair<QString, QString> > vValues;

            QDomNodeList oChildren = pElement->childNodes();
            for (size_t i = 0; i < oChildren.size(); ++i)
            {
                QDomElement oChildNode = oChildren.at(i).toElement();

                QString sValue = this->getAttribute(&oChildNode, "value", "");
                QString sDisplay = this->getAttribute(&oChildNode, "display", sValue);

                vValues.push_back( std::pair<QString, QString>(sValue, sDisplay) );
            }

            pSlider->setRange(1, vValues.size());

            QGridLayout* pGLayout = new QGridLayout();
            pGLayout->addWidget(pSlider, 0,0,1, vValues.size(), Qt::AlignCenter);

            for (size_t i = 0; i < vValues.size(); ++i)
            {

                QLabel* pLabel = new QLabel( vValues.at(i).second);
                pGLayout->addWidget( pLabel, 1, i, 1, 1, Qt::AlignCenter );

            }

            this->addValueFetcher(pElement, [vValues, pSlider] () {

                return vValues.at(pSlider->value()-1).first.toStdString();

            });

        } else {

            float fMin = this->getAttribute(pElement, "min", "0.0").toFloat();
            float fMax = this->getAttribute(pElement, "max", "1.0").toFloat();
            float fStep = this->getAttribute(pElement, "step", "0.1").toFloat();

            int iMinFactor  = Maths<float>::getIntegerFactor(fMin);
            int iMaxFactor  = Maths<float>::getIntegerFactor(fMax);
            int iStepFactor = Maths<float>::getIntegerFactor(fStep);

            const int iFactor = std::max(std::max(iMaxFactor, iMinFactor), iStepFactor);

            int iMin = fMin * iFactor;
            int iMax = fMax * iFactor;
            int iStep = fStep * iFactor;

            std::cerr << "Slider: " << iMin << " " << iMax << " " << iStep << std::endl;

            pSlider->setRange(iMin, iMax);
            pSlider->setSingleStep( iStep );


            int iInterval = (iMax-iMin) / 20;
            pSlider->setTickInterval( iInterval );
            pSlider->setPageStep(iInterval);



            this->addValueFetcher(pElement, [iFactor, pSlider] () {

                return std::to_string(pSlider->value() / iFactor);

            });

            pSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            //Assemble consturct

            std::stringstream oMinSS;
            oMinSS << std::setprecision( std::ceil(log10(fMin) + log10(iMinFactor))+1 ) << fMin;

            std::stringstream oMaxSS;
            oMaxSS << std::setprecision( std::ceil(log10(fMax) + log10(iMaxFactor))+1 ) << fMax;


            QWidget* pSliderWidget = new QWidget();
            QLabel* pLeftSliderValue  = new QLabel( QString( oMinSS.str().c_str() ) );
            QLabel* pRightSliderValue = new QLabel( QString( oMaxSS.str().c_str() ) );

            QLineEdit *pLineEdit = new QLineEdit( "0" );
            pLineEdit->setValidator( new QDoubleValidator() );

            pLineEdit->setMaxLength( std::ceil(log10(fMax) + log10(iMaxFactor) + log10(fStep) + log10(iStepFactor))+1 );
            pLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

            this->connect(pSlider, &QSlider::valueChanged, [pLineEdit, iFactor, pSlider] () {

                float fValue = (float) pSlider->value() / (float) iFactor;

                std::stringstream oValSS;
                int iValueFactor = Maths<float>::getIntegerFactor(fValue);
                oValSS << std::setprecision( std::ceil(log10(fValue) + log10(iValueFactor))+1 ) << fValue;

                pLineEdit->setText( QString(oValSS.str().c_str()) );

            });

            this->connect(pLineEdit, &QLineEdit::textChanged, [iFactor, pLineEdit, pSlider] () {

                int iPosition = pLineEdit->text().toFloat() * iFactor;

                pSlider->setValue(iPosition);
            });

            this->addValueFetcher(pElement, [pLineEdit] () {return pLineEdit->text().toStdString();});

            QLayout* pLayout = new QHBoxLayout();
            pLayout->addWidget(pLeftSliderValue);
            pLayout->addWidget(pSlider);
            pLayout->addWidget(pRightSliderValue);
            pLayout->addWidget(pLineEdit);

            pSliderWidget->setLayout(pLayout);

            pWidget = pSliderWidget;

        }





    }

    if (sTag.compare("filelist", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QComboBox *pComboBox = new QComboBox();

        QString sCurrentPath = QDir::current().absolutePath();
        QString sSearchPath = this->getAttribute(pElement, "path", sCurrentPath + "/install_templates/");

        bool bHasPathSet = this->hasAttribute(pElement, "path");

        if (bHasPathSet)
        {

            QFileInfo oSetPath(sSearchPath);

            if (oSetPath.isRelative())
            {
                std::cerr << "Changed path from " << sSearchPath.toStdString();
                sSearchPath = sCurrentPath + "/" + sSearchPath;

                std::cerr << " to " << sSearchPath.toStdString() << std::endl;
            }

        }

        QStringList vFileEnding = this->getAttribute(pElement, "ext", "*.igui").split(",");

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
            QString sFileName = oFileInfo.completeBaseName();

            //if this is an install template, now only show filename but read first line ...
            if (!bHasPathSet)
            {
                QFile oFile( sFoundFile );
                oFile.open(QFile::ReadOnly);
                QTextStream in(&oFile);
                QString sFirstLine = in.readLine();

                if (sFirstLine.startsWith("#!"))
                {

                    QString sName = sFirstLine.right(sFirstLine.size()-2);

                    sName = sName.trimmed();

                    sFileName = sName;
                }


            }

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

        //apply sizes here

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

    */
}
