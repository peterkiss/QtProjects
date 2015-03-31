#ifndef XGRAPHICSITEM_H
#define XGRAPHICSITEM_H

#include <QGraphicsItem>
#include "xlayer.h"
#include <QMouseEvent>

enum graphicsItemType{BezierCurveType,LineType,ColorType,PixmapType,EllipseType};


class XGraphicsScene;
//class XLayer;
class XGraphicsItem : public QGraphicsItem
{

public:
    explicit XGraphicsItem(QGraphicsItem *parent = 0);
    bool isBezierCurve() {return type == BezierCurveType;}
    bool isConstraintLine() {return type == LineType;}
    XLayer* getLayer() {return layer;}
protected:
    graphicsItemType type;
    XLayer* layer;

signals:

public slots:

};

#endif // XGRAPHICSITEM_H
