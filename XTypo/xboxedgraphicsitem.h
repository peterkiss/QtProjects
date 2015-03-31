#ifndef XBOXEDGRAPHICSITEM_H
#define XBOXEDGRAPHICSITEM_H

#include "xgraphicsitem.h"
#include <QList>
#include "beziercurve.h"

#include <QList>
#include <QPainterPath>
#include <QPainter>

//class XGraphicsItem;
class QPointF;
class QPainterPath;
class QPainter;
class QRectF;
//class QList;
class QLineF;

class XBoxedGraphicsItem : public XGraphicsItem
{
public:
    XBoxedGraphicsItem(){}
    XBoxedGraphicsItem(QPointF p1,QPointF p2,XGraphicsScene* parentScene,XLayer* layer);

        QRectF boundingRect() const;
        QList<QLineF*> *getHorizontalConstraintLines(QPointF pos);
        QList<QLineF*> *getVerticalConstraintLines(QPointF pos);
        QPointF& getFirstBoxPoint();
        QPointF& getSecondBoxPoint();
        void moveSecondBoxPoint(QPointF pt);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    public slots:
        void mousePressEvent(QPointF pt) ;
        void mouseMoveEvent(QPointF pt, bool moveOther);
        void mouseReleaseEvent(QMouseEvent * event);
        void moveHandlePoint(QPointF handlePos);

    protected:
        QPointF* firstBoxPoint,*secondBoxPoint;
        QPointF* actPoint;
        QPainterPath* path;
        QPainter* painter;
        QList<BezierCurve*> curves;


};

#endif // XBOXEDGRAPHICSITEM_H
