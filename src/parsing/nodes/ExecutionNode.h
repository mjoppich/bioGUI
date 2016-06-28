//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONNODE_H
#define BIOGUI_EXECUTIONNODE_H

#include <QDomElement>
#include <QWidget>
#include <vector>
#include <map>

class QBufferTcpServer;

class ExecutionNode {

public:

    enum NODE_TYPE {

        STRING = 0,
        NUMBER = 1

    };

    struct sMainThreadInfo {
        QBufferTcpServer* pTCPserver;
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

        this->m_sTag = pElement->tagName().toStdString();

    }

    std::string getID()
    {
        return m_sID;
    }

    std::string getTag()
    {
        return m_sTag;
    }

    QString getQTag()
    {
        return QString(m_sTag.c_str());
    }



    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, QWidget*>* pInputID2Widget) = 0;


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


    virtual std::string evaluateChildren( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, QWidget*>* pInputID2Widget)
    {
        std::string sReturn = "";

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (i > 0)
            {
                sReturn = sReturn + "\n";
            }

            sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

        }

        return sReturn;
    }



    virtual void finishedCreation()
    {

    }

protected:

    std::string evaluateID( std::string sID, std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::map<std::string, std::string>::iterator oIt = pInputID2Value->find( sID );

        // either the id is an input field
        if (oIt != pInputID2Value->end())
        {
            return oIt->second;
        }

        // or it also might be another node
        std::map<std::string, ExecutionNode*>::iterator oJt = pID2Node->find( sID );

        // either the id is an input field
        if (oJt != pID2Node->end())
        {
            return oJt->second->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
        }

        throw "Node ID not found: " + sID;

    }

    std::string getNodeValueOrValue(std::string sValue, std::string sDefaultValue,
                                std::map< std::string, ExecutionNode*>* pID2Node,
                                std::map<std::string, std::string>* pInputID2Value,
                                std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::string sReturn;

        try
        {
            // if it is a node value, fetch it here!
            sReturn = this->evaluateID(sValue, pID2Node, pInputID2Value, pInputID2Widget);
        } catch (...)
        {
            sReturn = sDefaultValue;
        }

        return sReturn;

    }

    bool asBool(std::string sValue)
    {
        return (QString(sValue.c_str()).compare("true", Qt::CaseInsensitive) == 0);
    }

    QString getDomElementAttribute(QDomElement* pElement, QString sAttribName, QString sDefault);

    std::vector<ExecutionNode*> m_vChildren;

    std::string m_sID;
    NODE_TYPE m_eType;
    std::string m_sValue;
    std::string m_sTag;


};


#endif //BIOGUI_EXECUTIONNODE_H
