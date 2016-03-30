//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_XMLPARSEREXECUTION_H
#define BIOGUI_XMLPARSEREXECUTION_H


#include "nodes/ExecutionValueNode.h"
#include "nodes/ExecutionConstNode.h"
#include "nodes/ExecutionAddNode.h"
#include "nodes/ExecutionExecuteNode.h"
#include "nodes/ExecutionNode.h"

#include "nodes/ExecutionNode.h"
#include "nodes/ExecutionNetwork.h"
#include "XMLParser.h"

class XMLParserExecution : public XMLParser {

    Q_OBJECT

public:

    XMLParserExecution(std::string sFileName)
    : XMLParser(sFileName)
    {

        m_pDocument = loadFromFile(sFileName);


        m_pKnownTags->push_back("executable");
        m_pKnownTags->push_back("add");
        m_pKnownTags->push_back("value");
        m_pKnownTags->push_back("execute");
        m_pKnownTags->push_back("const");

    }

    ExecutionNetwork * getExecutionNetwork();

protected:


    ExecutionNode* createExecutionNode(QDomElement* pElement)
    {
        ExecutionNode* pReturn = NULL;

        QString sTagName = pElement->tagName();

        if ( sTagName.compare("executable", Qt::CaseInsensitive)==0 )
        {

            ExecutionNode* pNode = new ExecutionNode();

            pReturn = pNode;

        }

        if ( sTagName.compare("execute", Qt::CaseInsensitive)==0 )
        {

            ExecutionExecuteNode* pNode = new ExecutionExecuteNode();

            pReturn = pNode;

        }

        if ( sTagName.compare("const", Qt::CaseInsensitive)==0 )
        {

            ExecutionExecuteNode* pNode = new ExecutionExecuteNode();

            pReturn = pNode;

        }

        if ( sTagName.compare("value", Qt::CaseInsensitive)==0 )
        {

            ExecutionExecuteNode* pNode = new ExecutionExecuteNode();

            pReturn = pNode;

        }

        if ( sTagName.compare("add", Qt::CaseInsensitive)==0 )
        {

            ExecutionExecuteNode* pNode = new ExecutionExecuteNode();

            pReturn = pNode;

        }


        return pReturn;
    }


    QDomElement * getRoot();

    ExecutionNetwork * createNetwork(QDomElement* pElement);

    ExecutionNode * getExecutionNodes(QDomElement* pElement );

};


#endif //BIOGUI_XMLPARSEREXECUTION_H
