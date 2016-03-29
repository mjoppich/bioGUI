#include <iostream>

#include <QtCore>
#include <QtXML>
#include <QDebug>
#include <QtGui>
#include <QWidget>
#include <QLabel.h>
#include <QApplication>
#include <QXmlQuery>

#include <QMainWindow>
#include <QDomElement>
#include <QHBoxLayout>
#include <QVBoxLayout>


void retrievElements(QDomElement root, QString tag, QString att)
{
    QDomNodeList nodes = root.elementsByTagName(tag);

    qDebug() << "# nodes = " << nodes.count();
    for(int i = 0; i < nodes.count(); i++)
    {
        QDomNode elm = nodes.at(i);
        if(elm.isElement())
        {
            QDomElement e = elm.toElement();
            qDebug() << e.attribute(att);
        }
    }
}

void printAllElements(QDomElement* pElement)
{

    qDebug() << pElement->tagName() << " child of " << pElement->parentNode().toElement().tagName() << " " << pElement->isElement();


    QDomNodeList oChildren = pElement->childNodes();

    for (size_t i = 0; i < oChildren.length(); ++i)
    {

        QDomElement oChild = oChildren.item(i).toElement();

        printAllElements( &oChild );

    }


}

enum ELEMENT_TYPE {ELEMENT, LAYOUT};

ELEMENT_TYPE getElementType(QDomElement* pElement)
{
    QString sTag = pElement->tagName();


    if (sTag.compare("hgroup", Qt::CaseInsensitive) == 0)
    {
        return ELEMENT_TYPE::LAYOUT;
    }

    if (sTag.compare("vgroup", Qt::CaseInsensitive) == 0)
    {
        return ELEMENT_TYPE::LAYOUT;
    }

    return ELEMENT_TYPE::ELEMENT;

}


QLayout* createLayout(QDomElement* pElement)
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

QWidget* createComponent(QDomElement* pElement)
{

    QString sTag = pElement->tagName();

    if (sTag.compare("window", Qt::CaseInsensitive) == 0)
    {
        QWidget *pWindow = new QWidget();
        return pWindow;
    }

    if (sTag.compare("label", Qt::CaseInsensitive) == 0)
    {
        QLabel *pLabel = new QLabel("label");
        return pLabel;
    }

    if (sTag.compare("input", Qt::CaseInsensitive) == 0)
    {
        QLabel *pLabel = new QLabel("input");
        return pLabel;
    }

    if (sTag.compare("action", Qt::CaseInsensitive) == 0)
    {
        QLabel *pLabel = new QLabel("action");
        return pLabel;
    }
    return NULL;
}

QWidget* createComponents(QWidget* pParentWidget, QDomElement* pElement, bool* pIsElement)
{

    if (pElement->isElement() == false)
    {
        qDebug() << pElement->tagName() << " child of " << pElement->parentNode().toElement().tagName() << " " << pElement->isElement();
        return NULL;
    }

    (*pIsElement) = (ELEMENT_TYPE::ELEMENT == getElementType(pElement));

    if (*pIsElement)
    {
        qDebug() << pElement->tagName() << " is an element";
    } else {
        qDebug() << pElement->tagName() << " is not an element";
    }

    void* pParent = NULL;

    if (*pIsElement)
    {
        pParent = (void*) createComponent(pElement);
    } else {

        if (pParentWidget == NULL)
        {
            qDebug() << "invalid null parent widget";
        }

        pParent = (void*) createLayout(pElement);

        if (pParentWidget != NULL)
            pParentWidget->setLayout( (QBoxLayout*) pParent);

    }


    QDomNodeList oChildren = pElement->childNodes();

    for (size_t i = 0; i < oChildren.length(); ++i)
    {

        QDomElement oChild = oChildren.item(i).toElement();

        if (oChild.isElement() == false)
        {
            continue;
        }

        if (*pIsElement)
        {
            bool bNotNeeded = false;
            createComponents((QWidget*)pParent, &oChild, &bNotNeeded);
        } else {

            bool bChildIsElement = false;
            QWidget* pReturn = createComponents( NULL, &oChild, &bChildIsElement);

            if (bChildIsElement == false)
            {
                ((QBoxLayout *) pParent)->addLayout( (QLayout*) pReturn);

            } else {
                ((QBoxLayout *) pParent)->addWidget( (QWidget*) pReturn);
            }

        }
    }

    return (QWidget*)pParent;

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create a document to write XML
    QDomDocument document;

    // Open a file for reading
    QFile file("/cygdrive/c/libraries/bioGUI/example.gui");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open the file for reading.";
        return -1;
    }
    else
    {
        // loading
        if(!document.setContent(&file))
        {
            qDebug() << "Failed to load the file for reading.";
            return -1;
        }
        file.close();
    }


    QDomNodeList oChildren = document.elementsByTagName("window");
    QDomElement oRoot;

    for (size_t i = 0; i < oChildren.length(); ++i)
    {

        QDomElement oChild = oChildren.item(i).toElement();

        printAllElements( &oChild );

        if (oChild.toElement().tagName().compare("window", Qt::CaseInsensitive) == 0)
        {
            oRoot = oChild;
        }

    }


    bool bUnimportant;
    QMainWindow* pWindow = (QMainWindow*)createComponents(NULL, &oRoot, &bUnimportant);

    pWindow->setFixedSize(400,400);
    pWindow->show();
    pWindow->setWindowTitle("bla");

    a.exec();

    exit(0);

}