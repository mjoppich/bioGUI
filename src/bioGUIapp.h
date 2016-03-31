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
#include <src/parsing/XMLParserExecution.h>
#include "parsing/XMLParserWindow.h"

class bioGUIapp : public QApplication {
    Q_OBJECT
public:

    bioGUIapp(int argc, char* argv[])
    : QApplication(argc, argv)
    {


        XMLParserWindow oParseWindow("/home/users/joppich/cpp/bioGUI/example.gui" );

        m_pWindow = oParseWindow.getWindow();
        m_pWindow->show();

        XMLParserExecution oParseExecution("/home/users/joppich/cpp/bioGUI/example.gui" );
        ExecutionNetwork* pNetwork = oParseExecution.getExecutionNetwork();

        pNetwork->execute( oParseWindow.getID2Value() );

    }

    void runProgram()
    {

    }

protected:

    QWidget* m_pWindow;


};


#endif //BIOGUI_BIOGUIAPP_H
