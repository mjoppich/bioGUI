
/****************************************************************************
**

**
****************************************************************************/

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "bioGUI.h"



int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Application Example");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    bioGUI oGui( "example.gui" );



    oGui.show();
    return app.exec();

}


