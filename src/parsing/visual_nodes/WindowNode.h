/*
 *  bioGUI - a new universal GUI for bioinformatic applications
 *  Copyright (C) 2016  Markus Joppich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BIOGUI_WINDOWNODE_H
#define BIOGUI_WINDOWNODE_H

#include <exception>
#include <string>
#include <QDomElement>
#include <functional>

/*
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
*/


class WindowComponentFactory;


class WindowBaseNode
{
public:
    WindowBaseNode(WindowComponentFactory* pFactory)
    : m_pFactory(pFactory)
    {
    }

    virtual ~WindowBaseNode()
    {

    }

    std::vector<std::string> getAcceptedAttributes()
    {
        std::vector<std::string> vReturnAttribs;

        this->addNodeAttributes(vReturnAttribs);

        std::sort(vReturnAttribs.begin(), vReturnAttribs.end());

        return vReturnAttribs;
    }

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

    QString getQAttribute(QDomElement* pElement, QString sAttribName, QString sDefault)
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

    std::string getAttribute(QDomElement* pElement, QString sAttribName, QString sDefault)
    {

        QString sQAttrib = this->getQAttribute(pElement, sAttribName, sDefault);

        return sQAttrib.toStdString();
    }

    std::string getDomID(QDomElement* pDOMElement)
    {
        return this->getAttribute(pDOMElement, "id", "");
    }

    WindowComponentFactory* m_pFactory = NULL;
};


struct Retriever {

    std::string sElementID;
    std::function<std::string()> oRetriever;

};

template <class T>
class WindowNode : public WindowBaseNode {
public:

    WindowNode(WindowComponentFactory* pFactory)
            : WindowBaseNode(pFactory)
    {

    }

    virtual ~WindowNode()
    {

    }


    struct CreatedElement {
        T* pElement = NULL;
        std::vector< Retriever > vRetriever;
        bool hasRetriever()
        {
            return vRetriever.size() > 0;
        }

        void addRetriever(std::string sID,std::function<std::string()> oFunc )
        {
            vRetriever.push_back( {sID, oFunc} );
        }

        bool bHasChildrenFinished = false;
    };



    virtual typename WindowNode<T>::CreatedElement getWindowElement( QDomElement* pDOMElement ) = 0;



};

#endif //BIOGUI_WINDOWNODE_H
