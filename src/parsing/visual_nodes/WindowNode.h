//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWNODE_H
#define BIOGUI_WINDOWNODE_H

#include <exception>
#include <string>
#include <QDomElement>

class WindowNodeException : public std::exception
{
public:

    WindowNodeException(std::string sMessage)
            : std::exception(), m_sMessage(sMessage)
    {

    }

    char const* what() const throw()
    {
        return m_sMessage.c_str();
    }

protected:

    const std::string m_sMessage;

};

template <class T>
class WindowNode {
public:

    WindowNode()
    {
    }

    virtual ~WindowNode()
    {

    }

    struct CreatedElement {
        T* pElement = NULL;
        std::function<std::string()> oRetriever = [] () {return "";};
        bool bHasRetriever = false;
        bool bHasChildrenFinished = false;
    };

    std::vector<std::string> getAcceptedAttributes()
    {
        std::vector<std::string> vReturnAttribs;

        this->addNodeAttributes(vReturnAttribs);

        std::sort(vReturnAttribs.begin(), vReturnAttribs.end());

        return vReturnAttribs;
    }

    virtual typename WindowNode<T>::CreatedElement getWindowElement( QDomElement* pDOMElement ) = 0;

protected:

    virtual void addNodeAttributes(std::vector<std::string>& vAttributes) = 0;

    bool hasAttribute(QDomElement* pElement, QString sAttribName)
    {
        if (pElement == NULL)
            return false;

        QDomNamedNodeMap mAttributes = pElement->attributes();

        for (size_t i = 0; i < mAttributes.length(); ++i)
        {

            QDomAttr oItem = mAttributes.item(i).toAttr();

            if (oItem.name().compare( sAttribName, Qt::CaseInsensitive ) == 0)
            {
                return true;
            }

        }

        return false;
    }

    QString getAttribute(QDomElement* pElement, QString sAttribName, QString sDefault)
    {

        if (pElement == NULL)
            return sDefault;

        QDomNamedNodeMap mAttributes = pElement->attributes();

        for (size_t i = 0; i < mAttributes.length(); ++i)
        {

            QDomAttr oItem = mAttributes.item(i).toAttr();

            if (oItem.name().compare( sAttribName, Qt::CaseInsensitive ) == 0)
            {
                return oItem.value();
            }

        }

        return sDefault;

    }

};

#endif //BIOGUI_WINDOWNODE_H
