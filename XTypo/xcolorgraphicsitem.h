#ifndef XCOLORGRAPHICSITEM_H
#define XCOLORGRAPHICSITEM_H

#include "xgraphicsitem.h"
#include "beziercurve.h"
#include "xgraphicsscene.h"

class XGraphicsItem;
class XGraphicsScene;
class XColorGraphicsItem : public XGraphicsItem
{

public:
   // XColorGraphicsItem(XColorGraphicsItem* other, QGraphicsScene* scene);
    XColorGraphicsItem(XGraphicsScene* parentscene);

    explicit XColorGraphicsItem(XGraphicsScene* parentscene,QColor* fillColor/*,QGraphicsItem  *parent = 0*/);
    ~XColorGraphicsItem(){delete fillColor,delete painter,delete path;}
    void setColor(const QColor& fillColor);
    void addItem(XGraphicsItem* item);
    QRectF	boundingRect () const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    QPainterPath *getPath();
signals:

public slots:
private:
    double maxX,minX, maxY, minY;
    QList<QPainterPath*> bezierPaths;
    QColor* fillColor;
    QPainter* painter;
    QPainterPath* path;
    XGraphicsScene* scene;


};

#endif // XCOLORGRAPHICSITEM_H
