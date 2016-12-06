//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONNODE_H
#define BIOGUI_EXECUTIONNODE_H

#include <QDomElement>
#include <QWidget>
#include <vector>
#include <map>
#include <iostream>
#include <src/Logging.h>
#include <src/Validable.h>

class QBufferTcpServer;

/*
class ExecutionNodeException : public std::exception
{
public:

    ExecutionNodeException(std::string sMessage)
        : std::exception(), m_sMessage(sMessage)
    {

    }

    char const* what() const throw()
    {
        return m_sMessage.c_str();
    }

protected:

    const std::string m_sMessage;

};
*/

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

    std::vector<std::string> getAcceptedAttributes()
    {
        std::vector<std::string> vReturnAttribs;

        vReturnAttribs.push_back("ID");
        vReturnAttribs.push_back("TYPE");

        this->addNodeAttributes(vReturnAttribs);

        std::sort(vReturnAttribs.begin(), vReturnAttribs.end());

        return vReturnAttribs;
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
            {
                LOGLVL("Duplicate node ids: " + m_sID, Logging::ERROR);
            }

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
                sReturn = sReturn + m_sSeperator;
            }

            sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

        }

        return sReturn;
    }

    virtual ~ExecutionNode()
    {

    }

    virtual void finishedCreation()
    {

    }

    void setSeperator(std::string sNewSep)
    {
        m_sSeperator = sNewSep;
    }

protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes) = 0;

    Validable<std::string> evaluateID( std::string sID, std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        Validable<std::string> oReturn("", false);

        std::map<std::string, std::string>::iterator oIt = pInputID2Value->find( sID );

        // either the id is an input field
        if (oIt != pInputID2Value->end())
        {
            oReturn.setValue(oIt->second);
        }

        // or it also might be another node
        std::map<std::string, ExecutionNode*>::iterator oJt = pID2Node->find( sID );

        // either the id is an input field
        if (oJt != pID2Node->end())
        {

            std::string sValue = oJt->second->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
            oReturn.setValue(sValue);
        }

        if (!oReturn.valid())
            LOGLVL("Duplicate node ids: " + m_sID, Logging::ERROR);

        return oReturn;

    }

    Validable<std::string> getNodeValue(std::string sID, std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {
        Validable<std::string> oReturn("", false);

        std::map<std::string, std::string>::iterator oIt = pInputID2Value->find( sID );

        // either the id is an input field
        if (oIt != pInputID2Value->end())
        {
            oReturn.setValue(oIt->second);
        }

        // or it also might be another node
        std::map<std::string, ExecutionNode*>::iterator oJt = pID2Node->find( sID );

        if (oJt != pID2Node->end())
        {
            std::string sValue = oJt->second->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
            oReturn.setValue(sValue);
        }

        if (!oReturn.valid())
            LOGLVL("id not found: " + m_sID, Logging::ERROR);

        return oReturn;
    }

    std::string getNodeValueOrValue(std::string sValue, std::string sDefaultValue,
                                std::map< std::string, ExecutionNode*>* pID2Node,
                                std::map<std::string, std::string>* pInputID2Value,
                                std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::string sSearchValue = sValue;

        if ((sValue.size() > 3) &&  (sValue[0] == '$') && (sValue[1] == '{') && (sValue[sValue.size()-1] == '}')) {
            sSearchValue = sValue.substr(2, sValue.size() - 3);
        }

        // if it is a node value, fetch it here!
        Validable<std::string> oRetValue = this->evaluateID(sSearchValue, pID2Node, pInputID2Value, pInputID2Widget);

        if (oRetValue.valid())
        {
            return oRetValue.value();
        } else {
            return sDefaultValue;
        }

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

    std::string m_sSeperator = ",";


};


#endif //BIOGUI_EXECUTIONNODE_H
