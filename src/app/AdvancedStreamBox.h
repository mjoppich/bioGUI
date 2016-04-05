//
// Created by joppich on 4/5/16.
//

#ifndef BIOGUI_ADVANCEDSTREAMBOX_H
#define BIOGUI_ADVANCEDSTREAMBOX_H

#include <QListWidget>
#include <QTextCursor>
#include <QProcess>
#include <QAbstractButton>
#include "ExtendedBuffer.h"


class AdvancedListWidgetItem : public QListWidgetItem {

public:

    AdvancedListWidgetItem(QString sText, QColor oColor)
            : QListWidgetItem(sText)
    {
        this->setTextColor(oColor);
    }

    QString getStreamID()
    {
        return m_sStreamID;
    }

    void setStreamID(QString sStreamID)
    {
        m_sStreamID = sStreamID;
    }

    void appendText(QString sText)
    {

        while (sText.at(sText.size()-1) == '\n')
        {
            m_bFinished = true;
            sText = sText.trimmed();
        }

        QString sMyText = this->text();
        sMyText.append(sText);
        this->setText(sMyText);


    }

    void setTextColor(QColor oColor)
    {
        this->setTextColor(oColor);
    }

    bool isFinished()
    {
        return m_bFinished;
    }

protected:

    QString m_sStreamID;
    bool m_bFinished;


};

class AdvancedStreamBox : public QListWidget {
Q_OBJECT

public:

    AdvancedStreamBox()
            : QListWidget()
    {
        qRegisterMetaType<QTextCursor>("QTextCursor");
        qRegisterMetaType<QString>("QString");
    }

    void addBuffer(QProcess* pProcess, QProcess::ProcessChannel eChannel, QString sStreamID, QColor oTextColor)
    {

        ExtendedBuffer* pBuffer = new ExtendedBuffer(pProcess, eChannel);
        pBuffer->setTextColor( oTextColor );
        pBuffer->setStreamID( sStreamID );

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

        QObject::connect(pBuffer, SIGNAL(sendText(QString,QColor, QString)), this , SLOT(receiveText(QString,QColor, QString)), Qt::QueuedConnection );

    }

    void addStream(std::string sStreamID, QAbstractButton* pControl)
    {

        m_mStreamID2Button.insert( std::pair<std::string, QAbstractButton*>(sStreamID, pControl) );

    }


protected slots:

    void filterText()
    {

        std::map<std::string, QAbstractButton*>::iterator oIt = m_mStreamID2Button.begin();
        std::map<std::string, bool> mShowText;

        while( oIt != m_mStreamID2Button.end())
        {

            mShowText.insert( std::pair<std::string, bool>( oIt->first, oIt->second->isChecked() ));

            ++oIt;

        }



        size_t iCount = this->count();

        for (size_t i = 0; i < iCount; ++i)
        {

            size_t iIndex = iCount - 1 - i;

            AdvancedListWidgetItem* pItem = dynamic_cast<AdvancedListWidgetItem*>( this->item(iIndex) );

            if (pItem == NULL)
                continue;

            std::map<std::string, bool>::iterator oDecide = mShowText.find( pItem->getStreamID().toStdString() );

            if (oDecide != mShowText.end())
            {

                pItem->setHidden(!oDecide->second);

            }

        }

    }


    void receiveText(QString sString, QColor oColor, QString sStreamID)
    {

        AdvancedListWidgetItem* pLastStream = this->getLastItemForStream(&sStreamID);

        if ((pLastStream == NULL) || (pLastStream->isFinished()))
        {
            pLastStream = new AdvancedListWidgetItem("", oColor);
            this->addItem(pLastStream);
        }

        QStringList vList = sString.split("\n");
        pLastStream->appendText( vList.at(0) );

        for (size_t i = 1; i < vList.size(); ++i)
        {

            pLastStream = new AdvancedListWidgetItem( vList.at(i), oColor );
            this->addItem(pLastStream);

        }

    }

protected:


    AdvancedListWidgetItem* getLastItemForStream(QString* sStreamID)
    {

        size_t iCount = this->count();

        for (size_t i = 0; i < iCount; ++i)
        {

            size_t iIndex = iCount - 1 - i;

            AdvancedListWidgetItem* pItem = dynamic_cast<AdvancedListWidgetItem*>( this->item(iIndex) );

            if (pItem == NULL)
                return NULL;

            if (pItem->getStreamID().compare( QString(sStreamID->toStdString().c_str()), Qt::CaseInsensitive) == 0)
            {
                return pItem;
            }

        }

        return NULL;
    }


    std::vector<ExtendedBuffer*> m_vBuffers;
    std::map<std::string, QAbstractButton*> m_mStreamID2Button;
};


#endif //BIOGUI_ADVANCEDSTREAMBOX_H
