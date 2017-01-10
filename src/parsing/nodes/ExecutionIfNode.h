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

#ifndef BIOGUI_EXECUTIONIFNODE_H
#define BIOGUI_EXECUTIONIFNODE_H

#include <iostream>
#include "ExecutionNode.h"
#include "ExecutionExecutableNode.h"

class ExecutionIfNode : public ExecutionExecutableNode {

public:

    ExecutionIfNode(QDomElement* pElement)
            : ExecutionExecutableNode(pElement)
    {

        m_sNotSet = "#NOTSET#";

        m_sCompareMode = this->getDomElementAttribute(pElement, "comp", "is_set").toStdString();
        m_sValue1 = this->getDomElementAttribute(pElement, "value1", QString(m_sNotSet.c_str())).toStdString();
        m_sValue2 = this->getDomElementAttribute(pElement, "value2", QString(m_sNotSet.c_str())).toStdString();

        m_sSeperator = this->getDomElementAttribute(pElement, "sep", "").toStdString();


        m_vValidCompareModes.push_back("is_set");
        m_vValidCompareModes.push_back("equals");
        m_vValidCompareModes.push_back("EQUALS");

    }

    virtual ~ExecutionIfNode()
    {

    }

    virtual void finishedCreation()
    {
        // find else node, stop on first found node
        for (size_t i = 0; i < m_vChildren.size(); ++i)
        {

            ExecutionNode* pOtherNode = m_vChildren.at(i);

            if (pOtherNode->getQTag().compare("else", Qt::CaseInsensitive) == 0)
            {
                m_pElseNode = pOtherNode;
                break;
            }

        }
    }

    virtual std::string evaluateChildren( std::map< std::string, ExecutionNode*>* pID2Node,
                                          std::map<std::string, std::string>* pInputID2Value,
                                          std::map<std::string, QWidget*>* pInputID2Widget,
                                          bool bEmitSignal);


    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget, bool bEmitSignal = false)
    {

        if (!(std::find(m_vValidCompareModes.begin(), m_vValidCompareModes.end(), m_sCompareMode) != m_vValidCompareModes.end() ))
        {
            LOGERROR("unknown compare method " + m_sCompareMode);
        }

        QString sQCompareMode(m_sCompareMode.c_str());

        if (sQCompareMode.compare("is_set", Qt::CaseInsensitive) != 0)
        {

            if (m_sValue2.compare(m_sNotSet) == 0)
            {
                LOGERROR("value2 attribute must be set!");
                return "";
            }


        }

        if (m_sValue1.compare(m_sNotSet) == 0)
        {
            LOGERROR("value1 attribute must be set!");
            return "";
        }

        std::string sReturn = "";

        std::string sValue1 = this->getNodeValueOrValue( m_sValue1, m_sValue1, pID2Node, pInputID2Value, pInputID2Widget);
        std::string sValue2 = "N/A";

        // by default do not evaluate else part
        m_bEvaluateElse = false;

        /*
         *
         * Any method that only needs value1
         *
         */
        if (sQCompareMode.compare("is_set", Qt::CaseInsensitive) == 0)
        {

            if (sValue1.size() == 0)
                m_bEvaluateElse = true;

        } else {

            sValue2 = this->getNodeValueOrValue( m_sValue2, m_sValue2, pID2Node, pInputID2Value, pInputID2Widget);

            if (sQCompareMode.compare("EQUALS", Qt::CaseInsensitive) == 0)
            {
                Qt::CaseSensitivity eCaseSensitivity = Qt::CaseInsensitive;
                if (sQCompareMode.compare("equals", Qt::CaseSensitive) == 0)
                {
                    eCaseSensitivity = Qt::CaseSensitive;
                }


                if ( QString(sValue1.c_str()).compare( QString(sValue2.c_str()), eCaseSensitivity ) == 0 )
                {
                    m_bEvaluateElse = false;

                } else {

                    m_bEvaluateElse = true;
                }

            }

        }


        /*
         * Any method that needs value2
         *
         */
        std::cerr << "Val 1: " << sValue1 << std::endl;
        std::cerr << "Val 2: " << sValue2 << std::endl;
        std::cerr << "Evaluating else node: " << m_bEvaluateElse << std::endl;

        sReturn = this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget, bEmitSignal);

        if (bEmitSignal)
        {
            if (m_iHasExecuteChild == 0)
                    emit finishedExecution();
        }

        return sReturn;

    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("COMP");
        vAttributes.push_back("VALUE1");
        vAttributes.push_back("VALUE2");
        vAttributes.push_back("SEP");

    }

    void childHasFinished( bool bEmitSignal = false )
    {

        if (bEmitSignal)
        {
            --m_iHasExecuteChild;

            if (m_iHasExecuteChild == 0)
                    emit finishedExecution();
        }

    }

/*
    std::string evaluateElseNode(std::map< std::string, ExecutionNode*>* pID2Node,
                                 std::map<std::string, std::string>* pInputID2Value,
                                 std::map<std::string, QWidget*>* pInputID2Widget);
*/
    std::string m_sCompareMode;
    std::string m_sValue1, m_sValue2;

    std::vector<std::string> m_vValidCompareModes;

    ExecutionNode* m_pElseNode = NULL;

    int m_iHasExecuteChild = 0;
    bool m_bEvaluateElse = false;

    std::string m_sNotSet;

};


#endif //BIOGUI_EXECUTIONIFNODE_H
