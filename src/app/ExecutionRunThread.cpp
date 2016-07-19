//
// Created by joppich on 3/31/16.
//

#include "ExecutionRunThread.h"

#include "../parsing/XMLParserWindow.h"
#include "../parsing/XMLParserExecution.h"
#include "../parsing/nodes/ExecutionNetwork.h"

ExecutionRunThread::ExecutionRunThread(XMLParserWindow* pWindowParser, XMLParserExecution* pExecution) {

    m_pExecutionParser = pExecution;
    m_pWindowParser = pWindowParser;

    m_pNetwork = m_pExecutionParser->getExecutionNetwork();
}

ExecutionRunThread::~ExecutionRunThread()
{

    m_pNetwork->deleteLater();
    m_pExecutionParser->deleteLater();

}


void ExecutionRunThread::startExecution()
{

    try
    {

        QObject::connect(m_pNetwork, &ExecutionNetwork::executionFinished, this, &ExecutionRunThread::executionFinished);

        m_pNetwork->setMaps( m_pWindowParser->getID2Value(), m_pWindowParser->getID2Widget() );
        m_pNetwork->execute();

    } catch (const char* e)
    {

        std::cerr << e << std::endl;

        QMessageBox oMsgBox;
        oMsgBox.setIcon( QMessageBox::Critical );
        oMsgBox.setText( QString(e) );
        oMsgBox.exec();

    } catch (const std::string& e)
    {
        std::cerr << e << std::endl;

        QMessageBox oMsgBox;
        oMsgBox.setIcon( QMessageBox::Critical );
        oMsgBox.setText( QString(e.c_str()) );
        oMsgBox.exec();
    }
}
