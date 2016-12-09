//
// Created by joppich on 4/25/16.
//

#ifndef BIOGUI_EXECUTIONWSLTRANSFORMNODE_H
#define BIOGUI_EXECUTIONWSLTRANSFORMNODE_H


#include <QFile>
#include <QTextStream>
#include <QtCore/qdir.h>
#include "ExecutionNode.h"
#include "ExecutionValueNode.h"
#include <iostream>

class ExecutionPathRelocateNode : public ExecutionNode {

public:

    ExecutionPathRelocateNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {

        m_sFrom = this->getDomElementAttribute(pElement, "from", "").toStdString();
        m_sTo = this->getDomElementAttribute(pElement, "to", "").toStdString();
        m_sPrepend = this->getDomElementAttribute(pElement, "prepend", "").toStdString();
        m_bMakeUnix = (this->getDomElementAttribute(pElement, "unix", "false").compare("TRUE", Qt::CaseInsensitive) == 0);

        m_sToWSL = this->getDomElementAttribute(pElement, "wsl", "false");

    }

    virtual ~ExecutionPathRelocateNode()
    {

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::string sChildren = "";

        bool bMakeUnix = m_bMakeUnix;

        /*
         * Are we relocating for WSL?
         *
         * */
        bool bWSL = false;

        if (m_sToWSL.size() > 0)
        {

            std::string sValue = this->getNodeValueOrValue(m_sToWSL.toStdString(), m_sToWSL.toStdString(), pID2Node, pInputID2Value, pInputID2Widget );

            if (QString(sValue.c_str()).compare("TRUE", Qt::CaseInsensitive) == 0)
            {
                bWSL = true;
            }
        }


        /*What should be relocated?
         * */
        if (m_sFrom.size() == 0)
        {
            sChildren = this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget);
        } else {

            if (m_sFrom[0] == '$' and m_sFrom[1] == '{' and m_sFrom[m_sFrom.size()-1] == '}')
            {
                std::string sDynID = "";

                if (bWSL)
                {
                    sDynID = m_sFrom;
                } else {
                    sDynID = m_sFrom.substr(2, m_sFrom.size()-3);
                }

                sChildren = this->getNodeValueOrValue(sDynID, sDynID, pID2Node, pInputID2Value, pInputID2Widget);
            } else {
                sChildren = m_sFrom;
            }
        }

        if (sChildren.size() == 0)
            return sChildren;

        if (bWSL)
        {
            bMakeUnix = true;
        }

        std::string sStart = sChildren;

        QString qsChildren(sChildren.c_str());

        QStringList vChildren = qsChildren.split(',');
        QStringList vOutput;

        for (size_t i = 0; i < vChildren.size(); ++i)
        {

            QString sChild = vChildren.at(i);

            if (m_sTo.size() > 0)
            {
                // replace from with to
                if (sChild.startsWith(QString(m_sFrom.c_str()), Qt::CaseInsensitive))
                {

                    sChild.remove(0, m_sFrom.size());
                    sChild.prepend(QString(m_sTo.c_str()));

                }

                // absolute path
                if ((sChild.size() > 1) && (sChild.at(1) != ':'))
                {
                    QDir oPath(sChild);
                    sChild = oPath.absolutePath();
                }
            }

            if (bMakeUnix)
            {
                // assumes that we have a windows string
                if (sChild.size() > 2)
                {
                    QChar cDrive = sChild.at(0);
                    sChild.remove(0,2);
                    sChild.prepend(QString("/") + cDrive.toLower());
                    }
                sChild.replace("\\", "/");

            }

            if (bWSL)
            {
                if (sChild.size() > 0)
                    sChild.prepend("/mnt/");
            }

            if (m_sPrepend.size() > 0)
                sChild.prepend( QString(m_sPrepend.c_str()) );

            vOutput.append(sChild);

        }

        std::string sReturn = vOutput.join( QString(m_sSeperator.c_str()) ).toStdString();

        std::cerr << "relocated from " << sChildren << " to " << sReturn << std::endl;

        return sReturn;

    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("FROM");
        vAttributes.push_back("TO");
        vAttributes.push_back("PREPEND");
        vAttributes.push_back("UNIX");
        vAttributes.push_back("WSL");

    }

    std::string m_sFrom;
    std::string m_sTo;
    std::string m_sPrepend;

    bool m_bMakeUnix = true;
    QString m_sToWSL = "";


};


#endif //BIOGUI_EXECUTIONWSLTRANSFORMNODE_H
