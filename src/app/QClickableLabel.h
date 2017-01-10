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

#ifndef BIOGUI_QCLICKABLELABEL_H
#define BIOGUI_QCLICKABLELABEL_H

#include <QLabel>

class QClickableLabel : public QLabel
{
    Q_OBJECT
public:
    QClickableLabel( const QString & text, QWidget * parent = 0 )
    : QLabel(text, parent)
    {

    }

    ~QClickableLabel(){}

signals:
    void clicked();

protected:

    virtual void mousePressEvent ( QMouseEvent * event )
    {
        emit clicked();
    }

};

#endif //BIOGUI_QCLICKABLELABEL_H
