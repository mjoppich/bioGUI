//
// Created by mjoppich on 2/19/17.
//

#ifndef BIOGUI_WIDGETFUNCTIONNODE_H
#define BIOGUI_WIDGETFUNCTIONNODE_H

#include <functional>
#include <string>

class QWidget;

class WidgetFunctionNode
{

public:

    WidgetFunctionNode(QWidget* pWidget, std::function<void(const QWidget*, std::string, std::string)> oFunction = [] (const QWidget* pVal, std::string key, std::string value) {})
    : m_oFunction(oFunction), m_pWidget(pWidget)
    {

    }

    void setAttribute(std::string sAttribute, std::string sValue)
    {
        m_oFunction(m_pWidget, sAttribute, sValue);
    }

    QWidget* getWidget()
    {
        return m_pWidget;
    }

protected:

    QWidget* const m_pWidget;
    std::function<void(const QWidget*, std::string, std::string)> const m_oFunction;

};

#endif //BIOGUI_WIDGETFUNCTIONNODE_H
