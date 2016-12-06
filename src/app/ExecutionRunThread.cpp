//
// Created by joppich on 3/31/16.
//

#include "ExecutionRunThread.h"

#include "../parsing/XMLParserWindow.h"
#include "../parsing/XMLParserExecution.h"
#include "../parsing/nodes/ExecutionNetwork.h"

ExecutionRunThread::ExecutionRunThread(XMLParserWindow* pWindowParser, XMLParserExecution* pExecution, std::string& sProgramToRun) {

    m_pExecutionParser = pExecution;
    m_pWindowParser = pWindowParser;

    m_pNetwork = m_pExecutionParser->getExecutionNetwork();

    m_sProgramToRun = sProgramToRun;
}

ExecutionRunThread::~ExecutionRunThread()
{

    m_pNetwork->deleteLater();
    m_pExecutionParser->deleteLater();

}


void ExecutionRunThread::startExecution()
{

    std::cerr << "Executing program: " << m_sProgramToRun << std::endl;

    if (m_pNetwork == NULL)
    {
        LOGERROR("Error in execution program "+m_sProgramToRun+"! Unable to execute.");
    }

    QObject::connect(m_pNetwork, &ExecutionNetwork::executionFinished, this, &ExecutionRunThread::executionFinished);

    m_pWindowParser->printRetrieverIDs();

    m_pNetwork->setMaps( m_pWindowParser->getID2Value(), m_pWindowParser->getID2Widget() );
    int iExitCode = m_pNetwork->execute( m_sProgramToRun );

    switch (iExitCode)
    {

        case -1:
        {
            std::string sMessage = "No Program with Name " + m_sProgramToRun + " found!";

            QMessageBox oMsgBox;
            oMsgBox.setIcon( QMessageBox::Critical );
            oMsgBox.setText( QString(sMessage.c_str())  );
            oMsgBox.exec();
        }
            break;

        default:

            break;

    }


}
