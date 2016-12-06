//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONNETWORK_H
#define BIOGUI_EXECUTIONNETWORK_H


#include <functional>
#include "ExecutionNode.h"
#include "ExecutionExecuteNode.h"
#include "ExecutionIfNode.h"

class QWidget;

class ExecutionNetwork : public QObject{
 Q_OBJECT
public:

    ExecutionNetwork(std::map< std::string, std::function< std::string () > >* pID2Value,
                     std::map< std::string, QWidget* >* pInputID2Widget )
    {
        this->setMaps(pID2Value, pInputID2Widget);
    }

    ~ExecutionNetwork()
    {
        // those are variables of xmlparserwindow!
        //delete m_pID2Value;
        //delete m_pInputID2Widget;

    }

    void setMaps(std::map< std::string, std::function< std::string () > >* pID2Value,
                 std::map< std::string, QWidget* >* pInputID2Widget)
    {
        m_pID2Value = pID2Value;
        m_pInputID2Widget = pInputID2Widget;
    }

    int execute( std::string& sProgramToRun )
    {

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

            if (ExecutionExecuteNode* pExecNode = dynamic_cast<ExecutionExecuteNode*>( pNode ))
            {

                if ((sProgramToRun.size() == 0) || (sProgramToRun.compare( pExecNode->getProgramName() ) == 0))
                {
                    m_vExecNodes.push_back(pExecNode);
                }

            }

            if (ExecutionIfNode* pExecNode = dynamic_cast<ExecutionIfNode*>( pNode ))
            {

                if ((sProgramToRun.size() == 0) || (sProgramToRun.compare( pExecNode->getProgramName() ) == 0))
                {
                    m_vExecNodes.push_back(pExecNode);
                }

            }

        }

        if (m_vExecNodes.size() == 0)
        {

            return -1;

        } else {
            return this->performExecution();
        }

    }

    void setNodes(ExecutionNode* pNode)
    {
        m_vNodes.push_back(pNode);
    }

    void setNodes(std::vector<ExecutionNode*> vNodes)
    {
        m_vNodes.insert(m_vNodes.end(), vNodes.begin(), vNodes.end());
    }

    int performExecution()
    {

        std::vector<ExecutionNode*>::iterator oIt = m_vExecNodes.begin();

        if (oIt == m_vExecNodes.end())
        {
            emit executionFinished();
            return 0;
        }

        ExecutionNode* pNode = *oIt;

        // this should never be called again :)
        m_vExecNodes.erase(oIt);

        if (ExecutionExecutableNode* pExecNode = dynamic_cast<ExecutionExecutableNode*>( pNode ))
        {

            // when node is finished, we start the next execution
            this->connect( pExecNode,&ExecutionExecutableNode::finishedExecution, this, &ExecutionNetwork::performExecution );

            std::cout << "evaluate " << pExecNode->getID() << std::endl;

            pExecNode->evaluate( &mID2Node, &mInputID2Value, m_pInputID2Widget, true );

        }

        return 0;

    }

signals:

    void executionFinished();

protected:


    // given from outside
    std::map< std::string, std::function< std::string () > >* m_pID2Value;
    std::map< std::string, QWidget* >* m_pInputID2Widget;


    // to be filled in execute
    std::map< std::string, std::string> mInputID2Value;
    std::map< std::string, ExecutionNode*> mID2Node;


    // for internal selection
    std::vector<ExecutionNode*> m_vNodes;
    std::vector<ExecutionNode*> m_vExecNodes;



};


#endif //BIOGUI_EXECUTIONNETWORK_H
