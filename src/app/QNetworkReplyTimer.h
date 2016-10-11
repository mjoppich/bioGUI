//
// Created by joppich on 7/25/16.
//

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
