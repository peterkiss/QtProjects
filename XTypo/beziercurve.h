#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QList>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QGraphicsScene>
#include "beziercurve.h"
#include <QFrame>
#include <QGraphicsView>
#include <QMessageBox>
#include <QColor>
#include <QtGlobal>
#include <QListIterator>
#include <QtMath>
#include "xgraphicsitem.h"
#include "xgraphicsscene.h"


class XGraphicsScene;

class BezierCurve : public XGraphicsItem
{
public:
    BezierCurve(QPointF p1, QPointF p2,XGraphicsScene* parentScene,XLayer* layer,BezierCurve* prev = NULL);
    BezierCurve(QPointF p1,QPointF p2,QPointF p3,QPointF p4,XGraphicsScene* parentScene,XLayer* layer);
    BezierCurve(QPointF* offset, BezierCurve* originalCurve,XLayer* actlayer,XGraphicsScene* scene);
    ~BezierCurve(){if(previousCurve)previousCurve->nextCurve=NULL;if(nextCurve)nextCurve->previousCurve=NULL;}
    QRectF boundingRect() const;
    void moveEndPoint(QPointF newEndPoint);
    QPointF* getNearestControlPoint(QPointF pt, double& dist);
    QPointF* getNearestCurvePoint(QPointF pt, double& dist);
    void setNextCurve(BezierCurve* next);
    void setPreviousCurve(BezierCurve* prev);
    QPointF& getFirstEndPoint()        {return controlPoints[0];}
    QPointF& getSecondEndPoint()       {return controlPoints[3];}
    QPointF& getFirstHandlePoint()     {return controlPoints[1];}
    QPointF& getSecondHandlePoint()    {return controlPoints[2];}
    BezierCurve *devideCurve();//{QMessageBox::warning(NULL,"osztas","osztas",QMessageBox::Ok,QMessageBox::Discard);}
    QList<QLineF*> *getHorizontalConstraintLines(QPointF pos);
    QList<QLineF*> *getVerticalConstraintLines(QPointF pos);
    void SetPreviousCurve(BezierCurve* other){this->previousCurve=other;other->nextCurve=this;}
    void SetNextCurve(BezierCurve* other){this->nextCurve=other;other->previousCurve=this;}
    BezierCurve *getPreviousCurve(){return previousCurve;}
    BezierCurve *getNextCurve(){return nextCurve;}
    QPainterPath *getPath();
    QList<QPointF>* getControlPoints();
    void selectThisShape();
    void deSelectThisShape();
    void selectThisCurve();
    void deSelectThisCurve();
    bool isCurveSelected() const{return isSelected;}
    bool editing;
    bool isActive;
    bool isSelected;
    bool isFirstCurve;
public slots:
    void mousePressEvent(QPointF pt) ;
    void mouseMoveEvent(QPointF pt, bool moveOther);
    void mouseReleaseEvent(QMouseEvent * event);
    void moveHandlePoint(QPointF handlePos);

private:
    XGraphicsScene* containingScene;
    double actT;
    //    bool isStartcurve;
    QList<QPointF> controlPoints;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    static const double ellipseD;
    BezierCurve* nextCurve;
    BezierCurve* previousCurve;
    QPointF* actPoint;
    QPointF* nearestCurvePoint;
    qreal length;
    int actPointIsHandle;

    QList<QPointF> tempPoints;

};

#endif // BEZIERCURVE_H
