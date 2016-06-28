//
// Created by joppich on 4/6/16.
//

#ifndef BIOGUI_ADVANCEDSTREAMBOXDELEGATE_H
#define BIOGUI_ADVANCEDSTREAMBOXDELEGATE_H


#include <QPainter>
#include <QAbstractItemDelegate>

class TemplateListDelegate : public QAbstractItemDelegate
{
public:
    TemplateListDelegate(QObject *parent = 0);

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    ~TemplateListDelegate()
    {

    }

};


#endif //BIOGUI_ADVANCEDSTREAMBOXDELEGATE_H
