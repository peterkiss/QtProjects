#ifndef XGRPHICSPIXMAPITEM_H
#define XGRPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem >
#include "xgraphicsitem.h"
#include "xgraphicsscene.h"


class XGraphicsPixmapItem : public /*QGraphicsPixmapItem,*/ XGraphicsItem
{
public:
   XGraphicsPixmapItem(XGraphicsScene* parentScene, XLayer* containinglayer,QString path);
   XGraphicsPixmapItem(XGraphicsScene *parentScene, XLayer *containinglayer, XGraphicsPixmapItem *original, QPointF* offset);
    QRectF	boundingRect () const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    void rotate(double deg);
    void move(QVector2D *dir);
    void scale(QPointF *movePoint, double scale);
    void setTranparency(double opacity);
    void setSelected(bool s){isSelected=s;}
    void setOrigPos(QPointF *op);
    void mousePressEvent(QMouseEvent* me);
    void setPositionBeforeTransform();
    QPointF* getMiddlePoint();
    QString getImagePath(){return this->imagepath;}
    QRectF* getGeometry(){return this->geometry;}
    double getWidth() {return this->pixmap->width();}
    double getHeight() {return this->pixmap->height();}
    double actScale;
private:
    double op;
    bool isSelected;
    QPointF*  positionBeforeTrsnform;
    QSizeF*      sizeBeforeTransForm;
    QRectF* geometry;
    QPainter* painter;
    QPixmap* pixmap;
    XGraphicsScene* scene;
    QString imagepath;
};

#endif // XGRPHICSPIXMAPITEM_H
