//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWLAYOUTHORIZONTALNODE_H
#define BIOGUI_WINDOWLAYOUTHORIZONTALNODE_H

#include <QtWidgets/QHBoxLayout>
#include <src/app/QOrderedLayout.h>
#include "WindowLayoutNode.h"

class WindowLayoutHorizontalNode : public WindowLayoutNode {

public:

    WindowLayoutHorizontalNode()
            : WindowLayoutNode()
    {

    }

    virtual ~WindowLayoutHorizontalNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QHBoxLayout *pLayout = new QOrderedHBoxLayout();

        pLayout->setAlignment(Qt::AlignHorizontal_Mask);

        CreatedElement oReturn;
        oReturn.pElement = pLayout;

        return oReturn;


    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {


    }


};

#endif //BIOGUI_WINDOWLAYOUTHORIZONTALNODE_H
