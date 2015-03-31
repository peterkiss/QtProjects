#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "drawarea.h"
#include <QGridLayout>
#include <QGraphicsScene>

#include "blendersplitter.h"
#include <QMenuBar>


/**
 * @brief MainWindow
 * alkalmazas foablaka, minden WorkFrame kozos valtozok
 *
 */


namespace Ui {
class MainWindow;
}
enum  editmodes : char{draw=0, edit=1};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exportAct;

    QAction* setModifyModeAct;
    QAction* setDrawModeAct;
    QAction* setConstrainToPointModeAct;
    QAction* setConstrainToLineModeAct;

    QAction* setSelectionModeAct;
    QAction* copySelectedItemsAct;
    QAction* setSelectedItemsPasteModeAct;
    QAction* setSelectedItemsDuplicateModeAct;
    QAction* deleteSelectedItemsAct;

    QAction* setRotateModeAct;
    QAction* setTranslateModeAct;
    QAction* setScaleModeAct;

    QAction* setRectangleDrawModeAct;
    QAction* setEllipseDrawModeAct;
    QAction* setCurveDrawModeAct;


    QFrame* designFrame,* testFrame;


    void createActions();
    void createMenus();
    void  fillCharMap();
    void ClearMap();
    static editmodes editMode;      //draw || modify
    // masolando elemekre : clipBoardOrigo,curveClipBoard,pixmapClipBoard
    static QPointF* clipBoardOrigo;
    static QList<BezierCurve*>* curveClipBoard;
    static QList<XGraphicsPixmapItem*>* pixmapClipBoard;
    static XGraphicsScene* lastModifiedScene;//utolso kijeloles scene
    static void clearClipBoard(){curveClipBoard = NULL; pixmapClipBoard = NULL;clipBoardOrigo=NULL;}
    static bool ConstraintToPoint,ConstraintToConstraintLine;
    static QList<WorkFrame*> frames;
public slots:
    void newSession();
    void saveSession();
    void openSession();
    void exportToOTF();

    void setModifyModeAllFrames();
    void setDrawModeAllFrames();
    void setConstrainToPointModeAllFrames();
    void setConstrainToLineModeAllFrames();
    void setScaleModeAllFrames();
    void copySelectedItems();
    void setSelectedItemsPasteModeAllFrames();
    void deleteSelectedItemsAllFrames();
    void setRotateModeAllFrames();
    void setEllipseDrawModeAllFrames();
    void setRectangleDrawModeAllFrames();
    void setCurveDrawModeAllFrames();

    void setMode(int index);
    void setConstraintToPoint(bool checkstate);
    void setConstraintToConstraintLine(bool checkstate);

private:

    Ui::MainWindow *ui;
    QGraphicsScene* d;
    XGraphicsScene *scene;
    BlenderSplitter *h1Splitter;
    QFrame* constraintFrame, *modeFrame;
    QToolButton* PointConstraintToolButton,*LineConstraintButton;
    QComboBox* modeCombobox;


};

#endif // MAINWINDOW_H
