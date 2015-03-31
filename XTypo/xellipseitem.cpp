

#include "xellipseitem.h"


XEllipseItem::XEllipseItem(QPointF p1, QPointF p2, XGraphicsScene *parentScene, XLayer *layer)
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

void XEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}

void XEllipseItem::moveSecondBoxPoint(QPointF pt)
{
    double x = getFirstBoxPoint().x();
    double y = getFirstBoxPoint().y() ;
    double w = getSecondBoxPoint().x()-getFirstBoxPoint().x();
    double h = getSecondBoxPoint().y()-getFirstBoxPoint().y();
    double kappa = .5522848;
          double ox = (w / 2) * kappa;// control point offset horizontal
          double oy = (h / 2) * kappa; // control point offset vertical
          double xe = x + w;           // x-end
          double ye = y + h;           // y-end
          double xm = x + w / 2;       // x-middle
          double ym = y + h / 2;       // y-middle
//          ox = (w / 2) * kappa, // co
//      ctx.moveTo(x, ym);
//      ctx.bezierCurveTo(x, ym - oy, xm - ox, y, xm, y);
//      ctx.bezierCurveTo(xm + ox, y, xe, ym - oy, xe, ym);
//      ctx.bezierCurveTo(xe, ym + oy, xm + ox, ye, xm, ye);
//      ctx.bezierCurveTo(xm - ox, ye, x, ym + oy, x, ym);
          *secondBoxPoint = pt;
          curves.at(0)->getFirstEndPoint()= QPointF(x,ym);
          curves.at(0)->getFirstHandlePoint()= QPointF(x,ym-oy);
          curves.at(0)->getSecondHandlePoint()=QPointF(xm-ox,y);
          curves.at(0)->getSecondEndPoint()= QPointF(xm,y);

          curves.at(1)->getFirstEndPoint()= QPointF(curves.at(0)->getSecondEndPoint());
          curves.at(1)->getFirstHandlePoint()= QPointF(xm + ox, y);
          curves.at(1)->getSecondEndPoint()= QPointF(xe, ym);
          curves.at(1)->getSecondHandlePoint()= QPointF(xe, ym - oy);


          curves.at(2)->getFirstEndPoint()= QPointF(curves.at(1)->getSecondEndPoint());
          curves.at(2)->getFirstHandlePoint()= QPointF(xe, ym + oy);
          curves.at(2)->getSecondEndPoint()= QPointF(xm, ye);
          curves.at(2)->getSecondHandlePoint()= QPointF(xm + ox, ye);

          curves.at(3)->getFirstEndPoint()= QPointF(curves.at(2)->getSecondEndPoint());
          curves.at(3)->getFirstHandlePoint()= QPointF(xm - ox, ye);
          curves.at(3)->getSecondHandlePoint()= QPointF( x, ym + oy);
          curves.at(3)->getSecondEndPoint()= QPointF(x, ym);

//    *secondBoxPoint = pt;
//    curves.at(0)->getFirstEndPoint()= QPointF((secondBoxPoint->x() + firstBoxPoint->x())/2,firstBoxPoint->y());
//    curves.at(0)->getFirstHandlePoint()= QPointF(firstBoxPoint->x()+(secondBoxPoint->x() -firstBoxPoint->x())/2*,firstBoxPoint->y());
//    curves.at(0)->getSecondHandlePoint()=QPointF(secondBoxPoint->x(),firstBoxPoint->y());
//    curves.at(0)->getSecondEndPoint()= QPointF(secondBoxPoint->x(),(secondBoxPoint->y() + firstBoxPoint->y()/2));

//    curves.at(1)->getFirstEndPoint()= QPointF(curves.at(0)->getSecondEndPoint());
//    curves.at(1)->getFirstHandlePoint()= QPointF(*secondBoxPoint);
//    curves.at(1)->getSecondEndPoint()= QPointF((secondBoxPoint->x() + firstBoxPoint->x())/2,secondBoxPoint->y());
//    curves.at(1)->getSecondHandlePoint()= QPointF(*secondBoxPoint);


//    curves.at(2)->getFirstEndPoint()= QPointF(curves.at(1)->getSecondEndPoint());
//    curves.at(2)->getFirstHandlePoint()= QPointF(getFirstBoxPoint().x(),getSecondBoxPoint().y ()  );
//    curves.at(2)->getSecondEndPoint()= QPointF(getFirstBoxPoint().x(),(getFirstBoxPoint().y() + getSecondBoxPoint().y()    )/2);
//    curves.at(2)->getSecondHandlePoint()= QPointF(getFirstBoxPoint().x(),getSecondBoxPoint().y ());

//    curves.at(3)->getFirstEndPoint()= QPointF(curves.at(2)->getSecondEndPoint());
//    curves.at(3)->getFirstHandlePoint()= QPointF(getFirstBoxPoint());
//    curves.at(3)->getSecondEndPoint()= QPointF(getFirstBoxPoint());
//    curves.at(3)->getSecondEndPoint()= QPointF(curves.at(0)->getFirstEndPoint());
}
