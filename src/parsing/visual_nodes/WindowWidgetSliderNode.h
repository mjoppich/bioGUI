//
// Created by joppich on 10/21/16.
//

#ifndef BIOGUI_WINDOWWIDGETSLIDERNODE_H
#define BIOGUI_WINDOWWIDGETSLIDERNODE_H

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <src/Maths.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <QtGui/QDoubleValidator>
#include "WindowWidgetNode.h"

class WindowWidgetSliderNode : public WindowWidgetNode {

public:

    WindowWidgetSliderNode(WindowComponentFactory* pFactory)
            : WindowWidgetNode(pFactory)
    {

    }

    virtual ~WindowWidgetSliderNode()
    {

    }

    virtual CreatedElement getWindowElement( QDomElement* pDOMElement ) {

        QString sTag = pDOMElement->tagName();
        QString sValue = pDOMElement->text();

        CreatedElement oReturn;

        QSlider *pSlider = new QSlider(Qt::Orientation::Horizontal);
        pSlider->setFocusPolicy(Qt::StrongFocus);
        pSlider->setTickPosition(QSlider::TicksBelow);

        if (pDOMElement->hasChildNodes()) {

            std::vector<std::pair<QString, QString> > vValues;

            QDomNodeList oChildren = pDOMElement->childNodes();
            for (size_t i = 0; i < oChildren.size(); ++i) {
                QDomElement oChildNode = oChildren.at(i).toElement();

                /**
                 *
                 * ENSURE THAT THIS IS A SLIDERITEM!
                 *
                 *
                 */

                if (oChildNode.tagName().compare("slideritem", Qt::CaseInsensitive) != 0)
                {
                    throw WindowNodeException("Within slider only slideritem is allowed, but found: " + oChildNode.tagName().toStdString());
                }

                QString sValue = this->getQAttribute(&oChildNode, "value", "");
                QString sDisplay = this->getQAttribute(&oChildNode, "display", sValue);

                vValues.push_back(std::pair<QString, QString>(sValue, sDisplay));
            }

            pSlider->setRange(1, vValues.size());

            QGridLayout *pGLayout = new QGridLayout();
            pGLayout->addWidget(pSlider, 0, 0, 1, vValues.size(), Qt::AlignCenter);

            for (size_t i = 0; i < vValues.size(); ++i) {

                QLabel *pLabel = new QLabel(vValues.at(i).second);
                pGLayout->addWidget(pLabel, 1, i, 1, 1, Qt::AlignCenter);

            }


            oReturn.pElement = pSlider;

            oReturn.addRetriever(this->getDomID(pDOMElement) , [vValues, pSlider] () {

                return vValues.at(pSlider->value()-1).first.toStdString();

            });


             return oReturn;

        } else {

            float fMin = this->getQAttribute(pDOMElement, "min", "0.0").toFloat();
            float fMax = this->getQAttribute(pDOMElement, "max", "1.0").toFloat();
            float fStep = this->getQAttribute(pDOMElement, "step", "0.1").toFloat();

            int iMinFactor = Maths<float>::getIntegerFactor(fMin);
            int iMaxFactor = Maths<float>::getIntegerFactor(fMax);
            int iStepFactor = Maths<float>::getIntegerFactor(fStep);

            const int iFactor = std::max(std::max(iMaxFactor, iMinFactor), iStepFactor);

            int iMin = fMin * iFactor;
            int iMax = fMax * iFactor;
            int iStep = fStep * iFactor;

            std::cerr << "Slider: " << iMin << " " << iMax << " " << iStep << std::endl;

            pSlider->setRange(iMin, iMax);
            pSlider->setSingleStep(iStep);


            int iInterval = (iMax - iMin) / 20;
            pSlider->setTickInterval(iInterval);
            pSlider->setPageStep(iInterval);

            /*
            oReturn.bHasRetriever = true;
            oReturn.oRetriever = [iFactor, pSlider] () {

                return std::to_string(pSlider->value() / iFactor);

            };
             */

            pSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            /*
             *
             * Assemble consturct
             *
             */

            std::stringstream oMinSS;
            oMinSS << std::setprecision(std::ceil(log10(fMin) + log10(iMinFactor)) + 1) << fMin;

            std::stringstream oMaxSS;
            oMaxSS << std::setprecision(std::ceil(log10(fMax) + log10(iMaxFactor)) + 1) << fMax;


            QWidget *pSliderWidget = new QWidget();
            QLabel *pLeftSliderValue = new QLabel(QString(oMinSS.str().c_str()));
            QLabel *pRightSliderValue = new QLabel(QString(oMaxSS.str().c_str()));

            QLineEdit *pLineEdit = new QLineEdit("0");
            pLineEdit->setValidator(new QDoubleValidator());

            pLineEdit->setMaxLength(std::ceil(log10(fMax) + log10(iMaxFactor) + log10(fStep) + log10(iStepFactor)) + 1);
            pLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

            QObject::connect(pSlider, &QSlider::valueChanged, [pLineEdit, iFactor, pSlider]() {

                float fValue = (float) pSlider->value() / (float) iFactor;

                std::stringstream oValSS;
                int iValueFactor = Maths<float>::getIntegerFactor(fValue);
                oValSS << std::setprecision(std::ceil(log10(fValue) + log10(iValueFactor)) + 1) << fValue;

                pLineEdit->setText(QString(oValSS.str().c_str()));

            });

            QObject::connect(pLineEdit, &QLineEdit::textChanged, [iFactor, pLineEdit, pSlider]() {

                int iPosition = pLineEdit->text().toFloat() * iFactor;

                pSlider->setValue(iPosition);
            });



            oReturn.addRetriever(this->getDomID(pDOMElement) , [pLineEdit]() { return pLineEdit->text().toStdString(); });

            QLayout *pLayout = new QHBoxLayout();
            pLayout->addWidget(pLeftSliderValue);
            pLayout->addWidget(pSlider);
            pLayout->addWidget(pRightSliderValue);
            pLayout->addWidget(pLineEdit);

            pSliderWidget->setLayout(pLayout);

            oReturn.bHasChildrenFinished = true;

            return oReturn;

        }
    }


protected:

        virtual void addNodeAttributes(std::vector<std::string>& vAttributes)
        {

            vAttributes.push_back( "min" );
            vAttributes.push_back( "max" );
            vAttributes.push_back( "step" );

        }


    };

#endif //BIOGUI_WINDOWWIDGETSLIDERNODE_H
