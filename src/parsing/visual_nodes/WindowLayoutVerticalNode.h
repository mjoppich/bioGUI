//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWLAYOUTVERTICALNODE_H
#define BIOGUI_WINDOWLAYOUTVERTICALNODE_H


#include <QtWidgets/QVBoxLayout>
#include <src/app/QOrderedLayout.h>
#include "WindowLayoutNode.h"

class WindowLayoutVerticalNode : public WindowLayoutNode {

public:

    WindowLayoutVerticalNode()
            : WindowLayoutNode()
    {

    }

    virtual ~WindowLayoutVerticalNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        QVBoxLayout *pLayout = new QOrderedVBoxLayout();

        pLayout->setAlignment(Qt::AlignVertical_Mask);

        CreatedElement oReturn;
        oReturn.pElement = pLayout;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {


    }


};

#endif //BIOGUI_WINDOWLAYOUTVERTICALNODE_H
