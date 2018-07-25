/*
 *  bioGUI - a new universal GUI for bioinformatic applications
 *  Copyright (C) 2016  Markus Joppich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ExecutionRunThread.h"

#include "../parsing/XMLParserWindow.h"
#include "../parsing/XMLParserExecution.h"
#include "../parsing/nodes/ExecutionNetwork.h"
#include "../bioGUIapp.h"

ExecutionRunThread::ExecutionRunThread(
        XMLParserWindow* pWindowParser,
        XMLParserExecution* pExecution,
        std::string& sProgramToRun,
        bioGUIapp* pApp) {

    m_pExecutionParser = pExecution;
    m_pWindowParser = pWindowParser;

    m_pNetwork = m_pExecutionParser->getExecutionNetwork();

    m_sProgramToRun = sProgramToRun;

    m_pApp = pApp;
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
    m_pNetwork->setApp(this->m_pApp);

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
