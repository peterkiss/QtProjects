#include "xtransform.h"
#include <QVector2D>
#include <QDebug>
#include <limits>
XTransform::XTransform()
{
}
//MetTr2d tr2d_mirror (const METPT &p1, const METPT &p2)
//{
//    MetTr2d tr2d;
//    METVEC dv = p2-p1; double r2 = dv.length2 ();
//    if (r2 < TRSm6) {
//        tr2d.m [0][0] = tr2d.m [1][1] = -1.0;
//        tr2d.m [0][1] = tr2d.m [1][0] = 0.0;
//        tr2d.m [0][2] = 2.0 * p1.x;
//        tr2d.m [1][2] = 2.0 * p1.y;
//    } else {
//        r2 = 2/r2;
//        tr2d.m [0][0] = 1 - r2 * dv.vy * dv.vy;
//        tr2d.m [1][1] = 1 - r2 * dv.vx * dv.vx;
//        tr2d.m [0][1] = tr2d.m [1][0] = r2 * dv.vx * dv.vy;
//        tr2d.m [0][2] = r2 * dv.vy * (p1.x * dv.vy - p1.y * dv.vx);
//        tr2d.m [1][2] = r2 * dv.vx * (p1.y * dv.vx - p1.x * dv.vy);
//    }
//    return tr2d;
//}
QTransform *XTransform::reflectY(QPointF *pt1)
{
    QTransform translateToYAxis(1,0,0,1,-pt1->x(),0/*,0,0,1*/);
    QTransform mirror(-1,0,0,1,0,0/*,0,0,1*/);
    QTransform transLateToOriginalPos(1,0,0,1,pt1->x(),0/*,0,0,1*/);
    QTransform* m = new QTransform(translateToYAxis*mirror*transLateToOriginalPos);
   //*m=translateToYAxis*mirror*transLateToOriginalPos;

    return m;
}
QTransform *XTransform::reflectX(QPointF *pt1)
{
    QTransform translateToYAxis(1,0,0,1,0,-pt1->y()/*,0,0,1*/);
    QTransform mirror(1,0,0,-1,0,0/*,0,0,1*/);
    QTransform transLateToOriginalPos(1,0,0,1,0,pt1->y()/*,0,0,1*/);
    QTransform* m = new QTransform(translateToYAxis*mirror*transLateToOriginalPos);
   //*m=translateToYAxis*mirror*transLateToOriginalPos;

    return m;
}

//QTransform *XTransform::reflect(QPointF *pt1, QPointF *pt2)
//{
//    QVector2D v1(*pt1);
//  //  v1.normalize();
//    QVector2D v2(*pt2);
//  //  v2.normalize();
//    QVector2D dv = v2 - v1;
//    double length2=dv.y()*dv.y() + dv.x()*dv.x();
//    double r2 = 2/length2;
//    double m00 = 1 - r2 * dv.y() * dv.y();
//    double m11 = 1 - r2 * dv.x() * dv.x();
//    double m01 = r2 * dv.x() * dv.y();
//    double m10 = m01;
//    double m02 = r2 * dv.y() * (v1.x() * dv.y() - v1.y() * dv.x());
//    double m12 = r2 * dv.x() * (v1.y() * dv.x() - v1.x() * dv.y());
//    qDebug()<<"mtx";
//    qDebug()<<m00<<""<<m01<<" "<<m02;
//    qDebug()<<m10<<""<<m11<<" "<<m12;
//    QTransform* tr=new QTransform(m00,m01,m02,
//                                  m10,m11,m12,
//                                  0,0,0);
//    return tr;
//}



QTransform *XTransform::centeredMirroring(QPointF *cpt)
{

}
//MetTr2d tr2d_rotation (double degree, const METPT *cp)
//{
//    double r = degree*FokToRad, ca = cos (r), sa = sin (r);
//    MetTr2d tr2d;
//    tr2d.m[0][0] = tr2d.m[1][1] = ca;
//    tr2d.m[0][1] = -sa;
//    tr2d.m[1][0] = sa;
//    ca = 1-ca;
//    if (cp) {
//    	tr2d.m[0][2] = cp->x*ca + cp->y*sa;
//    	tr2d.m[1][2] = cp->y*ca - cp->x*sa;
//    } else
//    	tr2d.m[0][2] = tr2d.m[1][2] = 0;
//    return tr2d;
//}
QTransform *XTransform::rotate(QPointF *cpt, double angle)
{
    qDebug()<<"angleNow =" << angle;
    QTransform rotateZ;
    rotateZ.rotate(angle,Qt::ZAxis);
    QTransform moveToOrigo(1,0,0,1,-cpt->x(),-cpt->y());
    QTransform moveBack(1,0,0,1,cpt->x(),cpt->y());
    QTransform* tr = new QTransform(moveToOrigo*rotateZ*moveBack);
    return tr;
}

QTransform *XTransform::move(QVector2D *v)
{
    QTransform* tr = new QTransform(1,0,0,1,v->x(),v->y());
    return tr;
}

QTransform *XTransform::scale(QPointF* cpt,double scale)
{
    qDebug()<<"scaleNow =" << scale;
    QTransform moveToOrigo(1,0,0,1,-cpt->x(),-cpt->y());
    QTransform moveBack(1,0,0,1,cpt->x(),cpt->y());
    QTransform scaling;
    scaling.scale(scale,scale);
    QTransform*tr= new QTransform(moveToOrigo*scaling*moveBack);
    return tr;
}

bool XTransform::isValidCoordinate(qreal value)
{
    if (value != value){
            return false;
        }
        else if (value > std::numeric_limits<qreal>::max()){
            return false;
        }
        else if (value < -std::numeric_limits<qreal>::max()){
            return false;
        }
        else
            return true;
}
//MetTr2d tr2d_scale (double sx, double sy, const METPT *cp)
//{
//    MetTr2d tr2d;
//    tr2d.m [0][1] = tr2d.m [1][0] = 0;
//    tr2d.m [0][0] = sx;
//    tr2d.m [1][1] = sy;
//    if (cp) {
//        tr2d.m [0][2] = cp->x * (1.0 - sx);
//        tr2d.m [1][2] = cp->y * (1.0 - sy);
//    } else
//    	tr2d.m[0][2] = tr2d.m[1][2] = 0;
//    return tr2d;
//}
