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

#ifndef BIOGUI_EXECUTIONNODE_H
#define BIOGUI_EXECUTIONNODE_H

#include <src/Logging.h>


#include <QDomElement>
#include <QObject>
#include <QWidget>
#include <vector>
#include <map>
#include <iostream>
#include <src/Validable.h>
#include <src/parsing/visual_nodes/WidgetFunctionNode.h>
#include <sstream>

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

class ExecutionNode : public QObject {

public:

    ExecutionNode()
        : QObject(NULL)
    {

    }

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


        this->m_sTag = "";

        if (pElement != NULL)
        {
            m_sTag = pElement->tagName().toStdString();
        }

    }


    virtual std::vector<std::string> outputs() {

        std::string sID = this->getID();

        std::vector<std::string> vReturn;
        vReturn.push_back(sID);

        return vReturn;
    }

    std::vector<ExecutionNode*> getChildren()
    {
        return m_vChildren;
    }


    virtual std::vector<std::string> inputs() = 0;

    bool onWindows()
    {
        return (QSysInfo::windowsVersion() != QSysInfo::WV_None);
    }

    bool checkWSL(QString& sWSLStr,
                  std::map< std::string, ExecutionNode*>* pID2Node,
                  std::map<std::string, std::string>* pInputID2Value,
                  std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget )
    {
        /*
         * Are we relocating for WSL?
         *
         * */
        bool bWSL = false;
        if (sWSLStr.size() > 0)
        {

            std::string sValue = this->getNodeValueOrValue(sWSLStr.toStdString(), sWSLStr.toStdString(), pID2Node, pInputID2Value, pInputID2FunctionWidget );


            // relocation must be demanded, and we must be on windows
            if ((QString(sValue.c_str()).compare("TRUE", Qt::CaseInsensitive) == 0) && this->onWindows())
            {
                bWSL = true;
            }
        }

        return bWSL;
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

    virtual std::string getID()
    {

        if (m_sID.size() > 0)
            return m_sID;

        const void * address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        std::string name = ss.str();

        return name;
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
                                  std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget) = 0;


    virtual NODE_TYPE getType()
    {
        return m_eType;
    }

    bool hasID();

    void addChild(ExecutionNode* pNode);

    void addChildren(std::vector<ExecutionNode*>::iterator oItStart, std::vector<ExecutionNode*>::iterator oItEnd );

    void getNodeMap( std::map< std::string, ExecutionNode*>* pID2Node)
    {

        std::string sID = this->getID();

        if (sID.size() != 0)
        {
            std::map< std::string, ExecutionNode*>::iterator oIt = pID2Node->find( sID );

            if (oIt != pID2Node->end())
            {
                LOGERROR("Duplicate node ids: " + sID);
            }

            pID2Node->insert( std::pair<std::string, ExecutionNode*>(sID, this) );
        }

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {
            m_vChildren.at(i)->getNodeMap(pID2Node);
        }


    }

    std::string sIDFromVarRef(std::string sVarRef)
    {
        std::string sModID = sVarRef;
        if ((sModID.find("${") == 0) and (sModID.at(sModID.size()-1) == '}'))
        {
            sModID = sModID.substr(2, sModID.size()-3);
        }

        return sModID;
    }

    virtual std::string evaluateChildren( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {
        std::string sReturn = "";

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (i > 0)
            {
                sReturn = sReturn + m_sSeperator;
            }

            sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget);

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

    virtual std::string parseDynamicValues( std::string& sArgs, std::map< std::string, ExecutionNode*>* pID2Node,
                           std::map<std::string, std::string>* pInputID2Value,
                           std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget )
    {
        std::string sCLArg = "";

        if (sArgs.size() > 0)
        {

            if (sArgs.find("${") == std::string::npos)
            {

                std::map< std::string, ExecutionNode*>::iterator oCLNode = pID2Node->find(sArgs);

                if (oCLNode != pID2Node->end())
                {
                    ExecutionNode* pParamNode = oCLNode->second;
                    sCLArg = pParamNode->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget);
                } else {
                    sCLArg = sArgs;
                }

            } else {

                sCLArg = std::string(sArgs);
                this->parseCommand(&sCLArg, 0, pID2Node, pInputID2Value, pInputID2FunctionWidget);

            }

        }

        return sCLArg;
    }

    virtual std::vector<std::string> referencedInputs( std::string& sArgs, std::map< std::string, ExecutionNode*>* pID2Node,
                                            std::map<std::string, std::string>* pInputID2Value,
                                            std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget )
    {

        std::vector<std::string> refIDs;

        if (sArgs.find("${") == std::string::npos)
        {
            return refIDs;
        }

        this->parseCommand(&sArgs, 0, pID2Node, pInputID2Value, pInputID2FunctionWidget, &refIDs);

        return refIDs;


    }



    virtual size_t parseCommand(std::string* pCommand, size_t iPos, std::map< std::string, ExecutionNode*>* pID2Node,
                        std::map<std::string, std::string>* pInputID2Value,
                        std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget,
                        std::vector<std::string>* pUsedIDs=NULL)
    {

        size_t iVarPos;
        size_t iStartPos = iPos;

        // { prog  2>&amp;1 1>&amp;3 3>&amp;- | netcat ${ip} 55026; } 3>&amp;1 1>&amp;2 | netcat ${ip} 55025

        while( ( iVarPos = std::min(pCommand->find("${", iPos), pCommand->find("}", iPos))  ) != std::string::npos )
        {
            if (( pCommand->at(iVarPos) == '}' ) && (iStartPos != -1))
            {

                std::string sPrefix = pCommand->substr(0, iStartPos-2);
                std::string sSuffix = pCommand->substr(iVarPos+1, pCommand->size());
                std::string sID = pCommand->substr(iStartPos, iVarPos-iStartPos);

                if (pUsedIDs)
                {

                    std::string sModID = this->sIDFromVarRef(sID);

                    pUsedIDs->push_back(sModID);
                }

                std::string sValue = this->getNodeValueOrValue(sID, "__NO_VALUE", pID2Node, pInputID2Value, pInputID2FunctionWidget);

                if (sValue.compare("__NO_VALUE") != 0)
                {
                    pCommand->clear();
                    pCommand->append(sPrefix);

                    //std::string sValue = this->evaluateID(sID, pID2Node, pInputID2Value, pInputID2Widget);

                    //std::cout << "id " << sID << " value " << sValue << std::endl;
                    pCommand->append(sValue);
                    pCommand->append(sSuffix);
                }

                // uncommented because I might want to replace variables in variables :)
                iPos = sPrefix.size() + sValue.size();

                iStartPos = -1;


            } else {
                iPos = this->parseCommand(pCommand, iVarPos+2, pID2Node, pInputID2Value, pInputID2FunctionWidget, pUsedIDs);
            }
        }


        return iVarPos;


    }

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes) = 0;

    Validable<std::string> evaluateID( std::string sID, std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {

        Validable<std::string> oReturn("", false);


        if (pInputID2Value)
        {
            std::map<std::string, std::string>::iterator oIt = pInputID2Value->find( sID );

            // either the id is an input field
            if (oIt != pInputID2Value->end())
            {
                oReturn.setValue(oIt->second);
            }
        }


        if (pID2Node)
        {
            // or it also might be another node
            std::map<std::string, ExecutionNode*>::iterator oJt = pID2Node->find( sID );

            // either the id is an input field
            if (oJt != pID2Node->end())
            {

                std::string sValue = oJt->second->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget);
                oReturn.setValue(sValue);
            }
        }


        if (!oReturn.valid())
        {
            //LOGERROR("In node: "+ m_sID + " : Neither node nor node value: " + sID);
        }

        return oReturn;

    }

    Validable<std::string> getNodeValue(std::string sID, std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {
        Validable<std::string> oReturn("", false);


        if ((sID.size() > 3) &&  (sID[0] == '$') && (sID[1] == '{') && (sID[sID.size()-1] == '}')) {
            sID = sID.substr(2, sID.size() - 3);
        }

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
            std::string sValue = oJt->second->evaluate(pID2Node, pInputID2Value, pInputID2FunctionWidget);
            oReturn.setValue(sValue);
        }

        if (!oReturn.valid())
        {
            LOGERROR("In node: "+ m_sID + " : id not found: " + sID);
        }

        return oReturn;
    }

    std::string getNodeValueOrValue(std::string sValue, std::string sDefaultValue,
                                std::map< std::string, ExecutionNode*>* pID2Node,
                                std::map<std::string, std::string>* pInputID2Value,
                                std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {

        std::string sSearchValue = sValue;

        if ((sValue.size() > 3) &&  (sValue[0] == '$') && (sValue[1] == '{') && (sValue[sValue.size()-1] == '}')) {
            sSearchValue = sValue.substr(2, sValue.size() - 3);
        }

        // if it is a node value, fetch it here!
        Validable<std::string> oRetValue = this->evaluateID(sSearchValue, pID2Node, pInputID2Value, pInputID2FunctionWidget);

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
    bool hasDomElementAttribute(QDomElement* pElement, QString sAttribName);


    std::vector<ExecutionNode*> m_vChildren;

    std::string m_sID;
    NODE_TYPE m_eType;
    std::string m_sValue;
    std::string m_sTag;

    std::string m_sSeperator = ",";


};


#endif //BIOGUI_EXECUTIONNODE_H
