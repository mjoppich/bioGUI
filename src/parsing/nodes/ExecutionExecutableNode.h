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

signals:

    void finishedExecution();


};



#endif // EXECUTIONEXECUTABLENODE_H
