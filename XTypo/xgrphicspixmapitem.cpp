#include "xgrphicspixmapitem.h"
#include "xtransform.h"
#include <QApplication>

XGraphicsPixmapItem::XGraphicsPixmapItem(XGraphicsScene* parentScene, XLayer* containinglayer, QString path)
{
    imagepath = path;
    XGraphicsItem::type=PixmapType;
    scene = parentScene;
    this->layer = containinglayer;
    pixmap = new QPixmap(path);
    painter = new QPainter;
    geometry = new QRectF(0,0,pixmap->width(),pixmap->height());
    isSelected= false;
    op = .5;
    actScale = 1;
    positionBeforeTrsnform = NULL;
    sizeBeforeTransForm = NULL;
}

XGraphicsPixmapItem::XGraphicsPixmapItem(XGraphicsScene *parentScene, XLayer *containinglayer, XGraphicsPixmapItem *original, QPointF* offset)
{
    XGraphicsItem::type=PixmapType;
    scene = parentScene;
    this->layer = containinglayer;
    this->imagepath = original->imagepath;
    pixmap = new QPixmap(*original->pixmap);
    painter = new QPainter;
    geometry = new QRectF(original->geometry->topLeft() + *offset,original->geometry->bottomRight() + *offset);
    isSelected= false;
    op = original->op;
    actScale = original->actScale;
    positionBeforeTrsnform = NULL;
    sizeBeforeTransForm = NULL;
}

QRectF XGraphicsPixmapItem::boundingRect() const
{
    return *geometry;
}

void XGraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    if(!layer->isActive())
        return;
    painter->setOpacity(op);
    if(isSelected){
        painter->setOpacity(0.5);
        QBrush br(Qt::red);
        painter->setBrush(br);
        painter->drawRect(*this->geometry);
    }
    painter->drawImage( *this->geometry,pixmap->scaled(pixmap->width()*actScale,pixmap->height()*actScale).toImage());

}
void XGraphicsPixmapItem::setTranparency(double opacity)
{
    op = opacity;

}

void XGraphicsPixmapItem::setOrigPos(QPointF* op)
{
    if(this->positionBeforeTrsnform)
        delete positionBeforeTrsnform;
    positionBeforeTrsnform = new QPointF(*op);
    sizeBeforeTransForm = new QSizeF(this->geometry->width(),this->geometry->height());
}

void XGraphicsPixmapItem::mousePressEvent(QMouseEvent *me)
{

    if(this->isSelected){
        this->scene->selectedPixmaps.removeAll(this);
        this->isSelected = false;
    }
    else{
        if(QApplication::keyboardModifiers()!=Qt::ControlModifier){

            this->scene->clearShapeSelection();
        }
        this->scene->selectedPixmaps.append(this);
        this->isSelected = true;
        if(positionBeforeTrsnform)
            delete positionBeforeTrsnform;
        this->positionBeforeTrsnform = new QPointF(this->pos());
        if(sizeBeforeTransForm)
            delete sizeBeforeTransForm;
        this->sizeBeforeTransForm = new QSizeF(this->geometry->width(),this->geometry->height());
    }
}

void XGraphicsPixmapItem::setPositionBeforeTransform()
{
    if(this->positionBeforeTrsnform)
        *this->positionBeforeTrsnform = this->pos();
    if(this->sizeBeforeTransForm)
        *this->sizeBeforeTransForm = QSizeF(this->geometry->width(),this->geometry->height());
}

QPointF *XGraphicsPixmapItem::getMiddlePoint()
{
    return new QPointF(this->pos().x()+this->geometry->width()/2,this->pos().y()+this->geometry->height()/2);

}

void XGraphicsPixmapItem::rotate(double deg)
{
    QPointF* cp = new QPointF(this->pos().x()+this->pixmap->width()/2,this->pos().y()+this->pixmap->height()/2);
    QTransform* tr = XTransform::rotate(cp,deg);
    *this->pixmap = this->pixmap->transformed(*tr);

    this->update();
}

void XGraphicsPixmapItem::move(/*QPointF* op,*/QVector2D* dir)
{
    this->setPos(positionBeforeTrsnform->x()+dir->x()/2,positionBeforeTrsnform->y()+dir->y()/2);
    *this->geometry = QRectF(this->pos().x(),this->pos().y(),this->geometry->width(),this->geometry->height());
    this->update();
}

void XGraphicsPixmapItem::scale(QPointF* movePoint, double scale)
{
     QTransform* tr = XTransform::scale(movePoint,scale);
     QPointF topLeftPoint = *this->positionBeforeTrsnform;
     QPointF  bottomRightToTopLeftPoint(this->sizeBeforeTransForm->width(),this->sizeBeforeTransForm->height());
     QPointF bottomRightPoint = topLeftPoint + bottomRightToTopLeftPoint;
     *this->geometry = QRectF(topLeftPoint*(*tr),bottomRightPoint*(*tr));
     this->actScale =scale;
     this->update();
}
