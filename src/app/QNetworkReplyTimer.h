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

#ifndef BIOGUI_QNETWORKREPLYTIMER_H
#define BIOGUI_QNETWORKREPLYTIMER_H

#include<QObject>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QNetworkReply>
#include <functional>

class QNetworkReplyTimer : public QObject {
Q_OBJECT

public:
    QNetworkReplyTimer(QNetworkReply* pReply, const int iMSecTimeout, std::function<void(void)> oTimeoutFunc = []() {  })
            : QObject(pReply)
    {

        if (pReply && pReply->isRunning())
        {
            m_oTimer.start(iMSecTimeout, this);
        }

        m_oTimeoutFunc = oTimeoutFunc;
    }

    static void set(QNetworkReply* reply, const int iMSecTimeout, std::function<void(void)> oTimeoutFunc = []() {  }) {
        new QNetworkReplyTimer(reply, iMSecTimeout, oTimeoutFunc);
    }

protected:

    void timerEvent(QTimerEvent * ev) {
        if (!m_oTimer.isActive() || ev->timerId() != m_oTimer.timerId())
            return;

        QNetworkReply* pReply = static_cast<QNetworkReply*>( this->parent() );
        if (pReply->isRunning())
        {
            pReply->close();
            m_oTimeoutFunc();
        }

        m_oTimer.stop();
    }

    QBasicTimer m_oTimer;

    std::function<void(void)> m_oTimeoutFunc = []() {  };
};


#endif //BIOGUI_QNETWORKREPLYTIMER_H
