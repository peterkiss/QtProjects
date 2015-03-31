#include "beziercurve.h"

const double BezierCurve::ellipseD=10;

extern const int CatchCurveDistance;
BezierCurve::BezierCurve(QPointF p1, QPointF p2, XGraphicsScene *parentScene, XLayer *layer, BezierCurve* prev)
{
    this->layer = layer;
    isSelected=false;
    isFirstCurve = false;
    isActive=true;
    containingScene= parentScene;
    type=BezierCurveType;
    actPoint=NULL;
    editing = true;
    nearestCurvePoint=NULL;
    controlPoints.append(p1);
    if(prev)
        controlPoints.append(QPointF(p1 + (p1 - prev->getSecondHandlePoint())));
    else
        controlPoints.append(/*QPointF(p1.x()+(p2.x()-p1.x())*1/3,p1.y()+(p2.y()-p1.y())*1/3)*/p1);
    controlPoints.append(/*QPointF(p1.x()+(p2.x()-p1.x())*2/3,p1.y()+(p2.y()-p1.y())*2/3)*/p2);
    controlPoints.append(p2);


    if(prev){
        prev->nextCurve=this;
        previousCurve=prev;
    }
    else
        previousCurve=NULL;
    nextCurve=NULL;

}
BezierCurve::BezierCurve(QPointF p1, QPointF p2, QPointF p3, QPointF p4, XGraphicsScene *parentScene, XLayer *layer)
{
    this->layer=layer;
    isFirstCurve = false;
    isSelected=false;
    isActive=true;
    containingScene= parentScene;
    type=BezierCurveType;
    controlPoints.append(p1);
    controlPoints.append(p2);
    controlPoints.append(p3);
    controlPoints.append(p4);
    actPoint=NULL;
    nearestCurvePoint=NULL;
    previousCurve=NULL;
    nextCurve=NULL;
}

BezierCurve::BezierCurve(QPointF* offset, BezierCurve* originalCurve,XLayer* actlayer,XGraphicsScene* scene)
{
    this->layer=actlayer;
    isSelected=false;
    isActive=false;
    isFirstCurve = originalCurve->isFirstCurve;
    containingScene= scene;
    type=BezierCurveType;
    this->controlPoints.append(QPointF(originalCurve->getFirstEndPoint()     + *offset));
    this->controlPoints.append(QPointF(originalCurve->getFirstHandlePoint()  + *offset));
    this->controlPoints.append(QPointF(originalCurve->getSecondHandlePoint() + *offset));
    this->controlPoints.append(QPointF(originalCurve->getSecondEndPoint()    + *offset));
    actPoint=NULL;
    nearestCurvePoint=NULL;
    previousCurve=NULL;
    nextCurve=NULL;
}


BezierCurve* BezierCurve::devideCurve(){
    double t=actT;
    double x1 = getFirstEndPoint().x(), y1 = getFirstEndPoint().y();
    double x2 = getFirstHandlePoint().x(), y2 = getFirstHandlePoint().y();
    double x3=getSecondHandlePoint().x(), y3 = getSecondHandlePoint().y();
    double x4=getSecondEndPoint().x(), y4 = getSecondEndPoint().y();

    double x12 = (x2-x1)*t+x1;
    double y12 = (y2-y1)*t+y1;

    double x23 = (x3-x2)*t+x2;
    double y23 = (y3-y2)*t+y2;

    double x34 = (x4-x3)*t+x3;
    double y34 = (y4-y3)*t+y3;

    double x123 = (x23-x12)*t+x12;
    double y123 = (y23-y12)*t+y12;

    double x234 = (x34-x23)*t+x23;
    double y234 = (y34-y23)*t+y23;

    double x1234 = (x234-x123)*t+x123;
    double y1234 = (y234-y123)*t+y123;
    //    tempPoints.append(QPointF(x1, y1));
    //    tempPoints.append(QPointF(x12, y12));
    //    tempPoints.append(QPointF(x123, y123));
    //    tempPoints.append(QPointF(x1234, y1234));

    t=1-t;
    x4 = getFirstEndPoint().x(), y4 = getFirstEndPoint().y();
    x3 = getFirstHandlePoint().x(), y3 = getFirstHandlePoint().y();
    x2 = getSecondHandlePoint().x(), y2 = getSecondHandlePoint().y();
    x1 = getSecondEndPoint().x(), y1 = getSecondEndPoint().y();

    this->getFirstHandlePoint()=QPointF(x12, y12);
    this->getSecondHandlePoint()=QPointF(x123, y123);
    this->getSecondEndPoint()=QPointF(x1234, y1234);

    x12 = (x2-x1)*t+x1;
    y12 = (y2-y1)*t+y1;

    x23 = (x3-x2)*t+x2;
    y23 = (y3-y2)*t+y2;

    x34 = (x4-x3)*t+x3;
    y34 = (y4-y3)*t+y3;

    x123 = (x23-x12)*t+x12;
    y123 = (y23-y12)*t+y12;

    x234 = (x34-x23)*t+x23;
    y234 = (y34-y23)*t+y23;

    x1234 = (x234-x123)*t+x123;
    y1234 = (y234-y123)*t+y123;

    //     tempPoints.append(QPointF(x1, y1));
    //     tempPoints.append(QPointF(x12, y12));
    BezierCurve* newCurve = new BezierCurve(QPointF(x1234,y1234),QPointF(x123, y123),QPointF(x12, y12),QPointF(x1, y1),containingScene,this->layer);
    nextCurve->setPreviousCurve(newCurve);
    newCurve->setNextCurve(nextCurve);
    newCurve->setPreviousCurve(this);
    setNextCurve(newCurve);
    //     tempPoints.append(QPointF(x123, y123));
    //     tempPoints.append(QPointF(x1234, y1234));
    this->update();


    return  newCurve;

    //return [(x1, y1), (x12, y12), (x123, y123), (x1234, y1234)]
}

QList<QLineF*>* BezierCurve::getHorizontalConstraintLines(QPointF pos)
{

    QList<QLineF*>* list=new QList<QLineF*>;
    if(editing)
        return list;
    if(abs(pos.y()-getFirstEndPoint().y())<20)
        list->append(new QLineF(getFirstEndPoint(),QPointF(pos.x()<getFirstEndPoint().x()?pos.x()-100:pos.x()+100,getFirstEndPoint().y())));
    if(abs(pos.y()-getSecondEndPoint().y())<20)
        list->append(new QLineF(getSecondEndPoint(),QPointF(pos.x()<getSecondEndPoint().x()?pos.x()-100:pos.x()+100,getSecondEndPoint().y())));
    return list;
}

QList<QLineF *> *BezierCurve::getVerticalConstraintLines(QPointF pos)
{
    QList<QLineF*>* list=new QList<QLineF*>;
    if(editing)
        return list;
    if(abs(pos.x()-getFirstEndPoint().x())<20)
        list->append(new QLineF(getFirstEndPoint(),QPointF(getFirstEndPoint().x(),pos.y()<getFirstEndPoint().y()?pos.y()-100:pos.y()+100)));
    if(abs(pos.x()-getSecondEndPoint().x())<20)
        list->append(new QLineF(getSecondEndPoint(),QPointF(getSecondEndPoint().x(),pos.y()<getSecondEndPoint().y()?pos.y()-100:pos.y()+100)));
    return list;
}

QPainterPath *BezierCurve::getPath()
{
    QPainterPath* CurvePath = new QPainterPath;
    //   if(this->controlPoints.size()==4){
    CurvePath->moveTo(getFirstEndPoint());
    CurvePath->cubicTo(getFirstHandlePoint(), getSecondHandlePoint(),getSecondEndPoint());
    //   }
    //        CurvePath->moveTo(controlPoints[0].x(),controlPoints[0].y());
    //        CurvePath->cubicTo(controlPoints[1].x(),controlPoints[1].y(), controlPoints[2].x(),controlPoints[2].y(),controlPoints[3].x(),controlPoints[3].y());
    return CurvePath;
}

QList<QPointF> *BezierCurve::getControlPoints()
{
    return &this->controlPoints;
}

void BezierCurve::selectThisShape()
{
    isSelected = true;
    this->containingScene->selectedCurves.append(this);

    this->containingScene->selectedPoints.append(&controlPoints[0]);
    this->containingScene->selectedPoints.append(&controlPoints[1]);
    this->containingScene->selectedPoints.append(&controlPoints[2]);
    this->containingScene->selectedPoints.append(&controlPoints[3]);
    BezierCurve* act = nextCurve;
    while(act != this)
    {
        this->containingScene->selectedCurves.append(act);
        act->containingScene->selectedPoints.append(&act->controlPoints[0]);
        act->containingScene->selectedPoints.append(&act->controlPoints[1]);
        act->containingScene->selectedPoints.append(&act->controlPoints[2]);
        act->containingScene->selectedPoints.append(&act->controlPoints[3]);
        act->isSelected=true;
        act = act->nextCurve;
    }

}

void BezierCurve::deSelectThisShape()
{
    isSelected = false;
    this->containingScene->selectedCurves.removeAll(this);

    this->containingScene->selectedPoints.removeAll(&controlPoints[0]);
    this->containingScene->selectedPoints.removeAll(&controlPoints[1]);
    this->containingScene->selectedPoints.removeAll(&controlPoints[2]);
    this->containingScene->selectedPoints.removeAll(&controlPoints[3]);
    BezierCurve* act = nextCurve;
    while(act != this)
    {
        this->containingScene->selectedCurves.removeAll(act);
        act->containingScene->selectedPoints.removeAll(&act->controlPoints[0]);
        act->containingScene->selectedPoints.removeAll(&act->controlPoints[1]);
        act->containingScene->selectedPoints.removeAll(&act->controlPoints[2]);
        act->containingScene->selectedPoints.removeAll(&act->controlPoints[3]);
        act->isSelected=false;
        act = act->nextCurve;
    }
}

void BezierCurve::selectThisCurve()
{
    isSelected = true;
    this->containingScene->selectedCurves.append(this);
}

void BezierCurve::deSelectThisCurve()
{
    isSelected = false;
    this->containingScene->selectedCurves.removeAll(this);
}

//bool BezierCurve::shapeOfThisIsSelected() const
//{
////    if(this->containingScene->selectedCurves.contains(this))
////        return true;
////    BezierCurve* act = nextCurve;
////    while(act != this)
////    {
////        if(this->containingScene->selectedCurves.contains(act))
////            return true;
////    }
////    return false;
//    return isSelected;
//}
void BezierCurve::setNextCurve(BezierCurve* next)
{
    nextCurve=next;
}
void BezierCurve::setPreviousCurve(BezierCurve* prev)
{
    previousCurve=prev;
}

void BezierCurve::moveHandlePoint(QPointF handlePos)
{
    controlPoints[2]=controlPoints[3] + controlPoints[3] -   handlePos;
    this->update();
}

void BezierCurve::moveEndPoint(QPointF newEndPoint)
{
    controlPoints[3]=newEndPoint;
    //if(previousCurve==NULL)
    //    controlPoints[1]=QPointF( controlPoints[0].x()+( controlPoints[3].x()- controlPoints[0].x())*1/3, controlPoints[0].y()+( controlPoints[3].y()- controlPoints[0].y())*1/3);
    controlPoints[2]=newEndPoint;//QPointF( controlPoints[0].x()+( controlPoints[3].x()- controlPoints[0].x())*2/3, controlPoints[0].y()+( controlPoints[3].y()- controlPoints[0].y())*2/3);
    this->update();
}
/**
 * @brief BezierCurve::getNearestControlPoint
 * ret: pt parameterhez leglkozelebbi kontrollpont, tavolsag dist-be irva
 *
 */

QPointF* BezierCurve::getNearestControlPoint(QPointF pt, double& dist){
    actPoint = &controlPoints[0];
    double actDist = 0 ;
    double minDistSqure = pow(controlPoints[0].x()-pt.x(),2) + pow(controlPoints[0].y()-pt.y(),2);
    actPointIsHandle = 0;
    for(int i = 1; i<controlPoints.size();++i)
    {
        actDist=pow(controlPoints[i].x() - pt.x(),2)+pow(controlPoints[i].y() - pt.y(),2);
        if(actDist<minDistSqure){
            minDistSqure=actDist;
            actPoint=&controlPoints[i];
            actPointIsHandle = i;
        }
    }
    dist = minDistSqure;
    return actPoint;
}
/**
 * @brief BezierCurve::getNearestCurvePoint
 * @param pt ehhez legkozelebbi kell
 * @param dist tavolsag visszaadaa
 * @return: pt parameterhez leglkozelebbi gorbepont,
 *
 */
QPointF* BezierCurve::getNearestCurvePoint(QPointF pt, double& dist){
    dist=9999;
    if(nearestCurvePoint)
        delete nearestCurvePoint;
    nearestCurvePoint = new QPointF(getFirstEndPoint());
    QPointF act;
    const double step = 3/length;
    for(double t=0;t<1;t+=step)
    {
        act=pow(1-t,3)*(getFirstEndPoint())+3*pow(1-t,2)*t*(getFirstHandlePoint())+3*(1-t)*pow(t,2)*(getSecondHandlePoint())+pow(t,3)*(getSecondEndPoint());
        double QadraticActDist=pow(pt.x()-act.x(),2)+pow(pt.y()-act.y(),2);
        if(QadraticActDist<dist){
            dist=QadraticActDist;
            *nearestCurvePoint=act;
            actT=t;
        }
    }

    if((dist=sqrt(dist))>5){
        delete  nearestCurvePoint;
        nearestCurvePoint=NULL;

    }
    return nearestCurvePoint;


}
/**
 * @brief BezierCurve::mousePressEvent
 * @param pt ehhez legkozelebbi kell
 * actPoint member beallitasa legkozelebbi pontra
 *
 */
void BezierCurve::mousePressEvent(QPointF pt)
{
    double dummy;
    /* actPoint=*/getNearestControlPoint(pt,  dummy);
    //QPointF pt = QGraphicsItem::mapToItem(pt->pos());
    //    if(pow(controlPoints[1].x()-pt.x(),2)+pow(controlPoints[1].y()-pt.y(),2)<pow(controlPoints[2].x()-pt.x(),2)+pow(controlPoints[2].y()-pt.y(),2))
    //        actPoint=&controlPoints[1];
    //    else
    //        actPoint=&controlPoints[2];
}
/**
 * @brief BezierCurve::mousePressEvent
 * @param pt ehhez legkozelebbi kell
 * @param moveOther mozog-e erintkezo gorbe
 * actPoint mozgatasa pt-be, ha moveOther == true érintkezo gorbe transzformacioja
 *
 */
void BezierCurve::mouseMoveEvent(QPointF pt, bool moveOther)
{
    if(actPoint){
        QPointF orig(*actPoint);
        *actPoint=pt;
        if(moveOther){
            if(actPointIsHandle==1 && previousCurve)
                previousCurve->getSecondHandlePoint() = getFirstEndPoint() + getFirstEndPoint() - getFirstHandlePoint();
            else if(actPointIsHandle==2 && nextCurve)
                nextCurve->getFirstHandlePoint() = getSecondEndPoint() + getSecondEndPoint()- getSecondHandlePoint();
            else if(actPointIsHandle==0 ){
                getFirstHandlePoint()-= orig - pt;
                if(previousCurve){
                    previousCurve->getSecondHandlePoint() -= previousCurve->getSecondEndPoint() - pt;
                    previousCurve->getSecondEndPoint() = pt;
                }
            }
            else if(actPointIsHandle==3){
                getSecondHandlePoint()-= orig - pt;
                if(nextCurve){
                    nextCurve->getFirstHandlePoint() -= nextCurve->getFirstEndPoint() - pt;
                    nextCurve->getFirstEndPoint() = pt;
                }
            }
        }
        else if(actPointIsHandle==0){previousCurve->nextCurve=NULL;previousCurve=NULL;}
        else if(actPointIsHandle==3){nextCurve->previousCurve=NULL;nextCurve=NULL;}


    }
    if(this->previousCurve)
        this->previousCurve->update();
    if(this->nextCurve)
        this->nextCurve->update();
    this->update();
}
void BezierCurve::mouseReleaseEvent(QMouseEvent * event)
{
    actPoint=NULL;
}
/**
 * @brief BezierCurve::getNearestCurvePoint
 * @return: befoglalo teglalap
 *
 */
QRectF BezierCurve::boundingRect() const
{
    int minx=9999999;
    int miny=9999999;
    int maxx=0;
    int maxy=0;
    QListIterator<QPointF> li(controlPoints);
    while (li.hasNext()) {
        QPointF act=li.next();
        if(act.x()<minx)
            minx=act.x();
        if(act.x()>maxx)
            maxx=act.x();
        if(act.y()<miny)
            miny=act.y();
        if(act.y()>maxy)
            maxy=act.y();

    }
    return QRectF( mapFromScene(QPointF(minx-ellipseD,miny-ellipseD)), mapFromScene(QPointF(maxx+ellipseD,maxy+ellipseD)));
}
/**
 * @brief BezierCurve::mousePressEvent
 * rajzolas
 */
void BezierCurve::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

    if(!layer->isActive())
        return;
    int x = 500, y=500;
    QColor fillColor =  Qt::yellow;

    QLinearGradient myGradient;
    QPen myPen(*containingScene->getLineColor());
    QFont myFont;
    QPointF baseline(x, y);
    QPainterPath CurvePath;

    CurvePath.moveTo(controlPoints[0].x(),controlPoints[0].y());
    CurvePath.cubicTo(controlPoints[1].x(),controlPoints[1].y(), controlPoints[2].x(),controlPoints[2].y(),controlPoints[3].x(),controlPoints[3].y());
    length = CurvePath.length();
    QBrush brush(Qt::white);
    // myPen.setBrush(brush);
    myPen.setWidth(2/ containingScene->scaleFactor);
    painter->setPen(myPen);
    if(isSelected)
        painter->setPen(Qt::red);
    painter->drawPath(CurvePath);
    painter->setPen(Qt::red);
    painter->drawLine(controlPoints[0], controlPoints[1]);
    painter->drawLine(controlPoints[2], controlPoints[3]);
    painter->setPen(myPen);
    double rectSize = 4/containingScene->scaleFactor;
    painter->drawRect(controlPoints[0].x()-rectSize/2,controlPoints[0].y()-rectSize/2,rectSize,rectSize);
    painter->drawRect(controlPoints[1].x()-rectSize/2,controlPoints[1].y()-rectSize/2,rectSize,rectSize);
    painter->drawRect(controlPoints[2].x()-rectSize/2,controlPoints[2].y()-rectSize/2,rectSize,rectSize);
    painter->drawRect(controlPoints[3].x()-rectSize/2,controlPoints[3].y()-rectSize/2,rectSize,rectSize);

}
