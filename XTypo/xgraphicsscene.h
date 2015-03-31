#ifndef XGRAPHICSSCENE_H
#define XGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "beziercurve.h"
#include "xcolorgraphicsitem.h"
#include "xgraphicsitem.h"
#include <QStack>

//class QStack;
class WorkFrame;
class BezierCurve;
class XColorGraphicsItem;
class XEllipseItem;
class XRectangleItem;
class XGraphicsPixmapItem;
class XGraphicsScene : public QGraphicsScene
{
public:

    XGraphicsScene(QObject *parent = NULL, char associatedChar = 'a',QList<XLayer*>* otherslayers = NULL);
    QList<XGraphicsItem*> selectedItems;
    QList<WorkFrame*> frames;
    QList<XLayer*> layers;
    QStack<QStringList> undoStack,redoStack;
    QList<XColorGraphicsItem*> colors;
    QList<BezierCurve*> StartCurves;
    QList<XGraphicsPixmapItem*> pixmaps;
    QList<XEllipseItem*> ellipses;
    QList<XRectangleItem*> rectangles;
    QList<QPointF*> selectedPoints;
    QList<BezierCurve*> selectedCurves;
    QList<XGraphicsPixmapItem*> selectedPixmaps;
    BezierCurve* actLoopFirstCurve;

    QList<QLineF *> *getConstraintLines(QPointF pos);
    void	addItem ( QGraphicsItem * item );
    void    addPixmap(QPixmap* pm);
    void    undo();
    void    redo();
    void    clear();
    bool    undoStackIsEmpty();
    bool    redoStackIsEmpty();
    void    saveState();
    XLayer* addLayer(QString name = "");
    void removeLayer(XLayer*layerToRemove);
    double scaleFactor;
    void setLineColor(const QColor* newColor);
    void setFillColor(const QColor* newColor);
    void addShowingWorkFrame(WorkFrame* f);
    void removeShowingWorkFrame(WorkFrame* f);
    QColor* getLineColor(){return lineColor;}
    QColor* getFillColor(){return fillColor;}
    XLayer* getActualLayer(){return actLayer;}
    QPointF* getMiddlePointOfSelectedShape();
    QPointF* getTopLeftPointOfSelectedShape();
    QList<XLayer*>* getLayers(){return &this->layers;}
    void paste(QPointF *offset, QList<BezierCurve*>* curves,QList<XGraphicsPixmapItem*>* pixmaps);
    void duplicate(QList<BezierCurve*>* curves,QList<XGraphicsPixmapItem*>* pixmaps);
    void deleteItems(QList<BezierCurve*>* curves,QList<XGraphicsPixmapItem*>* pixmaps);
    void clearShapeSelection();
    bool detailedMode;
    void addPixmpItem(QString fileName);
    void setActualLayer(XLayer* layer){actLayer = layer;}
    char getAssociatedChar() {return associatedChar;}
    QString getAssociatedCharAsString();
    void setAssociatedChar(char c) { associatedChar = c;}
    QStringList toString();
    void fromString(QStringList sceneData);
    void createBaseLines();
private:
    QColor* lineColor,*fillColor;
    XLayer* actLayer;
    char associatedChar;
};

#endif // XGRAPHICSSCENE_H
