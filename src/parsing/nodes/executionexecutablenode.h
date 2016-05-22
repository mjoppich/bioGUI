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

signals:

    void finishedExecution();


};



#endif // EXECUTIONEXECUTABLENODE_H
