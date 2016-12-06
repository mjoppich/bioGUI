//
// Created by joppich on 5/18/16.
//

#ifndef BIOGUI_EXECUTIONPARSEFILENODE_H
#define BIOGUI_EXECUTIONPARSEFILENODE_H

#include <QFile>
#include <QTextStream>
#include <fstream>
#include "ExecutionNode.h"
/*
class ExecutionParseFileNode : public ExecutionNode {

public:

    ExecutionParseFileNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {


        m_sFrom = this->getDomElementAttribute(pElement, "FROM", "").toStdString();
        m_sSeperator = this->getDomElementAttribute(pElement, "sep", "").toStdString();


    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        if (!file_exists(m_sFrom))
        {
            throw "file does not exist: " + m_sFrom;
        }

        return this->readFile(&m_sFrom, &m_sSeperator);

    }

protected:

    inline bool file_exists (const std::string& name) {
        if (FILE *file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        } else {
            return false;
        }
    }


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



    std::string m_sFrom;


};

*/

#endif //BIOGUI_EXECUTIONPARSEFILENODE_H
