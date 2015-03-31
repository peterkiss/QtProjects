#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QPoint>
#include <QList>
#include <QMouseEvent>
#include <QGraphicsScene>
#include "beziercurve.h"
#include <QFrame>
#include <QGraphicsView>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QComboBox>
#include <QHBoxLayout>
#include "xgraphicsscene.h"
#include <QToolButton>
#include <qlistview.h>
#include <QListWidgetItem>
#include <QStandardItem>
#include "xellipseitem.h"
#include "xrectangleitem.h"
#include "xeditor.h"

class QStandardItemModel;
class WorkFrame;
class XColorGraphicsItem;


enum MyRoles {
    LayerRole = Qt::UserRole + 1
};
enum contstrainedMoveDirection {
    none = 0,vertical = 1,horizontal =2
};



enum drawModes : char{curve=0, ellipse=1, mirror=2, offset=3, color=4, rectangle=5, polyline=6, rotateMode=7,moveMode=8,scaleMode=9, copyMode =10,selectMode = 11, waitForPastePos = 12};
class DrawGraphicsView :  public QGraphicsView
{
    Q_OBJECT


signals:
public slots:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent *event );
    void keyPressEvent(QKeyEvent *event);
    void leaveEvent(QEvent* e);

public:
    void deselect();
    void reset()
    {
        mirrorAxis = NULL;
        actCurve=NULL;
        actRect=NULL;
        actEllipse=NULL;
        drawMode = curve;
        lastPoint=NULL;
        newLoop=false;
        firstHandlePoint=NULL;
        origVectorsImage = NULL;
        newVectorsImage = NULL;
        nearestLineItem = NULL;
        constrainedPositionEllipse = NULL;
        constrainedPositionPoint = NULL;
    }

    DrawGraphicsView(WorkFrame *v, XGraphicsScene* scene) : QGraphicsView(), frame(v)
    {
        SetXScene(scene);
        reset();

    }
    QPointF* findNearestCurveByEndPoint(QPointF pt,QTransform transform);
    BezierCurve* findNearestCurveByControlPoint(QPointF pt,QTransform transform);
    BezierCurve* findNearestCurveByCurvePoint(QPointF pt,QTransform transform);
    XGraphicsScene* getXScene(){return xScene;}
    void SetXScene(XGraphicsScene* sc) {xScene = sc;setScene(xScene);}
    void resetWork(){actCurve = NULL;firstHandlePoint=NULL;actRect=NULL;
                     actEllipse=NULL; mirrorAxis = NULL; }
    void SetDrawMode(drawModes newMode){resetWork(); drawMode=newMode;}
    void wheelEvent ( QWheelEvent * event );
    void setSelectedPixmapsPos();
    void WaitingForPos();
    void newSelectedItems();
    double getConstrLineDist(QPointF* point, QLineF *line);
    bool isLineHorizontal(QLineF *line);
protected:
    void DrawConstraintLines(QPointF pos, contstrainedMoveDirection Constraining);
    void ClearConstraintLines();
private:
    contstrainedMoveDirection moveConstrainly(QPointF& newpt, QPointF startPt);
    bool ConstraintToPoint,ConstraintToConstraintLine;
    drawModes drawMode;
    WorkFrame* frame;
    XGraphicsScene* xScene;
    BezierCurve* actCurve;
    XEllipseItem* actEllipse;
    XRectangleItem* actRect;
    QPointF* lastPoint;
    QPointF* firstHandlePoint;
    QLineF* mirrorAxis;
    QGraphicsLineItem* mirrorAxisLineItem;
    QList<BezierCurve> curves;
    QList<QPoint> points;
    QList<QGraphicsLineItem*> ConstrLineItems;
    QGraphicsEllipseItem* constrainedPositionEllipse;
    QPointF* constrainedPositionPoint;
    QGraphicsLineItem* nearestLineItem;
    QList<QPointF*> tempPointList;
    QPointF* originalMiddlePoint;
    QVector2D* origVector,*newVector;
    QGraphicsLineItem* origVectorsImage,*newVectorsImage;
    QPixmap pixmap;
    QPen pen;
    QBrush brush;

    bool newLoop;


};



class WorkFrame : public QFrame
{
    Q_OBJECT
public:
    explicit WorkFrame(const QString &name, XGraphicsScene *scene, QWidget *parent = 0);
    WorkFrame(const WorkFrame& other);
    QGraphicsView *getDrawGraphicsView() const;
    QFrame* drawFrame;
    XEditor* writeFrame;
    QComboBox *FrameModeCombo;
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *pe) override;
    void mousePressEvent(QMouseEvent* me);
    void mouseMoveEvent(QMouseEvent* me);
    void mouseReleaseEvent(QMouseEvent *);
    void leaveEvent(QEvent* e);
    void SetUpWorkFrame(XGraphicsScene *scene);
    void setHandlePos();
    void updateLayersFromScene(bool synchronizing = false);
    void updateColorsFromScene();
    void updateFromUndoStack();
    void setScene(XGraphicsScene* sc);



    XGraphicsScene* getScene() const {return drawGraphicsView->getXScene();}
    bool reOrg;
    bool  moveSplitterAfterReorg;
public    slots:

    void setMirrorMode();
    void setEllipseMode();
    void setRectangleMode();
    void setCurveMode();
    void setOffsetMode();
    void setColorMode();
    void setRotateMode();
    void setMoveMode();
    void setScaleMode();
    void setSelectMode();
    void copySelectedItems();
    void pasteSelectedItems();
    void newSelectedItems();
    void deleteSelectedItems();
    void toogleToolFrame();
    void toogleLayerFrame();
    void toogleColorFrame();
    void toogleShapeFrame();
    void undo();
    void redo();
    void FrameModeChange();

    void chooseLineColor();
    void chooseFillColor();
    void addPixmap();
    void addLayer();
    void deleteLayer();
    void LayersItemClicked(QModelIndex modelindex);
    void LayersItemChecked(QStandardItem*item);
    void SetCharForGraphicsScene(int i);
    void SetGraphicsScene(XGraphicsScene* scene);



private:
    bool visibilitychanged;
    void setLineColor(const QColor* color );
    void setFillColor(const QColor* color );

    QScrollArea* scrollArea ;
    QList<QToolButton*> toolButtons;
    QStandardItemModel* layersItemModel;
    QBrush* activeLayerBackGroundBrush;
    QBrush* inActiveLayerBackGroundBrush;
    DrawGraphicsView *drawGraphicsView;
    QListView* layerListView ;
    QFrame* toolPanelFrame,*layerFrame,*colorFrame,*statusFrame,*toolsFrame,*shapesFrame,*undoFrame;
    QPushButton* layersToogleButton,*shapeToogleButton,*toolsToogleButton,*colorToogleButton,*newLayerButton,*deleteLayerButton,* addPixmapButton;
    QPixmap* lineColorPixmap,*fillColorPixmap;
    QIcon* fillColorIcon,*lineColorIcon;
    QColor* fillColor,*lineColor;
    QComboBox/** modeCombobox,* constrainCombobox,*/*characterChoseCombo;
    QToolButton *deleteButton,* pasteButton,*copyButton,*newShapeButton,*selectModeButton,*curveModeButton,*ellipseModeButton,*mirrorModeButton,*offsetModeButton,
    *colorModeButton,*rectangleModeButton,*rotateModeButton,*moveModeButton,*scaleModeButton, *pixmapToolButton, *undoButton,*redoButton;
    QList<BezierCurve> curves;
    QPushButton *lineColorButton,*fillColorButton  ;
    QPointF handlePos;
    void pasteFromClipBoard();
};



#endif // DRAWAREA_H
