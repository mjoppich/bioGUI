//
// Created by mjoppich on 2/19/17.
//

#ifndef BIOGUI_WIDGETFUNCTIONNODE_H
#define BIOGUI_WIDGETFUNCTIONNODE_H

#include <functional>

class WidgetFunctionNode
{

public:

    WidgetFunctionNode(QWidget* pWidget, std::function<QWidget*, std::string, std::string(void)> oFunction = [] (QWidget* pVal, std::string key, std::string value) {})
    : m_oFunction(oFunction), m_pWidget(pWidget)
    {

    }

    void setAttribute(std::string sAttribute, std::string sValue)
    {
        this->m_oFunction(m_pWidget, sAttribute, sValue);
    }


protected:

    const QWidget* m_pWidget;
    const std::function<QWidget* std::string, std::string(void)> m_oFunction;

};

#endif //BIOGUI_WIDGETFUNCTIONNODE_H
