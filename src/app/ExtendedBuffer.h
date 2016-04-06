//
// Created by joppich on 3/31/16.
//

#ifndef BIOGUI_EXTENDEDBUFFER_H
#define BIOGUI_EXTENDEDBUFFER_H

#include <QProcess>
#include <QObject>
#include <QColor>
#include <iostream>

class ExtendedBuffer : public QObject, public std::basic_streambuf<char> {
    Q_OBJECT
public:

    ExtendedBuffer()
            : std::basic_streambuf<char>()
    {
    }

    ExtendedBuffer(QProcess* pProcess, QProcess::ProcessChannel eChannel)
    {

        this->m_pParentProcess = new sParentProcess();
        this->m_pParentProcess->pProcess = pProcess;
        this->m_pParentProcess->eChannel = eChannel;

    }

    ~ExtendedBuffer()
    {

        if (this->m_pParentProcess != NULL)
        {
            delete m_pParentProcess;
        }

    }

    void setTextColor(QColor oColor)
    {
        m_oColor = oColor;
    }

    void setStreamID(QString sID)
    {
        m_sID = sID;
    }



public slots:

    void receiveProcData()
    {

        if (this->m_pParentProcess == NULL)
            throw "no parent process given";

        QByteArray oArray;

        switch (this->m_pParentProcess->eChannel)
        {
            default:

            case QProcess::StandardOutput:

                oArray = this->m_pParentProcess->pProcess->readAllStandardOutput();
                break;

            case QProcess::StandardError:

                oArray = this->m_pParentProcess->pProcess->readAllStandardError();
                break;
        }


        QString sString = QString(oArray);

        emit sendText(sString, m_oColor, m_sID);
    }

    void transferText(QString sString)
    {
        emit sendText(sString, m_oColor, m_sID);
    }

    signals:
    void sendText(QString sString, QColor oColor, QString sID);

protected:

    struct sParentProcess
    {
        QProcess* pProcess;
        QProcess::ProcessChannel eChannel;
    };


    virtual int_type overflow(int_type v)
    {
        if (v == '\n')
        {
        }
        return v;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n)
    {

        std::string sStdText(p, n);

        if ((p[n] == 4) || (p[n] == 0))
        {
            sStdText.append("\n");
        }

        QString sText(sStdText.c_str());

        this->transferText(sText);

        return n;
    }

private:
    QColor m_oColor;
    QString m_sID;

    sParentProcess* m_pParentProcess = NULL;

};


#endif //BIOGUI_EXTENDEDBUFFER_H
