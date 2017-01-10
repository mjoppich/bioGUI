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
