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

class ExecutionWSLtransformNode : public ExecutionNode {

public:

    ExecutionWSLtransformNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {

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

        // absolute path
        if (qsChildren.at(1) != ':')
        {
            QDir oPath(qsChildren);
            qsChildren = oPath.absolutePath();
        }

        QChar cDrive = qsChildren.at(0);
        qsChildren.remove(0,2);
        qsChildren.prepend(QString("/mnt/") + cDrive.toLower() );
        qsChildren.replace("\\", "/");

        return qsChildren.toStdString();

    }

protected:

    std::string m_sFrom;

};


#endif //BIOGUI_EXECUTIONWSLTRANSFORMNODE_H
