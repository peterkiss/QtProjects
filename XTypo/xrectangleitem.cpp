#include "xrectangleitem.h"
#include "xboxedgraphicsitem.h"
#include <QList>
#include <QPainterPath>
#include <QPainter>

XRectangleItem::XRectangleItem(QPointF p1, QPointF p2, XGraphicsScene *parentScene, XLayer *layer)
{

    painter = new QPainter;
    path = new QPainterPath;
    firstBoxPoint = new QPointF(p1);
    secondBoxPoint = new QPointF(p2);
    scene=parentScene;
    curves.append(new BezierCurve(p1,p2,scene,layer));
    curves.append(new BezierCurve(p1,p2,scene,layer,curves.at(0)));
    curves.append(new BezierCurve(p1,p2,scene,layer,curves.at(1)));
    curves.append(new BezierCurve(p1,p2,scene,layer,curves.at(2)));
    curves.at(0)->SetPreviousCurve(curves.at(3));
    scene->addItem(curves.at(0));
    scene->addItem(curves.at(1));
    scene->addItem(curves.at(2));
    scene->addItem(curves.at(3));
    scene->StartCurves.append(curves.at(0));
     curves.at(0)->isFirstCurve = true;



}

void XRectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}

void XRectangleItem::moveSecondBoxPoint(QPointF pt)
{
    *secondBoxPoint = pt;
    curves.at(0)->getSecondEndPoint()= QPointF(pt.x(),curves.at(0)->getFirstEndPoint().y());
    curves.at(0)->getSecondHandlePoint()= QPointF(curves.at(0)->getSecondEndPoint());

    curves.at(1)->getFirstEndPoint()= QPointF(curves.at(0)->getSecondEndPoint());
    curves.at(1)->getFirstHandlePoint()= QPointF(curves.at(1)->getFirstEndPoint());
    curves.at(1)->getSecondEndPoint()= QPointF(pt.x(),pt.y());
    curves.at(1)->getSecondHandlePoint()= QPointF(curves.at(1)->getSecondEndPoint());


    curves.at(2)->getFirstEndPoint()= QPointF(curves.at(1)->getSecondEndPoint());
    curves.at(2)->getFirstHandlePoint()= QPointF(curves.at(2)->getFirstEndPoint());
    curves.at(2)->getSecondEndPoint()= QPointF(firstBoxPoint->x(),pt.y());
    curves.at(2)->getSecondHandlePoint()= QPointF(curves.at(2)->getSecondEndPoint());

    curves.at(3)->getFirstEndPoint()= QPointF(curves.at(2)->getSecondEndPoint());
    curves.at(3)->getFirstHandlePoint()= QPointF(curves.at(3)->getFirstEndPoint());
    curves.at(3)->getSecondEndPoint()= QPointF(*firstBoxPoint);


}
