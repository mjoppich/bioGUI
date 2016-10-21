//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWLAYOUTNODE_H
#define BIOGUI_WINDOWLAYOUTNODE_H

#include <QWidget>
#include "WindowNode.h"

class WindowLayoutNode : public WindowNode<QLayout> {
public:

    WindowLayoutNode()
            : WindowNode<QWidget>()
    {
    }

    virtual ~WindowWidgetNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement ) = 0;

protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes) = 0;

};

#endif //BIOGUI_WINDOWLAYOUTNODE_H
