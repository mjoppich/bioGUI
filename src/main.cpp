#include <iostream>

#include <QtCore>
#include <QDebug>
#include <QtGui>
#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QXmlQuery>

#include <QMainWindow>
#include <QDomElement>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QPushButton>
#include "bioGUIapp.h"


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

int main(int argc, char *argv[])
{

    bioGUIapp* pApp = new bioGUIapp(argc, argv);

    pApp->exec();

    exit(0);

}