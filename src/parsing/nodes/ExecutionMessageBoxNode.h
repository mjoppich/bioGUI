//
// Created by joppich on 2/20/17.
//

#ifndef BIOGUI_EXECUTIONMESSAGEBOXNODE_H
#define BIOGUI_EXECUTIONMESSAGEBOXNODE_H

#include "ExecutionNode.h"

#include <QDialogButtonBox>
#include <QMessageBox>

class ExecutionMessageBoxNode : public ExecutionNode {

public:

    enum MB_TYPE {INFO, ALERT, QUESTION} ;

    ExecutionMessageBoxNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {
        m_sTag = "messagebox";

        this->prepareButtonText();

        m_sValue = "";
        m_sCaption = this->getDomElementAttribute(pElement, "caption", "");

        m_eType = ExecutionMessageBoxNode::MB_TYPE::INFO;

        if (pElement != NULL)
            m_sValue = pElement->text();

        QString sType = this->getDomElementAttribute(pElement, "type", "INFO");
        sType = sType.toUpper();

        if (sType.compare("INFO", Qt::CaseInsensitive) == 0)
        {
            m_eType = ExecutionMessageBoxNode::MB_TYPE::INFO;
        } else if (sType.compare("ALERT", Qt::CaseInsensitive) == 0) {
            m_eType = ExecutionMessageBoxNode::MB_TYPE::ALERT;
        } else if (sType.compare("QUESTION", Qt::CaseInsensitive) == 0) {
            m_eType = ExecutionMessageBoxNode::MB_TYPE::QUESTION;
        }

    }

    virtual ~ExecutionMessageBoxNode()
    {

    }

    std::string evaluate( std::map< std::string, ExecutionNode*>* pID2Node,
                          std::map<std::string, std::string>* pInputID2Value,
                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {

        if (m_sValue.size() > 0)
        {

            std::string sValue = m_sValue.toStdString();
            sValue = this->parseDynamicValues(sValue, pID2Node, pInputID2Value, pInputID2FunctionWidget);

            m_sValue = QString(sValue.c_str());

            QMessageBox::StandardButton reply = QMessageBox::NoButton;
            switch (m_eType)
            {

                case MB_TYPE::ALERT:

                    reply = QMessageBox::critical(NULL, m_sCaption, m_sValue);

                    break;


                case MB_TYPE::QUESTION:

                    reply = QMessageBox::question(NULL, m_sCaption, m_sValue);

                    break;

                case MB_TYPE::INFO:
                default:

                    reply = QMessageBox::information(NULL, m_sCaption, m_sValue);

                    break;


            }

            return this->buttonText(reply).toStdString();

        }


        return m_sValue.toStdString();

    }

protected:

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("TYPE");
    }

    void prepareButtonText()
    {

        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::NoButton, "" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Ok, "Ok" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Save, "Save" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::SaveAll, "Save All" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Open, "Open" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Yes, "Yes" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::YesToAll, "Yes To All" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::No, "No" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::NoToAll, "No To All" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Abort, "Abort" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Retry, "Retry" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Ignore, "Ignore" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Close, "Close" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Cancel, "Cancel" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Discard, "Discard" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Help, "Help" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Apply, "Apply" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::Reset, "Reset" ) );
        m_mButtonText.insert( std::pair<QMessageBox::StandardButton, QString>( QMessageBox::RestoreDefaults, "Restore Defaults" ) );

    }

    QString buttonText(QMessageBox::StandardButton eButton)
    {

        std::map<QMessageBox::StandardButton, QString>::iterator oIt = m_mButtonText.find(eButton);

        if (oIt != m_mButtonText.end())
            return oIt->second;

        return "";

    }

    ExecutionMessageBoxNode::MB_TYPE m_eType;

    QString m_sValue;
    QString m_sCaption;

    std::map<QMessageBox::StandardButton, QString> m_mButtonText;

};


#endif //BIOGUI_EXECUTIONMESSAGEBOXNODE_H
