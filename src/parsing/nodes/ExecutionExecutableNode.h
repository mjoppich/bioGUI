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
