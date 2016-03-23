/****************************************************************************
**

****************************************************************************/

#ifndef BIOGUI_H
#define BIOGUI_H

#include <QMainWindow>

class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;

class bioGUI : public QMainWindow
{
Q_OBJECT

public:
    bioGUI(std::string sXMLfile);

    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void commitData(QSessionManager &);

private:
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QPlainTextEdit *textEdit;
    QString curFile;
};

#endif