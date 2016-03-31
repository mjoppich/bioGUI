//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONNODE_H
#define BIOGUI_EXECUTIONNODE_H

#include <QDomElement>
#include <vector>
#include <map>

class ExecutionNode {

public:

    enum NODE_TYPE {

        STRING = 0,
        NUMBER = 1

    };

    static NODE_TYPE cstring2nodetype(std::string sType)
    {

        if (sType.compare("STRING") == 0)
            return NODE_TYPE::STRING;

        if (sType.compare("NUMBER") == 0)
            return NODE_TYPE::NUMBER;

        return NODE_TYPE::STRING;
    }

    ExecutionNode(QDomElement* pElement)
    {

        QString qsID = this->getDomElementAttribute(pElement, "ID", "");
        this->m_sID = qsID.toStdString();

        QString qsType = this->getDomElementAttribute(pElement, "TYPE", "STRING");
        this->m_eType = ExecutionNode::cstring2nodetype( qsType.toUpper().toStdString() );

    }

    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node, std::map<std::string, std::string>* pInputID2Value ) = 0;


    virtual NODE_TYPE getType()
    {
        return m_eType;
    }

    bool hasID();

    void addChild(ExecutionNode* pNode);

    void addChildren(std::vector<ExecutionNode*>::iterator oItStart, std::vector<ExecutionNode*>::iterator oItEnd );

    void getNodeMap( std::map< std::string, ExecutionNode*>* pID2Node)
    {

        if (m_sID.size() != 0)
        {
            std::map< std::string, ExecutionNode*>::iterator oIt = pID2Node->find( m_sID );

            if (oIt != pID2Node->end())
                throw "duplicate node ids : " + m_sID;

            pID2Node->insert( std::pair<std::string, ExecutionNode*>(m_sID, this) );
        }

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {
            m_vChildren.at(i)->getNodeMap(pID2Node);
        }


    }

protected:


    QString getDomElementAttribute(QDomElement* pElement, QString sAttribName, QString sDefault);

    std::vector<ExecutionNode*> m_vChildren;

    std::string m_sID;
    NODE_TYPE m_eType;
    std::string m_sValue;


};


#endif //BIOGUI_EXECUTIONNODE_H
