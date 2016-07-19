//
// Created by joppich on 4/27/16.
//

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
