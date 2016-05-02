//
// Created by joppich on 3/31/16.
//

#include "ExecutionRunThread.h"

#include "../parsing/XMLParserWindow.h"
#include "../parsing/nodes/ExecutionNetwork.h"

ExecutionRunThread::ExecutionRunThread(XMLParserWindow* pWindowParser, ExecutionNetwork* pNetwork) {
    m_pNetwork = pNetwork;
    m_pWindowParser = pWindowParser;
}


void ExecutionRunThread::startExecution()
{
    m_pNetwork->execute( m_pWindowParser->getID2Value(), m_pWindowParser->getID2Widget() );

    emit executionFinished();

}
