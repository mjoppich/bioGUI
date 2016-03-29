//
// Created by mjopp on 26.03.2016.
//

#ifndef BIOGUI_BIOGUIAPP_H
#define BIOGUI_BIOGUIAPP_H

#include <QApplication>
#include <QXmlQuery.h>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QXmlResultItems>
#include <iostream>

class bioGUIapp : public QApplication {
    Q_OBJECT
public:

    bioGUIapp(int argc, char* argv[])
    : QApplication(argc, argv)
    {
        startXML();
    }

    int startXML()
    {
        QXmlQuery query;
        QXmlQuery entries;
        QXmlResultItems res;
        QDomDocument rhythmdb;


        /*
         * Try and open the Rhythmbox DB. An API call which tells us where
         * the file is would be nice.
         */
        QFile db( "/cygdrive/c/libraries/bioGUI/example.gui");
        if ( ! db.exists()) {

            qDebug() << "bla";
            return 1;

        }

        if (!db.open(QIODevice::ReadOnly | QIODevice::Text))
            return 1;

        /*
         * Use QXmlQuery to execute and XPath query. Check the version to
         * make sure.
         */
        query.setFocus(&db);
        query.setQuery("//label");
        if ( ! query.isValid())
            return 1;

        query.evaluateTo(&res);
        db.close();

        QXmlItem item(res.next());
        while (!item.isNull()) {
            if (item.isNode())
            {
                query.setFocus(item);
                query.setQuery("./string()");
                QString text;
                query.evaluateTo(&text);

                std::cout << "i" << text.toStdString() << std::endl;
            }

            item = res.next();

        }

    }

};


#endif //BIOGUI_BIOGUIAPP_H
