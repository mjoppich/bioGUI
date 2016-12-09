//
// Created by joppich on 10/14/16.
//

#ifndef BIOGUI_EXECUTIONFILENODE_H
#define BIOGUI_EXECUTIONFILENODE_H


#include <QFile>
#include <QTextStream>
#include <fstream>
#include "ExecutionNode.h"

class ExecutionFileNode : public ExecutionNode {

public:

    ExecutionFileNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {

        m_sAction = this->getDomElementAttribute(pElement, "action", "").toStdString();


        m_sFrom = this->getDomElementAttribute(pElement, "FROM", "").toStdString();
        m_sSeperator = this->getDomElementAttribute(pElement, "sep", "").toStdString();
        m_sTo = this->getDomElementAttribute(pElement, "to", "").toStdString();

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        if ( QString("PARSE").compare(QString(m_sAction.c_str()), Qt::CaseInsensitive) == 0 )
        {

            if (!file_exists(m_sFrom))
            {
                LOGERROR("File " + m_sFrom + " does not exist."", Logging::ERROR);");
                return "";
            }

            return this->readFile(&m_sFrom, &m_sSeperator);

        } else if ( QString("SAVE").compare(QString(m_sAction.c_str()), Qt::CaseInsensitive) == 0 )
        {

            if (m_sTo.size() == 0)
            {
                LOGERROR("File Node with no given to attribute");
                return "";
            }

            std::map<std::string, std::string>::iterator oIt = pInputID2Value->find( m_sFrom );

            // either the id is an input field
            if (oIt != pInputID2Value->end())
            {
                std::string sReturn = oIt->second;

                QFile oFile(QString(m_sTo.c_str()));
                if (oFile.open(QIODevice::ReadWrite)) {
                    QTextStream stream(&oFile);
                    stream << QString(sReturn.c_str()) << endl;
                }

                return m_sTo;
            }

            // or it also might be another node
            std::map<std::string, ExecutionNode*>::iterator oJt = pID2Node->find( m_sFrom );

            // either the id is an input field
            if (oJt != pID2Node->end())
            {

                std::string sReturn = oJt->second->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

                QFile oFile(QString(m_sTo.c_str()));
                if (oFile.open(QIODevice::ReadWrite)) {
                    QTextStream stream(&oFile);
                    stream << QString(sReturn.c_str()) << endl;
                }

                return m_sTo;
            }


        } else if ( QString("DELETE").compare(QString(m_sAction.c_str()), Qt::CaseInsensitive) == 0 ) {

            std::string sNodeValue = this->getNodeValueOrValue(m_sFrom, m_sFrom, pID2Node, pInputID2Value, pInputID2Widget);

            QFile oDeleteFile(QString(sNodeValue.c_str()));

            bool bRemoved = oDeleteFile.remove();

        }


        return "";
    }



    static bool file_exists (const std::string& name) {
        if (FILE *file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        } else {
            return false;
        }
    }

    virtual ~ExecutionFileNode()
    {

    }

protected:




    std::string readFile(std::string* pFilename, std::string* pSep) {

        std::ifstream oInputStream;
        std::string sOutput, sLine;
        oInputStream.open(pFilename->c_str());

        if (oInputStream.eof() == true)
            return "";

        sOutput = "";

        while (!oInputStream.eof()) {

            std::getline(oInputStream, sLine);

            if (sLine.length() == 0)
                continue;

            if (sOutput.length() != 0)
                sOutput.append(*pSep);

            sOutput.append(sLine);

        }

        return sOutput;

    }

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("FROM");
        vAttributes.push_back("SEP");
        vAttributes.push_back("TO");

    }


    std::string m_sFrom;
    std::string m_sTo;
    std::string m_sAction;


};



#endif //BIOGUI_EXECUTIONFILENODE_H
