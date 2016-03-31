//
// Created by joppich on 3/31/16.
//

#ifndef BIOGUI_STREAMTEXTEDIT_H
#define BIOGUI_STREAMTEXTEDIT_H


#include <QTextEdit>
#include <iostream>
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

protected slots:

    void receiveText(QString sString, QColor oColor)
    {
        this->moveCursor(QTextCursor::End);
        this->setTextColor(oColor);
        this->insertPlainText(sString);
    }

protected:
    std::vector<ExtendedBuffer*> m_vBuffers;

};


#endif //BIOGUI_STREAMTEXTEDIT_H
