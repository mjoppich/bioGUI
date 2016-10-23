//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETNODE_H
#define BIOGUI_WINDOWWIDGETNODE_H

#include <QWidget>
#include "WindowNode.h"

class WindowWidgetNode : public WindowNode<QWidget> {
public:

    WindowWidgetNode(WindowComponentFactory* pFactory)
    : WindowNode<QWidget>(pFactory)
    {
    }

    virtual ~WindowWidgetNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement ) = 0;

protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes) = 0;

};


#endif //BIOGUI_WINDOWWIDGETNODE_H
