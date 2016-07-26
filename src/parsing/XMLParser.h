//
// Created by joppich on 3/30/16.
//

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


class XMLParser : public QObject {

Q_OBJECT

public:

    XMLParser(std::string sFileName)
    : QObject(0), m_sTemplateFile(sFileName)
    {

        m_pKnownTags = new std::vector<std::string>();

    }

    ~XMLParser()
    {

        delete m_pDocument;
        delete m_pKnownTags;

    }

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
            throw "Failed to open the file for reading.";
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


    std::string getTemplateFile()
    {
        return m_sTemplateFile;
    }

    bool isValid()
    {
        return m_pDocument != NULL;
    }

protected:

    virtual QDomElement* getRoot( QDomDocument* pDocument ) = 0;

    virtual bool checkValidTags(QDomElement* pElement)
    {

        if (!pElement->isElement())
            return true;

        bool bReturn = isValidTag( pElement->tagName() );

        if (bReturn == false)
        {
            std::cerr << pElement->tagName().toStdString() << " is not a valid tag." << std::endl;
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

        std::string sTag = sTagName.toStdString();

        std::vector<std::string>::iterator oIt = std::find( m_pKnownTags->begin(), m_pKnownTags->end(), sTag );

        return (oIt != m_pKnownTags->end());

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



    QDomDocument* m_pDocument;
    std::vector<std::string>* m_pKnownTags;
    std::string m_sTemplateFile;


};


#endif //BIOGUI_XMLPARSER_H
