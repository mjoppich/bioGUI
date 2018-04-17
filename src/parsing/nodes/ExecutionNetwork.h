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

#ifndef BIOGUI_EXECUTIONNETWORK_H
#define BIOGUI_EXECUTIONNETWORK_H

#include <set>
#include <map>
#include <functional>
#include <QObject>
#include <src/parsing/visual_nodes/WidgetFunctionNode.h>



class QWidget;
class ExecutionNode;

class ExecutionNetwork : public QObject{
 Q_OBJECT
public:

    ExecutionNetwork(std::map< std::string, std::function< std::string () > >* pID2Value,
                     std::map< std::string, WidgetFunctionNode* >* pInputID2FunctionWidget )
    {
        this->setMaps(pID2Value, pInputID2FunctionWidget);

    }

    ~ExecutionNetwork()
    {
        // those are variables of xmlparserwindow!
        //delete m_pID2Value;
        //delete m_pInputID2Widget;

    }

    void printNetworkAddNode(std::ofstream* pOut, const std::string sNodeID, std::string sNodeDesc, int iNodeType, std::set<std::string>* pContainedIDs);

    void printNetworkAddEdge(std::ofstream* pOut, const std::string sSource, const std::string sTarget, size_t iEdgeID);

    void printNetwork();

    void setMaps(std::map< std::string, std::function< std::string () > >* pID2Value,
                 std::map< std::string, WidgetFunctionNode* >* pInputID2FunctionWidget)
    {
        m_pID2Value = pID2Value;
        m_pInputID2FunctionWidget = pInputID2FunctionWidget;
    }

    int execute( std::string& sProgramToRun )
    {

        this->printNetwork();


        this->prepareAllNodeLists(&sProgramToRun);

        if (m_vExecNodes.size() == 0)
        {

            return -1;

        } else {
            return this->performExecution();
        }

    }

    void prepareAllNodeLists(std::string* pProgToRun);

    void setNodes(ExecutionNode* pNode)
    {
        m_vNodes.push_back(pNode);
    }

    void setNodes(std::vector<ExecutionNode*> vNodes)
    {
        m_vNodes.insert(m_vNodes.end(), vNodes.begin(), vNodes.end());
    }

    int performExecution();

signals:

    void executionFinished();

protected:


    // given from outside
    std::map< std::string, std::function< std::string () > >* m_pID2Value;
    std::map< std::string, WidgetFunctionNode* >* m_pInputID2FunctionWidget;


    // to be filled in execute
    std::map< std::string, std::string> mInputID2Value;
    std::map< std::string, ExecutionNode*> mID2Node;


    // for internal selection
    std::vector<ExecutionNode*> m_vNodes;
    std::vector<ExecutionNode*> m_vExecNodes;



};


#endif //BIOGUI_EXECUTIONNETWORK_H
