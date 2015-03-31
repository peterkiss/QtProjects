#ifndef XELLIPSEITEM_H
#define XELLIPSEITEM_H
#include "xgraphicsitem.h"
#include "xboxedgraphicsitem.h"
//class QPointF;
//class QPainterPath;
//class QPainter;
//class QRectF;
//class QList;
//class QLineF;


class XEllipseItem : public XBoxedGraphicsItem
{
public:
    XEllipseItem(QPointF p1,QPointF p2,XGraphicsScene* parentScene,XLayer* layer);/*: XBoxedGraphicsItem( p1, p2, parentScene, layer){}*/
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    void moveSecondBoxPoint(QPointF pt);
    XGraphicsScene* scene;
//    QList<QLineF*> *getHorizontalConstraintLines(QPointF pos);
//    QList<QLineF*> *getVerticalConstraintLines(QPointF pos);
//public slots:
//    void mousePressEvent(QPointF pt) ;
//    void mouseMoveEvent(QPointF pt, bool moveOther);
//    void mouseReleaseEvent(QMouseEvent * event);
//    void moveHandlePoint(QPointF handlePos);

//private:
//    QRectF* bounding;
//    QList<QPointF*> points;
//    QPointF* actPoint;
//    QPainterPath* path;
//    QPainter* painter;
};
//#endif
#endif // XELLIPSEITEM_H
