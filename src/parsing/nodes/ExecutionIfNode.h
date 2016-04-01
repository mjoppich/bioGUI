//
// Created by joppich on 3/31/16.
//

#ifndef BIOGUI_EXECUTIONIFNODE_H
#define BIOGUI_EXECUTIONIFNODE_H

#include "ExecutionNode.h"

class ExecutionIfNode : public ExecutionNode {

public:

    ExecutionIfNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {

        std::string sNotSet = "#NOTSET#";

        m_sCompareMode = this->getDomElementAttribute(pElement, "comp", "is_set").toStdString();
        m_sValue1 = this->getDomElementAttribute(pElement, "value1", QString(sNotSet.c_str())).toStdString();
        m_sValue2 = this->getDomElementAttribute(pElement, "value2", QString(sNotSet.c_str())).toStdString();

        m_vValidCompareModes.push_back("is_set");
        m_vValidCompareModes.push_back("equals");
        m_vValidCompareModes.push_back("EQUALS");

        if (!(std::find(m_vValidCompareModes.begin(), m_vValidCompareModes.end(), m_sCompareMode) != m_vValidCompareModes.end() ))
        {
            throw "unknown compare method " + m_sCompareMode;
        }


        if (m_sCompareMode.compare("is_set") != 0)
        {

            if (m_sValue2.compare(sNotSet) == 0)
                throw "value2 must be set";

        }

        if (m_sValue1.compare(sNotSet) == 0)
            throw "value1 must be set";

    }

    std::string getValue(std::string sNodeID, std::map< std::string, ExecutionNode*>* pID2Node,
                         std::map<std::string, std::string>* pInputID2Value,
                         std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::map<std::string, std::string>::iterator oValueIt = pInputID2Value->find(sNodeID);
        if ( oValueIt != pInputID2Value->end())
        {
            return oValueIt->second;
        }


        std::map< std::string, ExecutionNode*>::iterator oNodeIt = pID2Node->find(sNodeID);

        if ( oNodeIt != pID2Node->end() )
        {
            return oNodeIt->second->evaluate(pID2Node, pInputID2Value, pInputID2Widget);

        }

        return sNodeID;


    }


    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, QWidget*>* pInputID2Widget)
    {

        std::string sValue1 = this->getValue( m_sValue1, pID2Node, pInputID2Value, pInputID2Widget);

        /*
         *
         * Any method that only needs value1
         *
         */

        if (m_sCompareMode.compare("is_set") == 0)
        {

            if (sValue1.size() > 0)
                return this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget);

        }


        /*
         * Any method that needs value2
         *
         */

        std::string sValue2 = this->getValue( m_sValue2, pID2Node, pInputID2Value, pInputID2Widget);


        if (QString(m_sCompareMode.c_str()).compare("EQUALS", Qt::CaseInsensitive) == 0)
        {
            Qt::CaseSensitivity eCaseSensitivity = Qt::CaseInsensitive;
            if (m_sCompareMode.compare("equals") == 0)
            {
                eCaseSensitivity = Qt::CaseSensitive;
            }


            if ( QString(sValue1.c_str()).compare( QString(sValue2.c_str()), eCaseSensitivity ) == 0 )
            {

                return this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget);

            }
        }



        return "";

    }

protected:

    std::string m_sCompareMode;
    std::string m_sValue1, m_sValue2;

    std::vector<std::string> m_vValidCompareModes;

};


#endif //BIOGUI_EXECUTIONIFNODE_H
