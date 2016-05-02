//
// Created by joppich on 3/30/16.
//

#ifndef BIOGUI_EXECUTIONEXECUTENODE_H
#define BIOGUI_EXECUTIONEXECUTENODE_H

#include <QDebug>
#include <QThread>
#include <QProcess>
#include <iostream>
#include <QtCore/qprocess.h>
#include "ExecutionNode.h"
#include "ExecutionOutputNode.h"

#ifdef WINDOWS
    #include <windows.h>
#endif

class ExecutionExecuteNode : public ExecutionNode {

public:

    ExecutionExecuteNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {

        std::string sNotSet = "#NOTSET#";

        m_sExecutable = this->getDomElementAttribute(pElement, "exec", QString(sNotSet.c_str())).toStdString();
        m_sExecLocation = this->getDomElementAttribute(pElement, "location", QString(sNotSet.c_str())).toStdString();
        m_sCLArg = this->getDomElementAttribute(pElement, "param", QString(sNotSet.c_str())).toStdString();

        m_bTest = (this->getDomElementAttribute(pElement, "test", "false").compare("True", Qt::CaseInsensitive) == 0);

        if (m_sExecutable.compare(sNotSet) == 0)
            throw "executable not set";

        if (m_sExecLocation.compare(sNotSet) == 0)
            throw "executable location not set";


    }

    virtual std::string evaluateChildren( std::map< std::string, ExecutionNode*>* pID2Node,
                                          std::map<std::string, std::string>* pInputID2Value,
                                          std::map<std::string, QWidget*>* pInputID2Widget,
                                          QProcess* pProcess,
                                          bool bDeferred = false)
    {
        std::string sReturn = "";

        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            if (bDeferred)
            {
                // only output nodes
                if (ExecutionOutputNode* pOutPutnode = dynamic_cast<ExecutionOutputNode*>( m_vChildren.at(i) ))
                {
                    sReturn = sReturn + pOutPutnode->evaluateDeferred(pID2Node, pInputID2Value, pInputID2Widget, pProcess, bDeferred);
                }
            } else {

                if (pProcess != NULL)
                {

                    if (ExecutionOutputNode* pOutPutnode = dynamic_cast<ExecutionOutputNode*>( m_vChildren.at(i) ))
                    {
                        sReturn = sReturn + pOutPutnode->evaluateDeferred(pID2Node, pInputID2Value, pInputID2Widget, pProcess, bDeferred);
                    } else {
                        sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

                    }

                } else {

                    sReturn = sReturn + m_vChildren.at(i)->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

                }



            }



        }

        return sReturn;
    }

    size_t parseCommand(std::string* pCommand, size_t iPos, std::map< std::string, ExecutionNode*>* pID2Node,
                        std::map<std::string, std::string>* pInputID2Value,
                        std::map<std::string, QWidget*>* pInputID2Widget)
    {

        size_t iVarPos;
        size_t iStartPos = iPos;

        while ( (iVarPos = pCommand->find("${", iPos)) != std::string::npos )
        {

            iPos = this->parseCommand(pCommand, iVarPos+2, pID2Node, pInputID2Value, pInputID2Widget);

        }

        size_t iVarEnd = pCommand->find("}", iPos);

        if (iVarEnd == std::string::npos)
        {
            iVarEnd = pCommand->size();
            return iVarEnd;
        }


        std::string sPrefix = pCommand->substr(0, iStartPos-2);
        std::string sSuffix = pCommand->substr(iVarEnd+1, pCommand->size());
        std::string sID = pCommand->substr(iStartPos, iVarEnd-iStartPos);

        pCommand->clear();
        pCommand->append(sPrefix);

        std::string sValue = this->evaluateID(sID, pID2Node, pInputID2Value, pInputID2Widget);


        std::cout << "id " << sID << " value " << sValue << std::endl;
        pCommand->append(sValue);
        size_t iReturn = pCommand->size();

        pCommand->append(sSuffix);


        return iReturn;


    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::string sCLArg = "";

        if (m_sCLArg.size() > 0)
        {

            if (m_sCLArg.find("${") == std::string::npos)
            {

                std::map< std::string, ExecutionNode*>::iterator oCLNode = pID2Node->find(m_sCLArg);

                if (oCLNode != pID2Node->end())
                {
                    ExecutionNode* pParamNode = oCLNode->second;
                    sCLArg = pParamNode->evaluate(pID2Node, pInputID2Value, pInputID2Widget);
                } else {
                    sCLArg = m_sCLArg;
                }

            } else {

                sCLArg = std::string(m_sCLArg);
                this->parseCommand(&sCLArg, 0, pID2Node, pInputID2Value, pInputID2Widget);

            }

        }

        bool bActuallyRun = true;
        int iReturnCode = 0;

        std::string sProgram = m_sExecLocation + m_sExecutable;
        std::string sModified(sProgram);

        if (m_bTest)
        {
            sProgram = "/usr/bin/echo";

        }


        QProcess* pProcess = new QProcess();

        this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, pProcess, false);

        pProcess->connect(pProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                [=](int exitCode, QProcess::ExitStatus exitStatus){

                    qDebug() << pProcess->error();
                    qDebug() << pProcess->program();
                    qDebug() << pProcess->arguments();

                    std::cout << "finished: " << iReturnCode << std::endl;

                    this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, pProcess, true);
                    delete pProcess;

                });


        bool bWSL = false;
        if (bWSL)
        {
            pProcess->setProcessChannelMode(QProcess::SeparateChannels);
        }

        /*
         *
         * this is totally WSL specific ... no idea how to integrate that

        if (true)
        {

            std::string sCMD = sProgram + " " + sCLArg;

            STARTUPINFO sinfo = {sizeof(sinfo), 0};
            PROCESS_INFORMATION pinfo = {0};

            if (CreateProcess(NULL, (LPSTR)sCMD.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sinfo, &pinfo)) {

                std::cerr << "executed: " << sCMD << std::endl;

                //WaitForSingleObject (pinfo.hProcess, INFINITE);

                std::cerr << "finished" << std::endl;


            } else {
              // boo

                return "";

            }


        }
        */

        if (bActuallyRun)
        {

            std::cout << "running " << sProgram << " " << sCLArg << std::endl;

            QStringList oArgs;
            if (sCLArg.size() > 0)
                oArgs = this->stringToArguments(QString(sCLArg.c_str()), '\"');

            for (int i = 0; i < oArgs.size(); ++i)
                std::cerr << oArgs.at(i).toStdString() << std::endl;

            pProcess->start( QString(sProgram.c_str()), oArgs );


        }

        return "";

    }

protected:

    QStringList stringToArguments(QString sString, char cQuoteChar)
    {
        bool bContainsQuotes = (sString.at(0).toLatin1() == cQuoteChar); //true if the first character is "

        QString sTest("a");
        sTest[0] = cQuoteChar;

        QStringList vTmpList = sString.split(QRegExp(sTest), QString::SkipEmptyParts); // Split by " and make sure you don't have an empty string at the beginning
        QStringList vArgsList;

                foreach (QString s, vTmpList) {
                if (bContainsQuotes) { // If 's' is inside quotes ...
                    vArgsList.append(s); // ... get the whole string
                } else { // If 's' is outside quotes ...
                    vArgsList.append(s.split(" ", QString::SkipEmptyParts)); // ... get the splitted string
                }
                bContainsQuotes = !bContainsQuotes;
            }

        return vArgsList;
    }

    std::string m_sExecutable;
    std::string m_sExecLocation;
    std::string m_sCLArg;

    bool m_bTest = false;

};


#endif //BIOGUI_EXECUTIONEXECUTENODE_H
