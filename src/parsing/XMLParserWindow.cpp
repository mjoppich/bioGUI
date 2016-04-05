//
// Created by joppich on 3/29/16.
//

#include <src/app/QComboItem.h>
#include <src/app/AdvancedStreamBox.h>
#include "XMLParserWindow.h"

#include "../bioGUIapp.h"

QLayout* XMLParserWindow::createLayout(QDomElement* pElement)
{
    QString sTag = pElement->tagName();


    if (sTag.compare("hgroup", Qt::CaseInsensitive) == 0)
    {
        QHBoxLayout *pLayout = new QHBoxLayout();

        return (QLayout*) pLayout;
    }

    if (sTag.compare("vgroup", Qt::CaseInsensitive) == 0)
    {
        QVBoxLayout *pLayout = new QVBoxLayout();
        return (QLayout*) pLayout;
    }

    return NULL;
}

QWidget* XMLParserWindow::createComponent(QDomElement* pElement, bool* pChildrenFinished)
{

    QString sTag = pElement->tagName();
    QString sValue = pElement->text();

    std::cout << "Tag " << sTag.toStdString() << " Value " << sValue.toStdString() << std::endl;

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

        pWidget->setFixedSize(iWidth, iHeight);

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

    if (sTag.compare("input", Qt::CaseInsensitive) == 0)
    {
        QLineEdit *pLineEdit = new QLineEdit( sValue );

        this->addValueFetcher(pElement, [pLineEdit] () {return pLineEdit->text().toStdString();});

        pWidget = pLineEdit;
    }


    if (sTag.compare("filedialog", Qt::CaseInsensitive) == 0)
    {

        QWidget* pLineButton = new QWidget();
        QHBoxLayout* pLayout = new QHBoxLayout();


        QLineEdit* pLineEdit = new QLineEdit();
        pLineEdit->setToolTip("this is a tooltip\nwith line break!");

        QPushButton* pFileButton = new QPushButton(sValue);


        pFileButton->connect(pFileButton,&QAbstractButton::clicked,[pLineEdit] (bool bChecked){

            QString sFileName = QFileDialog::getOpenFileName(0, "Open XML File 2", QDir::homePath(), "XML Files (*.xml)");

            pLineEdit->setText(sFileName);
        });

        pLayout->addWidget( pLineEdit );
        pLayout->addWidget( pFileButton );
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

        QRadioButton* pRadioButton = new QRadioButton(sValue);
        std::string sCheckedValue = this->getAttribute(pElement, "checked_value", "true").toStdString();
        std::string sUncheckedValue = this->getAttribute(pElement, "unchecked_value", "false").toStdString();

        this->addValueFetcher(pElement, [pRadioButton, sCheckedValue, sUncheckedValue] () {

            if (pRadioButton->isChecked())
            {
                return sCheckedValue;
            } else {
                return sUncheckedValue;
            }

        });

        pWidget = pRadioButton;

    }

    if (sTag.compare("checkbox", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QCheckBox *pCheckBox = new QCheckBox( sValue );

        std::string sCheckedValue = this->getAttribute(pElement, "checked_value", "true").toStdString();
        std::string sUncheckedValue = this->getAttribute(pElement, "unchecked_value", "false").toStdString();

        this->addValueFetcher(pElement, [pCheckBox, sCheckedValue, sUncheckedValue] () {

            if (pCheckBox->isChecked())
            {
                return sCheckedValue;
            } else {
                return sUncheckedValue;
            }

        });

        pWidget = pCheckBox;

    }

    bool bBoolean = true;

    if (sTag.compare("group", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QGroupBox* pGroupBox = new QGroupBox();

        /*
         * Window Title
         */
        QString sTitle = this->getAttribute(pElement, "title", "bioGUI");
        pGroupBox->setTitle( sTitle );

        /*
         * checkable?
         */

        std::string sCheckable = this->getAttribute(pElement, "checkable", "false").toUpper().toStdString();
        if (sCheckable.compare("TRUE") == 0)
        {
            pGroupBox->setCheckable(true);

            std::string sCheckedValue = this->getAttribute(pElement, "checked_value", "true").toStdString();
            std::string sUncheckedValue = this->getAttribute(pElement, "unchecked_value", "false").toStdString();

            this->addValueFetcher(pElement, [pGroupBox, sCheckedValue, sUncheckedValue] () {

                if (pGroupBox->isChecked())
                {
                    return sCheckedValue;
                } else {
                    return sUncheckedValue;
                }

            });

        }


        // this could also be a hboxlayout or a grid layout
        QLayout* pLayout = new QVBoxLayout();

        QDomNodeList oChildren = pElement->childNodes();
        for (size_t i = 0; i < oChildren.size(); ++i)
        {
            QDomElement oChildNode = oChildren.at(i).toElement();
            QWidget* pChildElement = createComponent(&oChildNode, &bBoolean);

            if (pElement == NULL)
                throw "error in creating groupbox components";

            if (QAbstractButton* pButton = dynamic_cast<QAbstractButton *>(pChildElement))
            {

                if (i == 0)
                    pButton->setChecked(true);
                else
                    pButton->setChecked(false);

            }


            pLayout->addWidget( pChildElement );
        }

        pGroupBox->setLayout(pLayout);

        pWidget = pGroupBox;
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

            }
        }

        this->addValueFetcher(pElement, [pComboBox] () {

            return pComboBox->currentData().toString().toStdString();

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


        QString sID = this->getAttribute(pElement, "id", "");
        if (sID.length() > 0)
        {

            m_pID2Widget->insert( std::pair<std::string, QWidget*>(sID.toStdString(), pWidget));

        }

    }

    return pWidget;
}