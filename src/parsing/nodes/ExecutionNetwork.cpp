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

#include <sstream>
#include "ExecutionNetwork.h"

#include "ExecutionNode.h"
#include "ExecutionExecuteNode.h"
#include "ExecutionIfNode.h"

void ExecutionNetwork::printNetwork() {
    this->prepareAllNodeLists(NULL);

    std::ofstream myfile;
    myfile.open ("/home/mjoppich/win/Desktop/exec_network.graphml");


    myfile << R"(<?xml version="1.0" encoding="UTF-8"?>)" << std::endl;
    myfile << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"" << std::endl;
    myfile << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << std::endl;
    myfile << "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">" << std::endl;
    myfile << R"(<graph id="G" edgedefault="directed">)" << std::endl;

    myfile << "<key id=\"name\" for=\"all\" attr.name=\"name\" attr.type=\"string\"/>" << std::endl;
    myfile << "<key id=\"label\" for=\"all\" attr.name=\"label\" attr.type=\"string\"/>" << std::endl;
    myfile << "<key id=\"weight\" for=\"all\" attr.name=\"weight\" attr.type=\"double\"/>" << std::endl;
    myfile << "<key id=\"ranking\" for=\"node\" attr.name=\"ranking\" attr.type=\"int\"/>" << std::endl;
    myfile << "<key id=\"type\" for=\"node\" attr.name=\"type\" attr.type=\"string\"/>" << std::endl;
    myfile << "<key id=\"shape\" for=\"node\" attr.name=\"shape\" attr.type=\"string\"/>" << std::endl;
    myfile << "<key id=\"special\" for=\"node\" attr.name=\"special\" attr.type=\"boolean\"/>" << std::endl;
    myfile << "<key id=\"type\" for=\"edge\" attr.name=\"type\" attr.type=\"string\"/>" << std::endl;
    myfile << "<key id=\"interaction\" for=\"edge\" attr.name=\"interaction\" attr.type=\"string\">" << std::endl;
    myfile << "  <default>pp</default>" << std::endl;
    myfile << "</key>" << std::endl;
    myfile << "<key id=\"weight\" for=\"edge\" attr.name=\"weight\" attr.type=\"double\"/>" << std::endl;
    myfile << "<key id=\"sourceArrowShape\" for=\"edge\" attr.name=\"sourceArrowShape\" attr.type=\"string\"/>" << std::endl;
    myfile << "<key id=\"targetArrowShape\" for=\"edge\" attr.name=\"targetArrowShape\" attr.type=\"string\"/>" << std::endl;

    std::map<std::string, ExecutionNode* >::iterator oIt = mID2Node.begin();
    std::set<std::string> setSeenNodeIDs;

    size_t iAddedEdges = 0;

    for (; oIt != mID2Node.end(); ++oIt)
    {

        std::string sKey = oIt->first;
        ExecutionNode* pNode = oIt->second;

        if (sKey.size() == 0)
        {
            std::ostringstream address;
            address << (void const *)this;
            std::string name = address.str();

            sKey = name;
        }

        std::string sPetriAction = "petriact_" + pNode->getTag() + "_" + sKey;

        std::string sDescr = pNode->getTag() + "\n" + sKey;

        this->printNetworkAddNode(&myfile, sPetriAction, sDescr, 1, &setSeenNodeIDs);

        std::vector<std::string> vInputs = pNode->inputs();
        std::vector<std::string> vOutputs = pNode->outputs();

        if (vInputs.size() == 0)
        {
            vInputs = pNode->inputs();
        }

        for (int i = 0; i < vInputs.size(); ++i)
        {
            std::string sNodeName = pNode->sIDFromVarRef(vInputs.at(i));

            if (sNodeName.find("--folders") != -1)
            {
                sNodeName = sNodeName;
            }

            std::string sDescr = sNodeName;

            this->printNetworkAddNode(&myfile, sNodeName, sDescr, 0, &setSeenNodeIDs);
            this->printNetworkAddEdge(&myfile, sNodeName, sPetriAction, iAddedEdges++);
        }

        for (int i = 0; i < vOutputs.size(); ++i)
        {
            std::string sNodeName = pNode->sIDFromVarRef(vOutputs.at(i));

            std::string sDescr = sNodeName;


            this->printNetworkAddNode(&myfile, sNodeName, sDescr, 0, &setSeenNodeIDs);
            this->printNetworkAddEdge(&myfile, sPetriAction, sNodeName, iAddedEdges++);
        }


    }

    myfile << "</graph>" << std::endl;
    myfile << "</graphml>" << std::endl;

}

void ExecutionNetwork::printNetworkAddEdge(std::ofstream* pOut, const std::string sSource, const std::string sTarget, size_t iEdgeID) {
    (*pOut) << R"(<edge id=")" << iEdgeID << R"(" source=")" << sSource << R"(" target=")" << sTarget << R"("/>)" << std::endl;
}

void
ExecutionNetwork::printNetworkAddNode(std::ofstream* pOut, const std::string sNodeID, std::string sNodeDesc, int iNodeType, std::set<std::string> *pContainedIDs) {
    bool hasID = pContainedIDs->count(sNodeID) != 0;

    if (hasID)
        return;

    (*pOut) << R"(<node id=")" << sNodeID << R"(">)" << std::endl;


    if (iNodeType == 0)
    {
        // regular input/output
        (*pOut) << R"(<data key="shape">ELLIPSE</data>)" << std::endl;
    } else if (iNodeType == 1)
    {
        (*pOut) << R"(<data key="shape">Rectangle</data>)" << std::endl;
    }


    (*pOut) << R"(<data key="name">)"<< sNodeDesc << R"(</data>)" << std::endl;
    (*pOut) << R"(<data key="label">)"<< sNodeDesc << R"(</data>)" << std::endl;

    (*pOut) << R"(</node>)" << std::endl;
    pContainedIDs->insert(pContainedIDs->end(), sNodeID);
}

void ExecutionNetwork::prepareAllNodeLists(std::string *pProgToRun) {
    mInputID2Value.clear();
    mID2Node.clear();

    // fill inputid2value
    std::map<std::string, std::function< std::string () > >::iterator oIt = m_pID2Value->begin();

    for ( ; oIt != m_pID2Value->end(); ++oIt)
    {

        std::string sResult = oIt->second();
        //std::cout << "inputid2value " << oIt->first << " " << sResult << std::endl;
        mInputID2Value.insert( std::pair<std::string, std::string>( oIt->first, sResult ) );

    }

    std::vector< ExecutionNode*> vExecNodes;

    for (size_t i = 0; i < m_vNodes.size(); ++i)
    {

        ExecutionNode* pNode = m_vNodes.at(i);
        pNode->getNodeMap( &mID2Node );

        if (pProgToRun != NULL)
        {
            if (ExecutionExecuteNode* pExecNode = dynamic_cast<ExecutionExecuteNode*>( pNode ))
            {

                if ((pProgToRun->size() == 0) || (pProgToRun->compare( pExecNode->getProgramName() ) == 0))
                {
                    m_vExecNodes.push_back(pExecNode);
                }

            }

            if (ExecutionIfNode* pExecNode = dynamic_cast<ExecutionIfNode*>( pNode ))
            {

                if ((pProgToRun->size() == 0) || (pProgToRun->compare( pExecNode->getProgramName() ) == 0))
                {
                    m_vExecNodes.push_back(pExecNode);
                }

            }
        }
    }
}

int ExecutionNetwork::performExecution() {

    std::vector<ExecutionNode*>::iterator oIt = m_vExecNodes.begin();

    if (oIt == m_vExecNodes.end())
    {
        LOGERROR("EXECUTION FINISHED!")

        emit executionFinished();
        return 0;
    }

    ExecutionNode* pNode = *oIt;

    // this should never be called again :)
    m_vExecNodes.erase(oIt);

    if (ExecutionExecutableNode* pExecNode = dynamic_cast<ExecutionExecutableNode*>( pNode ))
    {
        pExecNode->setApp(this->m_pApp);

        // when node is finished, we start the next execution
        this->connect( pExecNode,&ExecutionExecutableNode::finishedExecution, this, &ExecutionNetwork::performExecution );

        std::cout << "evaluate " << pExecNode->getID() << std::endl;

        pExecNode->evaluate( &mID2Node, &mInputID2Value, m_pInputID2FunctionWidget, true );

    }

    return 0;

}

void ExecutionNetwork::setApp(bioGUIapp* pApp)
{
    this->m_pApp = pApp;
}
