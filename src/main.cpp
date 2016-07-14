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

int main(int argc, char *argv[])
{

    bioGUIapp* pApp = new bioGUIapp(argc, argv);

    pApp->exec();

    delete pApp;

    return 0;
}
