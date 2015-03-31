#ifndef XRECTANGLEITEM_H
#define XRECTANGLEITEM_H

#include "xboxedgraphicsitem.h"
//class XGraphicsItem;
//class QPointF;
//class QPainterPath;
//class QPainter;
//class QRectF;
//class QList;
//class QLineF;

class XRectangleItem : public XBoxedGraphicsItem
{
public:
    XRectangleItem(QPointF p1,QPointF p2,XGraphicsScene* parentScene,XLayer* layer) ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    void moveSecondBoxPoint(QPointF pt);
//    QRectF boundingRect() const;
//    QList<QLineF*> *getHorizontalConstraintLines(QPointF pos);
//    QList<QLineF*> *getVerticalConstraintLines(QPointF pos);
//    QPointF* getFirstBoxPoint();
//    QPointF* getSecondBoxPoint();
//    void moveSecondBoxPoint(QPointF pt);
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
//public slots:
//    void mousePressEvent(QPointF pt) ;
//    void mouseMoveEvent(QPointF pt, bool moveOther);
//    void mouseReleaseEvent(QMouseEvent * event);
//    void moveHandlePoint(QPointF handlePos);

//private:
//    QPointF* firstBoxPoint,*secondBoxPoint;
//    QPainter* painter;
//    QPointF* actPoint;
    //    QPainterPath* path;
    XGraphicsScene* scene;
};
//#endif
#endif // XRECTANGLEITEM_H
