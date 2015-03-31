#include "xboxedgraphicsitem.h"

//#include <QList>
//#include <QPainterPath>
//#include <QPainter>
//#ifdef newShapes
XBoxedGraphicsItem::XBoxedGraphicsItem(QPointF p1, QPointF p2, XGraphicsScene *parentScene, XLayer *layer)
{
    painter = new QPainter;
    path = new QPainterPath;
    firstBoxPoint = new QPointF(p1);
    secondBoxPoint = new QPointF(p2);
}

QRectF XBoxedGraphicsItem::boundingRect() const
{
    return QRectF(*firstBoxPoint,*secondBoxPoint);
}

QPointF &XBoxedGraphicsItem::getFirstBoxPoint()
{
    return *firstBoxPoint;
}

QPointF &XBoxedGraphicsItem::getSecondBoxPoint()
{
    return *secondBoxPoint;
}

void XBoxedGraphicsItem::moveSecondBoxPoint(QPointF pt)
{
    *secondBoxPoint = pt;
}

void XBoxedGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    this->path->addRect(QRectF(getFirstBoxPoint(),getSecondBoxPoint()));
    painter->drawPath(*path);
}
