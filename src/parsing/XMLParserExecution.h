//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_XMLPARSEREXECUTION_H
#define BIOGUI_XMLPARSEREXECUTION_H


#include <src/parsing/nodes/ExecutionMathNode.h>
#include "nodes/ExecutionValueNode.h"
#include "nodes/ExecutionConstNode.h"
#include "nodes/ExecutionAddNode.h"
#include "nodes/ExecutionExecuteNode.h"
#include "nodes/ExecutionOutputNode.h"

#include "nodes/ExecutionNode.h"
#include "nodes/ExecutionNetwork.h"
#include "XMLParser.h"

class XMLParserExecution : public XMLParser {

    Q_OBJECT

public:

    XMLParserExecution(std::string sFileName)
    : XMLParser(sFileName)
    {
        m_pKnownTags->push_back("add");
        m_pKnownTags->push_back("value");
        m_pKnownTags->push_back("execute");
        m_pKnownTags->push_back("const");
        m_pKnownTags->push_back("math");
        m_pKnownTags->push_back("output");
        m_pKnownTags->push_back("execution");

        m_pDocument = loadFromFile(sFileName);


    }

    ExecutionNetwork * getExecutionNetwork();

protected:


    ExecutionNode* createExecutionNode(QDomElement* pElement)
    {
        ExecutionNode* pReturn = NULL;

        QString sTagName = pElement->tagName();

        if ( sTagName.compare("execute", Qt::CaseInsensitive)==0 )
        {

            ExecutionExecuteNode* pNode = new ExecutionExecuteNode( pElement );

            pReturn = pNode;

        }

        if ( sTagName.compare("const", Qt::CaseInsensitive)==0 )
        {

            ExecutionConstNode* pNode = new ExecutionConstNode( pElement );

            pReturn = pNode;

        }

        if ( sTagName.compare("value", Qt::CaseInsensitive)==0 )
        {

            ExecutionValueNode* pNode = new ExecutionValueNode( pElement );

            pReturn = pNode;

        }

        if ( sTagName.compare("add", Qt::CaseInsensitive)==0 )
        {

            ExecutionAddNode* pNode = new ExecutionAddNode( pElement );

            pReturn = pNode;

        }

        if ( sTagName.compare("math", Qt::CaseInsensitive)==0 )
        {

            ExecutionMathNode* pNode = new ExecutionMathNode( pElement );

            pReturn = pNode;

        }

        if ( sTagName.compare("output", Qt::CaseInsensitive)==0 )
        {

            ExecutionOutputNode* pNode = new ExecutionOutputNode( pElement );

            pReturn = pNode;

        }

        if (pReturn == NULL)
        {

            std::cout << sTagName.toStdString() << std::endl;
            std::cout << sTagName.toStdString() << std::endl;

        }


        return pReturn;
    }


    QDomElement * getRoot(QDomDocument* pDocument);

    ExecutionNetwork * createNetwork(QDomElement* pElement);

    ExecutionNode * getExecutionNodes(QDomElement* pElement );

};


#endif //BIOGUI_XMLPARSEREXECUTION_H
