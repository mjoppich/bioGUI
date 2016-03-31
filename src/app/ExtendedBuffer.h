//
// Created by joppich on 3/31/16.
//

#ifndef BIOGUI_EXTENDEDBUFFER_H
#define BIOGUI_EXTENDEDBUFFER_H

#include <QObject>
#include <QColor>

class ExtendedBuffer : public QObject, public std::basic_streambuf<char> {
    Q_OBJECT
public:

    ExtendedBuffer()
            : std::basic_streambuf<char>()
    {
    }

    ~ExtendedBuffer()
    {
    }

    void setTextColor(QColor oColor)
    {
        m_oColor = oColor;
    }

public slots:
    void transferText(QString sString)
    {
        emit sendText(sString, m_oColor);
    }

    signals:
    void sendText(QString sString, QColor oColor);

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

private:
    QColor m_oColor;

};


#endif //BIOGUI_EXTENDEDBUFFER_H
