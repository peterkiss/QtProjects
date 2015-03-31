#ifndef XLINE_H
#define XLINE_H

#include "xgraphicsitem.h"
#include <QGraphicsLineItem>

class XGraphicsLineItem : public XGraphicsItem, public QGraphicsLineItem
{
public:
    XGraphicsLineItem(const QLineF & line, QGraphicsItem * parent = 0 );
};

#endif // XLINE_H
