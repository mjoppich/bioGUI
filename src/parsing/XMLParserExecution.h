//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_XMLPARSEREXECUTION_H
#define BIOGUI_XMLPARSEREXECUTION_H

#include <src/parsing/nodes/ExecutionSaveFileNode.h>
#include <src/parsing/nodes/ExecutionStringReplaceNode.h>
#include <src/parsing/nodes/ExecutionPathRelocateNode.h>
#include <src/parsing/nodes/ExecutionHTTPExecuteNode.h>
#include "nodes/ExecutionOrderedAddNode.h"
#include "nodes/ExecutionPlaceholderNode.h"
#include "nodes/ExecutionIfNode.h"
#include "nodes/ExecutionMathNode.h"
#include "nodes/ExecutionValueNode.h"
#include "nodes/ExecutionConstNode.h"
#include "nodes/ExecutionAddNode.h"
#include "nodes/ExecutionExecuteNode.h"
#include "nodes/ExecutionOutputNode.h"
#include "nodes/ExecutionEnvNode.h"
#include "nodes/ExecutionNode.h"
#include "nodes/ExecutionNetwork.h"
#include "XMLParser.h"

class XMLParserExecution : public XMLParser {

    Q_OBJECT

public:

    XMLParserExecution(std::string sFileName)
    : XMLParser(sFileName)
    {

        this->insertNodeType("execute", [] (QDomElement* pElement) {
            ExecutionExecuteNode* pNode = new ExecutionExecuteNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("httpexecute", [] (QDomElement* pElement) {
            ExecutionHTTPExecuteNode* pNode = new ExecutionHTTPExecuteNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("savefile", [] (QDomElement* pElement) {
            ExecutionSaveFileNode* pNode = new ExecutionSaveFileNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("const", [] (QDomElement* pElement) {
            ExecutionConstNode* pNode = new ExecutionConstNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("value", [] (QDomElement* pElement) {
            ExecutionValueNode* pNode = new ExecutionValueNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("add", [] (QDomElement* pElement) {
            ExecutionAddNode* pNode = new ExecutionAddNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("replace", [] (QDomElement* pElement) {
            ExecutionStringReplaceNode* pNode = new ExecutionStringReplaceNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("orderedadd", [] (QDomElement* pElement) {
            ExecutionOrderedAddNode* pNode = new ExecutionOrderedAddNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("math", [] (QDomElement* pElement) {
            ExecutionMathNode* pNode = new ExecutionMathNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("output", [] (QDomElement* pElement) {
            ExecutionOutputNode* pNode = new ExecutionOutputNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("if", [] (QDomElement* pElement) {
            ExecutionIfNode* pNode = new ExecutionIfNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("else", [] (QDomElement* pElement) {
            ExecutionPlaceholderNode* pNode = new ExecutionPlaceholderNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("relocate", [] (QDomElement* pElement) {
            ExecutionPathRelocateNode* pNode = new ExecutionPathRelocateNode( pElement );
            return (ExecutionNode*) pNode;
        });

        this->insertNodeType("env", [] (QDomElement* pElement) {
            ExecutionEnvNode* pNode = new ExecutionEnvNode( pElement );
            return (ExecutionNode*) pNode;
        });



        if (m_pKnownTags != NULL)
            delete m_pKnownTags;

        m_pKnownTags = this->getKnownTags();

        m_pDocument = loadFromFile(sFileName);


    }

    ~XMLParserExecution()
    {

        for (size_t i = 0; i < m_vCreatedNodes.size(); ++i)
        {
            delete m_vCreatedNodes.at(i);
        }

    }

    ExecutionNetwork * getExecutionNetwork();

protected:

    void insertNodeType(std::string sType, std::function< ExecutionNode* ( QDomElement* )> oFunc)
    {
        std::pair<std::string, std::function< ExecutionNode*( QDomElement*)> > oPair;

        oPair = std::pair<std::string, std::function< ExecutionNode*( QDomElement*)>>( sType, oFunc);
        m_mCreateNodeMap.insert(oPair);

    }


    std::vector<std::string>* getKnownTags()
    {
        std::vector<std::string>* pTags = new std::vector<std::string>();

        std::map<std::string, std::function< ExecutionNode*( QDomElement*)> >::iterator oIt = m_mCreateNodeMap.begin();

        while (oIt != m_mCreateNodeMap.end())
        {
            pTags->push_back( oIt->first );
            ++oIt;
        }


        // manually add master node!
        pTags->push_back("execution");

        return pTags;
    }

    ExecutionNode* createNode(QDomElement* pElement, std::string sNodeType)
    {

        std::map<std::string, std::function< ExecutionNode*( QDomElement*)> >::iterator oIt = m_mCreateNodeMap.find( sNodeType );

        if (oIt != m_mCreateNodeMap.end())
        {
            return oIt->second( pElement );
        } else {
            return NULL;
        }


    }

    ExecutionNode* createExecutionNode(QDomElement* pElement)
    {


        QString sTagName = pElement->tagName();

        ExecutionNode* pReturn = this->createNode(pElement, sTagName.toStdString());

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


    std::map< std::string, std::function< ExecutionNode*( QDomElement*)> > m_mCreateNodeMap;
    std::vector<ExecutionNode*> m_vCreatedNodes;

};


#endif //BIOGUI_XMLPARSEREXECUTION_H
