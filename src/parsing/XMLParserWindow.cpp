//
// Created by joppich on 3/29/16.
//

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
        pWidget = pLabel;
    }

    if (sTag.compare("input", Qt::CaseInsensitive) == 0)
    {
        QLineEdit *pLineEdit = new QLineEdit( sValue );
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

        pWidget = pLineButton;
    }

    if (sTag.compare("action", Qt::CaseInsensitive) == 0)
    {
        QPushButton *pAction = new QPushButton( sValue );

        bioGUIapp* pApp = this->m_pApp;

        pAction->connect(pAction,&QAbstractButton::clicked,[pApp] (bool bChecked){


            pApp->runProgram();

        });

        pWidget = pAction;
    }


    if (sTag.compare("radiobutton", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QRadioButton* pRadioButton = new QRadioButton(sValue);

        pWidget = pRadioButton;

    }

    if (sTag.compare("checkbox", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QCheckBox *pCheckBox = new QCheckBox( sValue );

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


        // this could also be a hboxlayout or a grid layout
        QLayout* pLayout = new QVBoxLayout();

        QDomNodeList oChildren = pElement->childNodes();
        for (size_t i = 0; i < oChildren.size(); ++i)
        {
            QDomElement oChildNode = oChildren.at(i).toElement();
            QWidget* pElement = createComponent(&oChildNode, &bBoolean);

            if (pElement == NULL)
                throw "error in creating groupbox components";

            if (QAbstractButton* pButton = dynamic_cast<QAbstractButton *>(pElement))
            {

                if (i == 0)
                    pButton->setChecked(true);
                else
                    pButton->setChecked(false);

            }


            pLayout->addWidget( pElement );
        }

        pGroupBox->setLayout(pLayout);

        pWidget = pGroupBox;
    }


    if (sTag.compare("combobox", Qt::CaseInsensitive) == 0)
    {

        (*pChildrenFinished) = true;

        QLabel *pLabel = new QLabel("combobox");


        pWidget = pLabel;

    }


    // some general things

    if (pWidget != NULL)
    {

        QString sToolTip = this->getAttribute(pElement, "hint", "");
        if (sToolTip.length() > 0)
        {
            pWidget->setToolTip( sToolTip );
        }


        QString sID = this->getAttribute(pElement, "id", "");
        if (sID.length() > 0)
        {

            throw "not implemented";

        }

    }

    return pWidget;
}