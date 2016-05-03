//
// Created by joppich on 4/6/16.
//

#ifndef BIOGUI_XMLPARSERINFO_H
#define BIOGUI_XMLPARSERINFO_H


#include "XMLParser.h"

class XMLParserInfo : public XMLParser {


public:

    XMLParserInfo(std::string sFileName)
    : XMLParser(sFileName)
    {
        m_pDocument = loadFromFile(sFileName);
    }


    QDomElement* getRoot( QDomDocument* pDocument ) {
        return this->getDocumentElementByName(pDocument, "template");
    }

    virtual bool checkValidTags(QDomElement* pElement)
    {
        // we do not want to check that here ...
        return true;
    }

    QString getTitle()
    {
        this->setRoot();

        QFileInfo oFileInfo(QString(m_sTemplateFile.c_str()));

        return this->getAttribute(m_pTemplateElement, "TITLE", oFileInfo.baseName());

    }

    QString getDescription()
    {
        this->setRoot();

        QFileInfo oFileInfo(QString(m_sTemplateFile.c_str()));

        return this->getAttribute(m_pTemplateElement, "Description", oFileInfo.baseName());

    }

    QString getIcon()
    {
        this->setRoot();

        return this->getAttribute(m_pTemplateElement, "icon", "biogui_logo.png");

    }


protected:

    void setRoot()
    {
        if (m_pTemplateElement == NULL)
            m_pTemplateElement = this->getRoot(m_pDocument);
    }


    QDomElement* m_pTemplateElement = NULL;


};


#endif //BIOGUI_XMLPARSERINFO_H
