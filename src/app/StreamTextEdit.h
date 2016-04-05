//
// Created by joppich on 3/31/16.
//

#ifndef BIOGUI_STREAMTEXTEDIT_H
#define BIOGUI_STREAMTEXTEDIT_H


#include <QTextEdit>
#include <QProcess>
#include <iostream>
#include <QtCore/qprocess.h>
#include <QAbstractButton>
#include "ExtendedBuffer.h"

class StreamTextEdit : public QTextEdit {
Q_OBJECT
public:

    StreamTextEdit(QWidget* pParent = 0)
    : QTextEdit(pParent)
    {

        qRegisterMetaType<QTextCursor>("QTextCursor");
        qRegisterMetaType<QString>("QString");

    }

    void addBuffer(std::ostream& oOut, QColor oTextColor)
    {

        ExtendedBuffer* pBuffer = new ExtendedBuffer();
        pBuffer->setTextColor( oTextColor );

        oOut.rdbuf(pBuffer);

        QObject::connect(pBuffer, SIGNAL(sendText(QString,QColor)), this , SLOT(receiveText(QString,QColor)), Qt::QueuedConnection );

    }

    void addBuffer(QProcess* pProcess, QProcess::ProcessChannel eChannel, QColor oTextColor)
    {

        ExtendedBuffer* pBuffer = new ExtendedBuffer(pProcess, eChannel);
        pBuffer->setTextColor( oTextColor );

        switch (eChannel)
        {
            default:

            case QProcess::ProcessChannel::StandardOutput:

                QObject::connect( pProcess, &QProcess::readyReadStandardOutput, pBuffer, &ExtendedBuffer::receiveProcData );


                break;
            case QProcess::ProcessChannel::StandardError :

                QObject::connect( pProcess, &QProcess::readyReadStandardError, pBuffer, &ExtendedBuffer::receiveProcData );

                break;

        }

        QObject::connect(pBuffer, SIGNAL(sendText(QString,QColor)), this , SLOT(receiveText(QString,QColor)), Qt::QueuedConnection );

    }

    void addStream(std::string sStreamID, QAbstractButton* pControl)
    {

        m_mStreamID2Button.insert( std::pair<std::string, QAbstractButton*>(sStreamID, pControl) );

    }

protected slots:

    void receiveText(QString sString, QColor oColor)
    {
        this->moveCursor(QTextCursor::End);
        this->setTextColor(oColor);
        this->insertPlainText(sString);
    }

protected:
    std::vector<ExtendedBuffer*> m_vBuffers;

    std::map<std::string, QAbstractButton*> m_mStreamID2Button;

};


#endif //BIOGUI_STREAMTEXTEDIT_H
