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
        m_bHasWSLAttrib = this->hasDomElementAttribute(pElement, "wsl");

    }

    virtual std::vector<std::string> inputs()
    {
        std::vector<std::string> vBase;
        vBase.push_back(m_sFrom);
        vBase.push_back(m_sTo);

        return vBase;
    }

    virtual ~ExecutionPathRelocateNode()
    {

    }

    static std::string relocateWSL(std::string sPaths, std::string sSeperator)
    {

        std::string sEmpty = "";
        std::string sWSLPrefix = "/mnt/";

        return relocateGeneral(sPaths, sEmpty, sEmpty, sWSLPrefix, sSeperator, true);
    }

    static std::string relocateGeneral(std::string& sPaths, std::string& sFrom, std::string& sTo, std::string& sPrepend, std::string& sSeperator, bool bMakeUnix)
    {
        QString qsChildren(sPaths.c_str());

        QStringList vChildren = qsChildren.split(',');
        QStringList vOutput;

        for (size_t i = 0; i < vChildren.size(); ++i)
        {

            QString sChild = vChildren.at(i);

            if (sTo.size() > 0)
            {
                // replace from with to
                if (sChild.startsWith(QString(sFrom.c_str()), Qt::CaseInsensitive))
                {

                    sChild.remove(0, sFrom.size());
                    sChild.prepend(QString(sTo.c_str()));

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

            if (sPrepend.size() > 0)
                sChild.prepend( QString(sPrepend.c_str()) );

            vOutput.append(sChild);

        }

        std::string sReturn = vOutput.join( QString(sSeperator.c_str()) ).toStdString();

        std::cerr << "relocated from " << sPaths << " to " << sReturn << std::endl;

        return sReturn;
    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {

        std::string sChildren = "";

        bool bMakeUnix = m_bMakeUnix;

        bool bWSL = this->checkWSL(m_sToWSL, pID2Node, pInputID2Value, pInputID2FunctionWidget);

        /*What should be relocated?
         * */
        if (m_sFrom.size() == 0)
        {
            sChildren = this->evaluateChildren(pID2Node, pInputID2Value, pInputID2FunctionWidget);
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

                sChildren = this->getNodeValueOrValue(sDynID, sDynID, pID2Node, pInputID2Value, pInputID2FunctionWidget);
            } else {
                sChildren = m_sFrom;
            }
        }

        if (sChildren.size() == 0)
            return sChildren;

        if ((m_bHasWSLAttrib) && (bWSL == false))
        {
            return sChildren;
        }

        if (bWSL)
        {
            bMakeUnix = true;
        }

        std::string sStart = sChildren;

        if (bWSL)
        {
            return relocateWSL(sChildren, m_sSeperator);
        } else {
            return relocateGeneral(sChildren, m_sFrom, m_sTo, m_sPrepend, m_sSeperator, bMakeUnix);
        }

/*
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


        return sReturn;
        */

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
    bool m_bHasWSLAttrib = true;
    QString m_sToWSL = "";


};


#endif //BIOGUI_EXECUTIONWSLTRANSFORMNODE_H
