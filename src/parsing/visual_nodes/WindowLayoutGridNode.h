//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWLAYOUTGRIDNODE_H
#define BIOGUI_WINDOWLAYOUTGRIDNODE_H

#include <src/app/QExtGridLayout.h>
#include "WindowLayoutNode.h"

class WindowLayoutGridNode : public WindowLayoutNode {

public:

    WindowLayoutGridNode(WindowComponentFactory* pFactory)
            : WindowLayoutNode(pFactory)
    {

    }

    virtual ~WindowLayoutGridNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement )
    {

        int iRows = this->getQAttribute(pDOMElement, "rows", 0).toInt();
        int iCols = this->getQAttribute(pDOMElement, "cols", 0).toInt();

        QExtGridLayout *pLayout = new QExtGridLayout(iRows, iCols);

        pLayout->setAlignment(Qt::AlignVertical_Mask);

        CreatedElement oReturn;

        oReturn.pElement = pLayout;

        return oReturn;

    }


protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("rows");
        vAttributes.push_back("cols");
    }


};

#endif //BIOGUI_WINDOWLAYOUTGRIDNODE_H
