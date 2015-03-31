#include "XGraphicsLineItem.h"

XGraphicsLineItem::XGraphicsLineItem(const QLineF &line, QGraphicsItem *parent) : QGraphicsLineItem(line,parent)
{
    XGraphicsItem::type=LineType;
}
