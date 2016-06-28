//
// Created by joppich on 5/9/16.
//

#ifndef BIOGUI_EXTENDEDSTDBUFFER_H
#define BIOGUI_EXTENDEDSTDBUFFER_H

#include <QObject>
#include <QColor>
#include <iostream>

class ExtendedStdBuffer : public QObject, public std::basic_streambuf<char> {
    Q_OBJECT
public:

    ExtendedStdBuffer()
            : std::basic_streambuf<char>()
    {
    }

    ~ExtendedStdBuffer()
    {
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
    void transferText(QString sString)
    {
        emit sendText(sString, m_oColor, m_sID);
    }

signals:
    void sendText(QString sString, QColor oColor, QString sID);

protected:
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

    QColor m_oColor;
    QString m_sID;


};


#endif //BIOGUI_EXTENDEDSTDBUFFER_H
