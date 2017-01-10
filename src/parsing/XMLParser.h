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

#ifndef BIOGUI_XMLPARSER_H
#define BIOGUI_XMLPARSER_H

#include <QtCore>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <src/Logging.h>

/*
class XMLParserException : public std::exception
{

public:
    XMLParserException(std::string sMessage)
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

class XMLParser : public QObject {

Q_OBJECT

public:

    XMLParser()
    : QObject(0)
    {

        m_pKnownTags = new std::vector<std::string>();

    }

    ~XMLParser()
    {
        delete m_pKnownTags;

        if (m_pDocument)
        {
            delete m_pDocument;
        }
    }

    void initializeFile(std::string& sFileName)
    {
        if (m_pDocument != NULL)
        {
            delete m_pDocument;
            m_sCurrentDocument = "";

        }

        m_pDocument = this->loadFromFile(sFileName);
        m_sCurrentDocument = sFileName;
    }

    std::string getCurrentDocumentPath()
    {
        return m_sCurrentDocument;
    }

    virtual void printAvailableNodes()
    {

    }

    QDomDocument* getDocument()
    {
        return m_pDocument;
    }

protected:

    QDomDocument* loadFromFile(std::string sFileName)
    {
        return this->loadFromFile( QString(sFileName.c_str()) );
    }

    QDomDocument* loadFromFile(QString sFileName)
    {

        // Open a file for reading
        QFile* pFile = new QFile( sFileName );

        if(!pFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            LOGERROR("Error opening file: " + sFileName.toStdString());
            return NULL;
        }

        QDomDocument* pDocument = new QDomDocument();

        QString sErrorString;
        int iErrorLine;
        int iErrorColumn;

        // loading
        if(!pDocument->setContent(pFile, false, &sErrorString, &iErrorLine, &iErrorColumn))
        {

            std::cerr << "Error loading File " << sFileName.toStdString() << " in line " << iErrorLine << ":" << iErrorColumn << " : " << sErrorString.toStdString() << std::endl;

            //throw "Failed to load the file for reading.";
            pFile->close();
            delete pFile;
            return NULL;
        }

        pFile->close();
        delete pFile;

        bool bIsValid = this->checkValidTags( getRoot(pDocument) );

        if (!bIsValid)
        {
            return NULL;
        }

        return pDocument;

    }

    virtual QDomElement* getRoot( QDomDocument* pDocument ) = 0;

    virtual bool checkValidTags(QDomElement* pElement)
    {

        if (!pElement->isElement())
            return true;

        bool bReturn = isValidTag( pElement->tagName() );

        if (bReturn == false)
        {
            std::string sInvalidTag = pElement->tagName().toStdString();
            std::cout << sInvalidTag << " is not a valid tag." << std::endl;
        }

        QDomNodeList oChildren = pElement->childNodes();

        for (size_t i = 0; i < oChildren.length(); ++i)
        {

            QDomElement oChild = oChildren.item(i).toElement();

            bool bResult = checkValidTags( &oChild );
            bReturn = bReturn && bResult;
        }


        return bReturn;

    }

    bool isValidTag(QString sTagName)
    {

        std::string sTag = sTagName.toUpper().toStdString();

        std::vector<std::string>::iterator oIt = std::find( m_pKnownTags->begin(), m_pKnownTags->end(), sTag );

        return (oIt != m_pKnownTags->end());

    }

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

    QDomElement* getDocumentElementByName( QDomDocument* pDocument, QString sTagName )
    {

        if (!pDocument)
            return NULL;

        QDomNodeList oChildren = pDocument->elementsByTagName( sTagName );

        for (size_t i = 0; i < oChildren.length(); ++i)
        {

            QDomElement oChild = oChildren.item(i).toElement();

            //printAllElements( &oChild );

            if (oChild.toElement().tagName().compare( sTagName , Qt::CaseInsensitive) == 0)
            {
                QDomElement* pRoot = new QDomElement( oChild );
                return pRoot;
            }

        }

        return NULL;

    }

    QDomNode* getDocumentNodeByID( QDomNode* pNode, QString sID )
    {


        QDomElement oElement = pNode->toElement();

        QString sThisID = this->getAttribute(&oElement, "ID", "");

        if (sThisID.compare( sID , Qt::CaseInsensitive) == 0)
        {
            return pNode;
        }

        QDomNodeList oChildren = pNode->childNodes();

        for (size_t i = 0; i < oChildren.length(); ++i)
        {

            QDomNode oNode = oChildren.at(i);
            QDomNode* pReturn = getDocumentNodeByID( &oNode , sID);

            if (pReturn != NULL)
                return pReturn;

        }

        return NULL;

    }

    void printAllElements(QDomElement* pElement)
    {

        qDebug() << pElement->tagName() << " child of " << pElement->parentNode().toElement().tagName() << " " << pElement->isElement();


        QDomNodeList oChildren = pElement->childNodes();

        for (size_t i = 0; i < oChildren.length(); ++i)
        {

            QDomElement oChild = oChildren.item(i).toElement();

            printAllElements( &oChild );

        }


    }

    std::vector<std::string>* m_pKnownTags = NULL;
    QDomDocument* m_pDocument = NULL;
    std::string m_sCurrentDocument;

};


#endif //BIOGUI_XMLPARSER_H
