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

#include "ExecutionExecuteNode.h"
#include <src/app/ProcessLauncher.h>

std::string ExecutionExecuteNode::evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                      std::map<std::string, std::string>* pInputID2Value,
                      std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget, bool bEmitSignal)
{

    std::string sCLArg = this->parseDynamicValues(m_sCLArg, pID2Node, pInputID2Value, pInputID2FunctionWidget);
    std::string sLocation = this->parseDynamicValues(m_sExecLocation, pID2Node, pInputID2Value, pInputID2FunctionWidget);

    std::string sProgram = sLocation + m_sExecutable;

    bool bWSL = this->asBool(this->getNodeValueOrValue(m_sWSL, m_sWSL, pID2Node, pInputID2Value, pInputID2FunctionWidget));
    bWSL = bWSL && this->onWindows();

    ProcessLauncher* pLauncher = new ProcessLauncher(QString(sProgram.c_str()), QString(sCLArg.c_str()), bWSL, m_pApp);

    this->evaluateChildren(pID2Node, pInputID2Value, pInputID2FunctionWidget, pLauncher->getProcess(), NULL, false); //

    pLauncher->connect(pLauncher, &ProcessLauncher::finished,
            [pLauncher, pID2Node, pInputID2Value, pInputID2FunctionWidget, bEmitSignal, this](){

                this->evaluateChildren(pID2Node, pInputID2Value, pInputID2FunctionWidget, pLauncher->getProcess(), NULL, true);
                pLauncher->deleteLater();

                // calculation finished!
                if (bEmitSignal == true)
                    emit finishedExecution();

            });


    pLauncher->start( );

    return "";

}
