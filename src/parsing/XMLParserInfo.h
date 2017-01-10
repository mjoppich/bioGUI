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

#ifndef BIOGUI_XMLPARSERINFO_H
#define BIOGUI_XMLPARSERINFO_H


#include "XMLParser.h"

class XMLParserInfo : public XMLParser {
public:

    XMLParserInfo(std::string sFileName)
    : XMLParser()
    {

        this->initializeFile(sFileName);

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

        QFileInfo oFileInfo(QString(m_sCurrentDocument.c_str()));

        return this->getAttribute(m_pTemplateElement, "TITLE", oFileInfo.baseName());

    }

    QString getDescription()
    {
        this->setRoot();

        QFileInfo oFileInfo(QString(m_sCurrentDocument.c_str()));

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
