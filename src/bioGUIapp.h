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
#include "parsing/XMLParserExecution.h"
#include "parsing/XMLParserWindow.h"
#include "app/ExecutionRunThread.h"

class bioGUIapp : public QApplication {
    Q_OBJECT
public:

    bioGUIapp(int argc, char* argv[])
    : QApplication(argc, argv)
    {

        m_sGUIFile = "/cygdrive/c/libraries/bioGUI/example.gui";
        m_sGUIFile = "/home/users/joppich/cpp/bioGUI/example.gui";

        //m_pWindowParser = new XMLParserWindow( this, "/home/users/joppich/cpp/bioGUI/example.gui" );

        m_pWindowParser = new XMLParserWindow( this, m_sGUIFile);

        m_pWindow = m_pWindowParser->getWindow();
        m_pWindow->show();

        // For testing purposes only
        this->runProgram();

    }

    void enableActions()
    {
        m_pWindowParser->setActionsEnabled(true);
    }

    void disableActions()
    {
        m_pWindowParser->setActionsEnabled(false);
    }




    void runProgram()
    {

        this->disableActions();


        //XMLParserExecution oParseExecution("/home/users/joppich/cpp/bioGUI/example.gui" );
        XMLParserExecution oParseExecution( m_sGUIFile );

        ExecutionNetwork* pNetwork = oParseExecution.getExecutionNetwork();

        ExecutionRunThread* pThread = new ExecutionRunThread(m_pWindowParser, pNetwork);

        this->connect(pThread, &QThread::finished, this, &bioGUIapp::programFinished);

        pThread->start();


    }

    void programFinished()
    {

        this->enableActions();

    }

protected:

    QWidget* m_pWindow;
    XMLParserWindow* m_pWindowParser;

    std::string m_sGUIFile;


};


#endif //BIOGUI_BIOGUIAPP_H
