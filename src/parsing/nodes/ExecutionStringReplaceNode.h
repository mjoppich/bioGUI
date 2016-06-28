//
// Created by joppich on 4/25/16.
//

#ifndef BIOGUI_EXECUTIONSTRINGREPLACENODE_H
#define BIOGUI_EXECUTIONSTRINGREPLACENODE_H


#include <QFile>
#include <QTextStream>
#include "ExecutionNode.h"
#include "ExecutionValueNode.h"

class ExecutionStringReplaceNode : public ExecutionNode {

    public:

        ExecutionStringReplaceNode(QDomElement* pElement)
                : ExecutionNode(pElement)
        {


            m_sReplace = this->getDomElementAttribute(pElement, "replace", "").toStdString();
            m_sReplaceWith = this->getDomElementAttribute(pElement, "replace_with", "").toStdString();

            m_sFrom = this->getDomElementAttribute(pElement, "from", "").toStdString();

        }

        std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                              std::map<std::string, std::string>* pInputID2Value,
                              std::map<std::string, QWidget*>* pInputID2Widget)
        {

            std::string sChildren = "";

            if (m_sFrom.size() == 0)
            {

                 sChildren = this->evaluateChildren(pID2Node, pInputID2Value, pInputID2Widget);

            } else {

                sChildren = ExecutionValueNode::getFromID(m_sFrom, pID2Node, pInputID2Value, pInputID2Widget);

            }

            QString qsChildren(sChildren.c_str());
            qsChildren = qsChildren.replace(QString(m_sReplace.c_str()), QString(m_sReplaceWith.c_str()));

            return qsChildren.toStdString();

        }

protected:

    std::string m_sReplace;
    std::string m_sReplaceWith;

    std::string m_sFrom;

};


#endif //BIOGUI_EXECUTIONSTRINGREPLACENODE_H
