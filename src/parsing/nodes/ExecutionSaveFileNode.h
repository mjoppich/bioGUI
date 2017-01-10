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

#ifndef BIOGUI_EXECUTIONSAVEFILENODE_H
#define BIOGUI_EXECUTIONSAVEFILENODE_H

#include <QFile>
#include <QTextStream>
#include "ExecutionNode.h"

/*
class ExecutionSaveFileNode : public ExecutionNode {

public:

    ExecutionSaveFileNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {


        m_sFrom = this->getDomElementAttribute(pElement, "FROM", "").toStdString();
        m_sTo = this->getDomElementAttribute(pElement, "to", "").toStdString();

        if (m_sTo.size() == 0)
        {
            throw "value node without from " + pElement->toCDATASection().nodeValue().toStdString();
        }

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

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

        return "";
    }

protected:


    std::string m_sFrom;
    std::string m_sTo;

};
*/

#endif //BIOGUI_EXECUTIONSAVEFILENODE_H
