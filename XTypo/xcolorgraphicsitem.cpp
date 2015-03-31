#include "xcolorgraphicsitem.h"


//XColorGraphicsItem::XColorGraphicsItem(XColorGraphicsItem *other, QGraphicsScene *scene)
//{
//    this->path = new QPainterPath(other->path);
//}
//QPainterPath* XColorGraphicsItem::getPath()
//{
//    return this->path;
//}

XColorGraphicsItem::XColorGraphicsItem(XGraphicsScene* parentscene)
{
    this->scene = parentscene;
    this->layer = scene->getActualLayer();
    maxX=minX=maxY= minY=-1;
    type = ColorType;
    painter = new QPainter;
    fillColor = new QColor(Qt::gray);
    path = NULL;
}

XColorGraphicsItem::XColorGraphicsItem(XGraphicsScene *parentscene, QColor* fillColor/*,QGraphicsItem  *parent = 0*/)/* :
    XGraphicsItem(parent)*/
{
    this->scene = parentscene;
    type = ColorType;
    painter = new QPainter;
    fillColor = new QColor(* fillColor);
    path = new QPainterPath;
}

void XColorGraphicsItem::setColor(const QColor &fillColor)
{
    this->fillColor = new QColor(fillColor);
}

void XColorGraphicsItem::addItem(XGraphicsItem *item)
{
//    if(item->isBezierCurve()){
//        BezierCurve* bc = (BezierCurve*)item;
//        path->moveTo(bc->getFirstEndPoint());
//        path->cubicTo(bc->getFirstHandlePoint(),bc->getSecondHandlePoint(),bc->getSecondEndPoint());

//    }

}

QRectF XColorGraphicsItem::boundingRect() const
{
    if(path)
        return path->boundingRect();
    else
        return QRectF(0,0,0,0);
}

QPainterPath* XColorGraphicsItem::getPath()
{
    if(path)
        delete path;
    if(!bezierPaths.empty())
        qDeleteAll(bezierPaths);
    bezierPaths.clear();
    path = new QPainterPath;
    QListIterator<BezierCurve*> loopFirstIterator(scene->StartCurves);
    while(loopFirstIterator.hasNext()){
        BezierCurve* loopFirstCurve=loopFirstIterator.next();
        BezierCurve* bc = loopFirstCurve;
        if(loopFirstCurve->getLayer()->getName()!="layer0")
            continue;
        do
        {
            QPainterPath* p = new QPainterPath(*bc->getPath());
                bezierPaths.append(p);
            if(bc == loopFirstCurve)
               path->addPath(*p);
            else
                path->connectPath(*p);
            if(bc->getNextCurve())
                bc = bc->getNextCurve();
            else
                break;
        }
        while(bc != loopFirstCurve);
    }
    return path;
}

void XColorGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

//    if(!layer)
//        return;
    if(!layer->isActive())
        return;
//    if(path)
//        delete path;
//    if(!bezierPaths.empty())
//        qDeleteAll(bezierPaths);
//    bezierPaths.clear();
//    path = new QPainterPath;
//    QListIterator<BezierCurve*> loopFirstIterator(scene->StartCurves);
//    while(loopFirstIterator.hasNext()){
//        BezierCurve* loopFirstCurve=loopFirstIterator.next();
//        BezierCurve* bc = loopFirstCurve;

//        do
//        {
//            QPainterPath* p = new QPainterPath(*bc->getPath());
//            bezierPaths.append(p);
//           path->connectPath(*p);
//            if(bc->getNextCurve())
//                bc = bc->getNextCurve();
//            else
//                break;
//        }
//        while(bc != loopFirstCurve);
//    }
//    if(fillColor)
//        delete fillColor;
//    fillColor =new QColor(*scene->getFillColor());
//    painter->setBrush(*fillColor);
//    painter->drawPath(*path);
    getPath();
    if(fillColor)
        delete fillColor;
    fillColor =new QColor(*scene->getFillColor());
    painter->setBrush(*fillColor);
    painter->drawPath(*path);
}
