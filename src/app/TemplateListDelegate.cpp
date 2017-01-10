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

#include <QtCore/QFileInfo>
#include "TemplateListDelegate.h"
#include <QDebug>

TemplateListDelegate::TemplateListDelegate(QObject *parent)
{

}

void TemplateListDelegate::paint (QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    QRect r = option.rect;

    //Color: #C4C4C4
    QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);

    //Color: #005A83
    QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);

    //Color: #333
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    //Color: #fff
    QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);

    if(option.state & QStyle::State_Selected){
        QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
        gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
        gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
        gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
        painter->setBrush(gradientSelected);
        painter->drawRect(r);

        //BORDER
        painter->setPen(lineMarkedPen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontMarkedPen);

    } else {
        //BACKGROUND
        //ALTERNATING COLORS
        painter->setBrush( (index.row() % 2) ? Qt::white : QColor(252,252,252) );
        painter->drawRect(r);

        //BORDER
        painter->setPen(linePen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontPen);
    }

    //GET TITLE, DESCRIPTION AND ICON
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 1).toString();
    QString filelocation = index.data(Qt::UserRole + 2).toString();

    int imageSpace = 10;
    if (!ic.isNull()) {
        //ICON
        r = option.rect.adjusted(5, 10, -10, -10);
        ic.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
        imageSpace = 75;
    }

    QFont oInfoFont( "Lucida Grande", -1, QFont::Normal );

    //TITLE
    r = option.rect.adjusted(imageSpace, -25, -10, -40);
    painter->setFont( oInfoFont );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, title, &r);

    //DESCRIPTION
    r = option.rect.adjusted(imageSpace, -10, -10, -25);
    painter->setFont( oInfoFont );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, description, &r);

    QFont oFileFont(oInfoFont);
    oFileFont.setItalic(true);

    QFontMetrics fontMetrics(oFileFont);
    int iRightBorder = 30;
    QString sElidedFileLocation = fontMetrics.elidedText(filelocation, Qt::ElideLeft, 300);


    //File
    r = option.rect.adjusted(imageSpace, 5, -1, -10);
    painter->setFont( oFileFont );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, sElidedFileLocation, &r);
}

QSize TemplateListDelegate::sizeHint (const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    return QSize(200, 60); // very dumb value
}

