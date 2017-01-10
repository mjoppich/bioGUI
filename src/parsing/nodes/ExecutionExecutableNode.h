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

#ifndef EXECUTIONEXECUTABLENODE_H
#define EXECUTIONEXECUTABLENODE_H

#include <QObject>
#include "ExecutionNode.h"

class ExecutionExecutableNode : public QObject, public ExecutionNode
{
    Q_OBJECT

public:

    ExecutionExecutableNode(QDomElement* pElement)
        : QObject(NULL), ExecutionNode(pElement)
    {
        m_sProgramName = this->getDomElementAttribute(pElement, "program", "").toStdString();
    }

    virtual ~ExecutionExecutableNode()
    {

    }

    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, QWidget*>* pInputID2Widget, bool bEmitSignal = false) = 0;

    virtual std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                                  std::map<std::string, std::string>* pInputID2Value,
                                  std::map<std::string, QWidget*>* pInputID2Widget)
    {
        return this->evaluate(pID2Node, pInputID2Value, pInputID2Widget, false);
    }

    std::string getProgramName()
    {
        return m_sProgramName;
    }

signals:

    void finishedExecution();


protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("PROGRAM");
    }

    std::string m_sProgramName = "";

};



#endif // EXECUTIONEXECUTABLENODE_H
