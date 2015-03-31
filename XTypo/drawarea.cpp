#include "drawarea.h"



#include <QtWidgets>
#include <qmath.h>
#include <QSizePolicy>
#include <QPushButton>
#include <QTransform>
#include "blendersplitter.h"
#include "XGraphicsLineItem.h"
#include "xgrphicspixmapitem.h"
#include "xellipseitem.h"
#include "xrectangleitem.h"
#include "xtransform.h"
#include <QVBoxLayout>
#include "mainwindow.h"

#define newDrawMode

const int CatchPointDistance = 10;
const int CatchCurveDistance = 5;
extern QMap<char,XGraphicsScene*> charMap;

//layerlist tartalmazhasson ilyen objetumot
Q_DECLARE_METATYPE(XLayer*)

/**
 * @brief WorkFrame::SetUpWorkFrame
 * @param scene tartalmazott scene
 * kontrollok elhelyezese inicializalasa
 *
 */
void WorkFrame::SetUpWorkFrame(XGraphicsScene* scene)
{
    QPixmap* rectanglePm = new QPixmap(":/typo/icon/rectangle.png");
    QIcon* rectangleIcon = new QIcon(*rectanglePm);
    QPixmap* ellipsePm = new QPixmap(":/typo/icon/ellipse.png");
    QIcon* ellipseIcon = new QIcon(*ellipsePm);
    QPixmap* CurvePm = new QPixmap(":/typo/curve");
    QIcon*  CurveIcon = new QIcon(*CurvePm);
    QPixmap* mirrorPm = new QPixmap(":/typo/icon/mirror.png");
    QIcon* mirrorIcon = new QIcon(*mirrorPm);
    QPixmap* copyPm = new QPixmap(":/typo/icon/copy.png");
    QIcon* copyIcon = new QIcon(*copyPm);
    QPixmap* pastePm = new QPixmap(":/typo/icon/paste.png");
    QIcon* pasteIcon = new QIcon(*pastePm);
    QPixmap* duplicatePm = new QPixmap(":/typo/icon/duplicate.png");
    QIcon* duplicateIcon = new QIcon(*duplicatePm);
    QPixmap* deletePm = new QPixmap(":/typo/icon/delete.png");
    QIcon* deleteIcon = new QIcon(*deletePm);
    QPixmap* rotatePm = new QPixmap(":/typo/icon/rotate.png");
    QIcon* rotateIcon = new QIcon(*rotatePm);
    QPixmap* translatePm = new QPixmap(":/typo/move");
    QIcon* translateIcon = new QIcon(*translatePm);
    QPixmap* redoPm = new QPixmap(":/typo/icon/redo.png");
    QIcon* redoIcon = new QIcon(*redoPm);
    QPixmap* undoPm = new QPixmap(":/typo/icon/undo.png");
    QIcon* undoIcon = new QIcon(*undoPm);
    QPixmap* selectPm = new QPixmap(":/typo/icon/select.png");
    QIcon* selectIcon = new QIcon(*selectPm);
    QPixmap* scalePm = new QPixmap(":/typo/icon/scale.png");
    QIcon* scaleIcon = new QIcon(*scalePm);
    QPixmap* pixmapPm = new QPixmap(":/typo/image");
    QIcon* pixmapIcon = new QIcon(*pixmapPm);
    lineColorIcon = NULL;
    fillColorIcon = NULL;
    moveSplitterAfterReorg=false;
    setFrameStyle(Sunken | StyledPanel);
    drawGraphicsView = new DrawGraphicsView(this,scene) ;
    getScene()->addShowingWorkFrame(this);
    drawGraphicsView->setRenderHint(QPainter::Antialiasing, true);
    drawGraphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing );
    drawGraphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);


    /////////////////////////////////////////////////
    QHBoxLayout *labelLayout = new QHBoxLayout;


    //QList<char> charlist;
    FrameModeCombo = new QComboBox;
    FrameModeCombo->addItem("Design");
    FrameModeCombo->addItem("Try");
    characterChoseCombo = new QComboBox;
    QMapIterator<char,XGraphicsScene*> cmi(charMap);
    while(cmi.hasNext()){
        QByteArray array;
        array.append(cmi.next().key());
        characterChoseCombo->addItem(QString::fromLocal8Bit(array));
    }
    characterChoseCombo->setCurrentText((QString)getScene()->getAssociatedChar());
    FrameModeCombo->setCurrentIndex(0);

    statusFrame = new QFrame;
    QHBoxLayout* constrLayout = new QHBoxLayout;
    constrLayout->addWidget(characterChoseCombo);
    constrLayout->addWidget(FrameModeCombo);
    statusFrame->setLayout(constrLayout);
    connect(characterChoseCombo,SIGNAL(currentIndexChanged (int)),this,SLOT(SetCharForGraphicsScene(int)));
    connect(FrameModeCombo,SIGNAL(currentIndexChanged (int)),this,SLOT(FrameModeChange()));
    /////////////////////////////////////////////////

    /////////////////////////////////////////////////
    toolsToogleButton = new QPushButton("Tools");
    shapeToogleButton = new QPushButton("Shapes");
    QGridLayout *ToolBoxLayout = new QGridLayout;
    QGridLayout *ShapeBoxLayout = new QGridLayout;
    ToolBoxLayout->setHorizontalSpacing(10);
    ToolBoxLayout->setVerticalSpacing(10);
    ShapeBoxLayout->setHorizontalSpacing(10);
    ShapeBoxLayout->setVerticalSpacing(10);
    toolsFrame = new QFrame;
    shapesFrame = new QFrame;
    curveModeButton = new QToolButton;
    curveModeButton->setIcon(*CurveIcon);
    curveModeButton->setToolTip("Curve");
    toolButtons.append(curveModeButton);
    rectangleModeButton = new QToolButton;
    rectangleModeButton->setIcon(*rectangleIcon);
    rectangleModeButton->setToolTip("Rect");
    toolButtons.append(rectangleModeButton);
    ellipseModeButton = new QToolButton();
    ellipseModeButton->setIcon(*ellipseIcon);
    ellipseModeButton->setToolTip("Ellipse");
    toolButtons.append(ellipseModeButton);
    mirrorModeButton = new QToolButton;
    mirrorModeButton->setIcon(*mirrorIcon);
    mirrorModeButton->setToolTip("Mirror");
    toolButtons.append(mirrorModeButton);
    colorModeButton  = new QToolButton;
    colorModeButton->setToolTip("Color");
    offsetModeButton  = new QToolButton;
    offsetModeButton->setToolTip("Offset");
    rotateModeButton = new QToolButton;
    rotateModeButton->setIcon(*rotateIcon);
    rotateModeButton->setToolTip("Rotate");
    toolButtons.append(rotateModeButton);
    moveModeButton = new QToolButton;
    moveModeButton->setIcon(*translateIcon);
    moveModeButton->setToolTip("Move");
    toolButtons.append(moveModeButton);
    scaleModeButton = new QToolButton;
    scaleModeButton->setIcon(*scaleIcon);
    scaleModeButton->setToolTip("Scale");
    toolButtons.append(scaleModeButton);
    copyButton  = new QToolButton;
    copyButton->setIcon(*copyIcon);
    copyButton->setToolTip("Copy");
    toolButtons.append(copyButton);
    pasteButton = new QToolButton;
    pasteButton->setToolTip("Paste");
    pasteButton->setIcon(*pasteIcon);
    toolButtons.append(pasteButton);
    newShapeButton = new QToolButton;
    newShapeButton->setIcon(*duplicateIcon);
    newShapeButton->setToolTip("duplicate");
    toolButtons.append(newShapeButton);
    selectModeButton = new QToolButton;
    selectModeButton->setIcon(*selectIcon);
    selectModeButton->setToolTip("Select");
    toolButtons.append(selectModeButton);
    pixmapToolButton = new QToolButton;
    pixmapToolButton->setIcon(*pixmapIcon);
    pixmapToolButton->setToolTip("Pixmap");
    toolButtons.append(pixmapToolButton);
    deleteButton = new QToolButton;
    deleteButton->setIcon(*deleteIcon);
    deleteButton->setToolTip("Delete");
    toolButtons.append(deleteButton);


    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext()){
        QToolButton* act =bi.next();
        act->setCheckable(true);
        act->setFixedSize(40,40);
    }
    curveModeButton->setChecked(true);

    ToolBoxLayout->addWidget(selectModeButton,0,0);
    ToolBoxLayout->addWidget(newShapeButton,0,1);
    ToolBoxLayout->addWidget(copyButton,0,2);
    ToolBoxLayout->addWidget(pasteButton,0,3);
    ToolBoxLayout->addWidget(mirrorModeButton,1,0);
    ToolBoxLayout->addWidget(rotateModeButton,1,1);
    ToolBoxLayout->addWidget( moveModeButton,1,2);
    ToolBoxLayout->addWidget(scaleModeButton,1,3);
    ToolBoxLayout->addWidget(deleteButton,2,1);



    ShapeBoxLayout->addWidget(curveModeButton,0,0);
    ShapeBoxLayout->addWidget(ellipseModeButton,0,1);
    ShapeBoxLayout->addWidget(rectangleModeButton,0,2);
    ShapeBoxLayout->addWidget(pixmapToolButton,0,3);


    connect(curveModeButton,SIGNAL(clicked(bool)),this,SLOT(setCurveMode())) ;
    connect(ellipseModeButton,SIGNAL(clicked(bool)),this,SLOT(setEllipseMode())) ;
    connect(mirrorModeButton,SIGNAL(clicked(bool)),this,SLOT(setMirrorMode())) ;
    connect(offsetModeButton,SIGNAL(clicked(bool)),this,SLOT(setOffsetMode())) ;
    connect(colorModeButton,SIGNAL(clicked(bool)),this,SLOT(setColorMode())) ;
    connect(rectangleModeButton,SIGNAL(clicked(bool)),this,SLOT(setRectangleMode())) ;
    connect(rotateModeButton,SIGNAL(clicked(bool)),this,SLOT(setRotateMode())) ;
    connect(moveModeButton,SIGNAL(clicked(bool)),this,SLOT(setMoveMode())) ;
    connect(scaleModeButton,SIGNAL(clicked(bool)),this,SLOT(setScaleMode())) ;
    connect(selectModeButton,SIGNAL(clicked(bool)),this,SLOT(setSelectMode()));
    connect(copyButton,SIGNAL(clicked(bool)),this,SLOT(copySelectedItems()));
    connect(pasteButton,SIGNAL(clicked(bool)),this,SLOT(pasteSelectedItems()));
    connect(newShapeButton,SIGNAL(clicked(bool)),this,SLOT(newSelectedItems()));
    connect(deleteButton,SIGNAL(clicked(bool)),this,SLOT(deleteSelectedItems()));
    connect(pixmapToolButton,SIGNAL(clicked()),this,SLOT(addPixmap()));
    connect(toolsToogleButton,SIGNAL(clicked()),this,SLOT(toogleToolFrame()));
    connect(shapeToogleButton,SIGNAL(clicked()),this,SLOT(toogleShapeFrame()));

    toolsFrame->setLayout(ToolBoxLayout);
    shapesFrame->setLayout(ShapeBoxLayout);
    /////////////////////////////////////////////////
    layersToogleButton = new QPushButton("Layers");
    layerFrame = new QFrame;
    QVBoxLayout* layerLayout = new QVBoxLayout;
    activeLayerBackGroundBrush = new QBrush(Qt::red);
    inActiveLayerBackGroundBrush = new QBrush(Qt::white);
    layerListView = new QListView();
    layersItemModel=new  QStandardItemModel;
    updateLayersFromScene();

    deleteLayerButton = new QPushButton("delete");
    newLayerButton = new QPushButton("new");
    addPixmapButton = new QPushButton("pixmap");
    QGridLayout* layerButtonsLAyout= new QGridLayout;
    layerButtonsLAyout->addWidget(newLayerButton,0,0);
    layerButtonsLAyout->addWidget(deleteLayerButton,0,1);
    layerButtonsLAyout->addWidget(addPixmapButton,1,1);

    layerLayout->addWidget(layerListView);
    layerLayout->addLayout(layerButtonsLAyout);

    connect(layersToogleButton,SIGNAL(clicked()),this,SLOT(toogleLayerFrame()));
    connect(addPixmapButton,SIGNAL(clicked()),this,SLOT(addPixmap()));
    connect(layerListView,SIGNAL(clicked(QModelIndex)),this,SLOT(LayersItemClicked(QModelIndex)));
    connect(layersItemModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(LayersItemChecked(QStandardItem*)));
    connect(newLayerButton,SIGNAL(clicked()),this,SLOT(addLayer()));
    connect(deleteLayerButton,SIGNAL(clicked()),this,SLOT(deleteLayer()));
    visibilitychanged = false;

    layerFrame->setLayout(layerLayout);

    /////////////////////////////////////////////////
    colorToogleButton = new QPushButton("Colors");
    colorFrame = new QFrame;
    QGroupBox* fgGroup = new QGroupBox("Line");
    QGroupBox* bgGroup = new QGroupBox("Paint");

    fillColorPixmap = new QPixmap(50,16);
    lineColorPixmap = new QPixmap(50,16);

    fillColorButton = new QPushButton;
    lineColorButton = new QPushButton;

    fillColorButton->setIconSize( QSize(50,16));
    lineColorButton->setIconSize( QSize(50,16));

    updateColorsFromScene();

    QHBoxLayout* lineLayout = new QHBoxLayout;
    QHBoxLayout* fillLayout = new QHBoxLayout;
    lineLayout->addWidget(lineColorButton);
    fillLayout->addWidget(fillColorButton);
    fgGroup->setLayout(lineLayout);
    bgGroup->setLayout(fillLayout);
    QHBoxLayout * colorChooserLayout = new QHBoxLayout;
    colorChooserLayout->addWidget(fgGroup);
    colorChooserLayout->addWidget(bgGroup);

    connect(lineColorButton,SIGNAL(clicked()),this,SLOT(chooseLineColor()));
    connect(fillColorButton,SIGNAL(clicked()),this,SLOT(chooseFillColor()));
    connect(colorToogleButton,SIGNAL(clicked()),this,SLOT(toogleColorFrame()));

    colorFrame->setLayout(colorChooserLayout);

    /////////////////////////////////////////////////
    QVBoxLayout* toollayout = new QVBoxLayout;


    toollayout->addWidget(toolsToogleButton);
    toollayout->addWidget(toolsFrame);
    toollayout->addWidget(shapeToogleButton);
    toollayout->addWidget(shapesFrame);
    toollayout->addWidget(layersToogleButton);
    toollayout->addWidget(layerFrame);
    toollayout->addWidget(colorToogleButton);
    toollayout->addWidget(colorFrame);
    toollayout->addStretch();

    toolPanelFrame = new QFrame;
    scrollArea = new QScrollArea;

    toolPanelFrame->setLayout(toollayout);
    toolPanelFrame->setFixedWidth(200);
    scrollArea->setWidget(toolPanelFrame);
    scrollArea->setFixedWidth(toolPanelFrame->width() + 20);

    /////////////////////////////////////////////////
    undoFrame = new QFrame;
    QHBoxLayout *undoLayout = new QHBoxLayout ;
    undoButton = new QToolButton;
    undoButton->setFixedSize(40,40);
    undoButton->setIcon(*undoIcon);
    undoButton->setToolTip("Undo");
    redoButton = new QToolButton;
    redoButton->setFixedSize(40,40);
    redoButton->setIcon(*redoIcon);
    redoButton->setToolTip("Redo");
    undoLayout->addWidget(undoButton);
    undoLayout->addWidget(redoButton);
    undoLayout->addStretch();

    connect(undoButton,SIGNAL(clicked()),this,SLOT(undo()));
    connect(redoButton,SIGNAL(clicked()),this,SLOT(redo()));

    updateFromUndoStack();
    undoFrame->setFixedWidth(200);
    undoFrame->setLayout(undoLayout);


    /////////////////////////////////////////////////

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(scrollArea,0,0);
    layout->addWidget(getDrawGraphicsView(),0,1);
    layout->addWidget(undoFrame,2,0);
    layout->addWidget(statusFrame,2,1);

    drawFrame = new QFrame;
    drawFrame->setLayout(layout);

    writeFrame = new XEditor(this);
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(drawFrame);
    mainLayout->addWidget(writeFrame);
    writeFrame->hide();
    setLayout(mainLayout);

}

/**
 * WorkFrame::updateLayersFromScene
 * @param synchronizing nem hasznalt
 * ha valtozik a workframehez(WorkFrame DrawGraphicsView memberehez) tarsiottmutatott scene
 * layerLista update-elese scenebol
 */
void WorkFrame::updateLayersFromScene(bool synchronizing)
{
    if(!layersItemModel)
        layersItemModel=new  QStandardItemModel;
    else
        layersItemModel->clear();
    QList<XLayer*> * layerList = this->getScene()->getLayers();
    QListIterator<XLayer*> layerIt(*layerList);
    while(layerIt.hasNext()){
        XLayer* act = layerIt.next();
        QStandardItem* item = new QStandardItem(act->getName());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(QVariant::fromValue(act), LayerRole);
        // item->setData(Qt::Checked, Qt::CheckStateRole);
        if(act->isActive() && !synchronizing)
            item->setData(Qt::Checked, Qt::CheckStateRole);
        else
            item->setData(Qt::Unchecked, Qt::CheckStateRole);
        if(act == this->getScene()->getActualLayer())
            item->setBackground(*activeLayerBackGroundBrush);
        layersItemModel->appendRow(item);

    }

    layerListView = new QListView();
    layerListView->setModel(layersItemModel);
}
/**
 * WorkFrame::updateColorsFromScene
 * ha valtoznak a workframehez(WorkFrame DrawGraphicsView memberehez) tarsiottmutatott scene
 * szinei update-elese scenebol
 */
void WorkFrame::updateColorsFromScene()
{

    lineColor = getScene()->getLineColor();
    lineColorPixmap->fill(*lineColor);
    if(lineColorIcon)
        delete lineColorIcon;
    lineColorIcon = new QIcon(*lineColorPixmap);
    lineColorButton->setIcon(*lineColorIcon);

    fillColor = getScene()->getFillColor();
    fillColorPixmap->fill(*fillColor);
    if(fillColorIcon)
        delete fillColorIcon;
    fillColorIcon = new QIcon(*fillColorPixmap);
    fillColorButton->setIcon(*fillColorIcon);
}
/**
 * WorkFrame::updateFromUndoStack
 * gombok elerhetoseget frissiti stackek allapota alapjan
 */
void WorkFrame::updateFromUndoStack()
{
    this->undoButton->setEnabled(!getScene()->undoStackIsEmpty());
    this->redoButton->setEnabled(!getScene()->redoStackIsEmpty());
}
/**
 * WorkFrame::updateFromUndoStack
 * @param sc XGraphicsScene-re mutato
 * megjelenitett scene lecserelese
 */
void WorkFrame::setScene(XGraphicsScene *sc)
{
    this->drawGraphicsView->SetXScene(sc);
    this->drawGraphicsView->reset();
    QByteArray byte;
    byte.append(sc->getAssociatedChar());
    this->characterChoseCombo->setCurrentText(QString::fromLocal8Bit(byte) );

}
/**
 * WorkFrame::set*Mode
 * szerkesztesi modok valtoztatasa eszkozvaltaskor
 */
void WorkFrame::setMirrorMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    mirrorModeButton->setChecked(true);
    this->drawGraphicsView->SetDrawMode(mirror);
}
/**
 * WorkFrame::set*Mode
 * szerkesztesi modok valtoztatasa eszkozvaltaskor
 */
void WorkFrame::setEllipseMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    ellipseModeButton->setChecked(true);
    this->drawGraphicsView->SetDrawMode(ellipse);
}
/**
 * WorkFrame::set*Mode
 * szerkesztesi modok valtoztatasa eszkozvaltaskor
 */
void WorkFrame::setRectangleMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    rectangleModeButton->setChecked(true);
    this->drawGraphicsView->SetDrawMode(rectangle);
}
/**
 * WorkFrame::set*Mode
 * szerkesztesi modok valtoztatasa eszkozvaltaskor
 */
void WorkFrame::setRotateMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    rotateModeButton->setChecked(true);
    this->drawGraphicsView->SetDrawMode(rotateMode);
}
/**
 * WorkFrame::set*Mode
 * szerkesztesi modok valtoztatasa eszkozvaltaskor
 */
void WorkFrame::setMoveMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    moveModeButton->setChecked(true);
    this->drawGraphicsView->SetDrawMode(moveMode);
}
/**
 * WorkFrame::set*Mode
 * szerkesztesi modok valtoztatasa eszkozvaltaskor
 */
void WorkFrame::setScaleMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    scaleModeButton->setChecked(true);
    this->drawGraphicsView->SetDrawMode(scaleMode);
}
/**
 * WorkFrame::set*Mode
 * szerkesztesi modok valtoztatasa eszkozvaltaskor
 */
void WorkFrame::setSelectMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    selectModeButton->setChecked(true);
    this->drawGraphicsView->SetDrawMode(selectMode);
}
/**
 * WorkFrame::copySelectedItems
 * MainWindow "clipboard" muatatoit raalitja a megjelenitett scene kijelolesi listaira
 */
void WorkFrame::copySelectedItems()
{
    MainWindow::clearClipBoard();
    MainWindow::clipBoardOrigo = getScene()->getTopLeftPointOfSelectedShape();
    MainWindow::curveClipBoard  = &getScene()->selectedCurves;
    MainWindow::pixmapClipBoard = &getScene()->selectedPixmaps;
    MainWindow::lastModifiedScene = this->getScene();
    copyButton->setChecked(false);
}
/**
 * WorkFrame::pasteSelectedItems
 * MainWindow "clipboard" muatatoit ellenorizve beillesztes helyere valo varakozas modba all
 */
void WorkFrame::pasteSelectedItems()
{
    if(MainWindow::clipBoardOrigo && MainWindow::curveClipBoard && MainWindow::pixmapClipBoard)
        this->drawGraphicsView->WaitingForPos();
    else return;
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    pasteButton->setChecked(true);
}
/**
 * WorkFrame::newSelectedItems
 * kivalasztott elemek duplikalasa copySelectedItems celja hogy sima paste-tal menjen->nem kell uj fuggveny
 */
void WorkFrame::newSelectedItems()
{
    copySelectedItems();
    this->drawGraphicsView->newSelectedItems();
    newShapeButton->setChecked(false);

}
/**
 * WorkFrame::deleteSelectedItems
 * kivalasztott elemek torlese a scene-bol
 */
void WorkFrame::deleteSelectedItems()
{
    this->drawGraphicsView->resetWork();

    copySelectedItems();
    getScene()->deleteItems(MainWindow::curveClipBoard,MainWindow::pixmapClipBoard);
    deleteButton->setChecked(false);
}
/**
 * WorkFrame::set*Mode
 * szerkesztesi modok valtoztatasa eszkozvaltaskor
 */
void WorkFrame::setCurveMode()
{

    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    curveModeButton->setChecked(true);
    this->drawGraphicsView->SetDrawMode(curve);
}
/**
 * WorkFrame::setOffsetMode
 * nem hasznalt funkcio
 */
void WorkFrame::setOffsetMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    offsetModeButton->setChecked(true);
}
/**
 * WorkFrame::setColorMode
 * nem hasznalt funkcio
 */
void WorkFrame::setColorMode()
{
    QListIterator<QToolButton*> bi(toolButtons);
    while(bi.hasNext())
        bi.next()->setChecked(false);
    colorModeButton->setChecked(true);
}
/**
 * WorkFrame::toogle*Frame
 * panelek lecsukasa
 */
void WorkFrame::toogleToolFrame()
{
    if(toolsFrame->isHidden())
        toolsFrame->show();
    else
        toolsFrame->hide();
}
void WorkFrame::toogleShapeFrame()
{
    if(shapesFrame->isHidden())
        shapesFrame->show();
    else
        shapesFrame->hide();
}

void WorkFrame::undo()
{
    this->getScene()->undo();
}

void WorkFrame::redo()
{
    this->getScene()->redo();
}
/**
 * WorkFrame::FrameModeChange
 * munkaablak es tesztablak mkozotti valtas
 */
void WorkFrame::FrameModeChange()
{
    if(this->FrameModeCombo->currentIndex() == 0){
        this->writeFrame->hide();
        this->drawFrame->show();
    }
    else{
        this->writeFrame->FrameModeCombo->setCurrentIndex(1);
        this->writeFrame->show();
        this->drawFrame->hide();
    }
}
/**
 * WorkFrame::toogle*Frame
 * panelek lecsukasa
 */
void WorkFrame::toogleLayerFrame()
{
    if(layerFrame->isHidden())
        layerFrame->show();
    else
        layerFrame->hide();
}
/**
 * WorkFrame::toogle*Frame
 * panelek lecsukasa
 */
void WorkFrame::toogleColorFrame()
{
    if(colorFrame->isHidden())
        colorFrame->show();
    else
        colorFrame->hide();
}
/**
 * WorkFrame::chooseLineColor
 * kontur-szin valasztasa majd atadasa a scene-nek. Ez utani tartalmazo scenek frissulnek
 */
void WorkFrame::chooseLineColor()
{
    QColorDialog dlg;
    dlg.exec();
    QColor* c = new QColor(dlg.selectedColor());
    setLineColor(c);
    delete c;

}
/**
 * WorkFrame::chooseFillColor
 * kitolto szin valasztasa majd atadasa a scene-nek. Ez utani tartalmazo scenek frissulnek
 */
void WorkFrame::chooseFillColor()
{
    QColorDialog dlg;
    dlg.exec();
    QColor* c = new QColor(dlg.selectedColor());
    setFillColor(c);
    delete c;
}

void WorkFrame::addPixmap()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

    getScene()->addPixmpItem(fileName);

}

void WorkFrame::addLayer()
{

    XLayer* l = getScene()->addLayer()  ;

}

void WorkFrame::deleteLayer()
{
    XLayer* layer = NULL;
    layer = (layerListView->currentIndex().data(LayerRole).value<XLayer*>());
    if(layer)
        layer->setActive(false);
    getScene()->removeLayer(layer);

}
/**
 * WorkFrame::LayersItemClicked
 * layer aktivva tetele- ha csak lathatova tettek a checkboxszal akkor is befut ide, ezert az if
 */
void WorkFrame::LayersItemClicked(QModelIndex modelindex)
{
    if(visibilitychanged){
        visibilitychanged=false;
        return;
    }

    for(int i=0;i<layersItemModel->rowCount();++i)
        layersItemModel->item(i)->setBackground(*inActiveLayerBackGroundBrush);
    (layersItemModel->itemFromIndex(modelindex))->setBackground(*activeLayerBackGroundBrush);


    layerListView->setCurrentIndex(modelindex);

    XLayer* layer = NULL;
    layer = /*static_cast<XLayer*>*/(modelindex.data(LayerRole).value<XLayer*>());
    if(layer)
        getScene()->setActualLayer(layer);
}
/**
 * WorkFrame::LayersItemChecked
 * layer lathatova tetele fenti miatt bool beallitasa
 */
void WorkFrame::LayersItemChecked(QStandardItem *item)
{
    XLayer* layer = NULL;
    layer = item->data(LayerRole).value<XLayer*>();
    if(layer){
        bool checkState = item->checkState()==Qt::Checked;
        layer->setActive(checkState);
    }
    visibilitychanged = true;
    getScene()->update();
}
/**
 * WorkFrame::SetCharForGraphicsScene
 * workframehez tarsitott char valtoztatasa -> scene csereje
 */
void WorkFrame::SetCharForGraphicsScene(int)
{
    this->drawGraphicsView->deselect();
    getScene()->removeShowingWorkFrame(this);
    char c = /*(char)*/(characterChoseCombo->currentText().toLocal8Bit()[0]);
    if(charMap[c])
        SetGraphicsScene(charMap[c]);
    else{
        XGraphicsScene* sc = new XGraphicsScene(NULL,c);
        SetGraphicsScene(sc);
        charMap[c] = sc;
    }
    getScene()->addShowingWorkFrame(this);


}
/**
 * WorkFrame::SetGraphicsScene
 * workframehez/drawGraphicsView-hez tarsitott scene beallitasa
 */
void WorkFrame::SetGraphicsScene(XGraphicsScene *scene)
{

    this->drawGraphicsView->SetXScene(scene); //duplikált pointer
}


void WorkFrame::setLineColor(const QColor *color)
{
    drawGraphicsView->getXScene()->setLineColor(color);
}

void WorkFrame::setFillColor(const QColor *color)
{
    drawGraphicsView->getXScene()->setFillColor(color);
}

void WorkFrame::pasteFromClipBoard()
{
    this->drawGraphicsView->WaitingForPos();
}
/**
 * WorkFrame::WorkFrame
 * konstruktor
 */
WorkFrame::WorkFrame(const QString &name, XGraphicsScene* scene, QWidget *parent)
    : QFrame(parent)
{
    MainWindow::frames.append(this);
    SetUpWorkFrame(scene);
}
/**
 * WorkFrame::WorkFrame
 * copy-konstruktor duplikalashoz
 */
WorkFrame::WorkFrame(const WorkFrame &other) /*: QFrame(other)*/
{
    MainWindow::frames.append(this);
    XGraphicsScene* sc = other.getScene();
    SetUpWorkFrame(sc);
    handlePos= other.handlePos;
}

void DrawGraphicsView::leaveEvent(QEvent *e)
{
    //    if(actCurve){
    //        xScene->removeItem(actCurve);
    //    if(actCurve->isFirstCurve)
    //        xScene->StartCurves.removeLast();
    //    delete actCurve;
    //    }
    //    actCurve=NULL;
    //    lastPoint=NULL;
    //    this->getScene()->colors.at(0)->update();
    //    viewport()->update();
}
/**
 * WorkFrame::WorkFrame
 * @param pt pozicio amihez legkozelebbi gorbet keressuk vegpont alapjan
 * @param transform dummy
 * vegpont kereses gorbe bezarasahoz, hasznalaton kivul
 */
QPointF *DrawGraphicsView::findNearestCurveByEndPoint(QPointF pt, QTransform transform)
{
    BezierCurve* nearestCurve = NULL;
    nearestCurve = findNearestCurveByControlPoint( pt,  transform);
    if(!nearestCurve)
        return NULL;
    double dummy;
    QPointF* nearestPoint = nearestCurve->getNearestControlPoint(pt,dummy);
    if(nearestPoint == &nearestCurve->getFirstEndPoint()||(nearestPoint == &nearestCurve->getSecondEndPoint()))
        return new QPointF(*nearestPoint);
    else
        return NULL;
}

/**
 * WorkFrame::getDrawGraphicsView
 * tartalmazott DrawGraphicsView visszaadasa kasztolva
 */
QGraphicsView *WorkFrame::getDrawGraphicsView() const
{
    return static_cast<QGraphicsView *>(drawGraphicsView);
}

void WorkFrame::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_T){
        if(scrollArea->isHidden())
            scrollArea->show();
        else
            scrollArea->hide();
    }
}
/**
 * WorkFrame::paintEvent
 * duplikalasra hasznlt terulet jelzese
 */
void WorkFrame::paintEvent(QPaintEvent *pe)
{

    QFrame::paintEvent(pe);
    QRect region = pe->rect();
    QPainter painter(this);
    QPen pen(Qt::red); //Note: set line colour like this

    pen.setWidth(10);
    painter.setPen(pen);
    painter.drawLine(width()-10,0,width(),10);

}
/**
 * WorkFrame::mousePressEvent
 * duplikalas kezdete
 */
void WorkFrame::mousePressEvent(QMouseEvent *me)
{
    if(me->pos().x()+me->pos().y()<width()-10)
        return;
    reOrg/*=moveSplitterAfterReorg*/=true;
    handlePos=me->globalPos();
    setMouseTracking(true);
}
/**
 * WorkFrame::mouseMoveEvent
 * mozgas iranyatol fugg eloszor milyen iranyu splittert szurunk be
 */
void WorkFrame::mouseMoveEvent(QMouseEvent *me)
{

    if(moveSplitterAfterReorg){
        ((BlenderSplitter*)parent())->moveSplitter(handlePos-me->globalPos(),0);
        qDebug()<<"moveSp/n";
    }
    else if(reOrg){
        ((BlenderSplitter*)parent())->reOrganize(handlePos-me->globalPos(),this);

    }

}

void WorkFrame::mouseReleaseEvent(QMouseEvent *)
{

    moveSplitterAfterReorg=false;
    setMouseTracking(false);
}

void WorkFrame::leaveEvent(QEvent *e)
{

    if(!moveSplitterAfterReorg)
        return;
    qDebug()<<"------------------------LEAVE-----------------------------";
    moveSplitterAfterReorg=false;
    setMouseTracking(false);
}

/**
* DrawGraphicsView::moveConstrainly
* @param newpt transzfolmalt pozicio visszaadva eredeti helyen
* @param startPt ebbol kiindulo vonalak menten haldhat a pont
* @return kenyszerites vertical vagy horizontal - hasznalaton kivul
*/
contstrainedMoveDirection DrawGraphicsView::moveConstrainly(QPointF& newpt, QPointF startPt)
{
    contstrainedMoveDirection ret = none;
    double xDiff=abs(newpt.x()-startPt.x());
    double yDiff=abs(newpt.y()-startPt.y());
    if(xDiff > yDiff && yDiff/xDiff < (double  )2/3){
        newpt.setY(startPt.y());
        ret = horizontal;
    }
    else if(xDiff/yDiff < (double  )2/3){
        newpt.setX(startPt.x());
        ret = vertical;
    }
    else{
        newpt.setY(startPt.y()<newpt.y()?startPt.y()+abs(newpt.x()-startPt.x()):startPt.y()-abs(newpt.x()-startPt.x()));
    }
    return ret;

}
/**
* DrawGraphicsView::DrawConstraintLines
* @param pos ehhez kozeli pontokat keressuk valamely kordinatengely iranyban
* @param Constraining esetleges raszteres kényszerítés iránya
* osszegujti a segédvonalakat az adott poziciohoz - sulyos instabilitas miatt kiveve
*/
void DrawGraphicsView::DrawConstraintLines(QPointF pos, contstrainedMoveDirection Constraining)
{
    QListIterator<QGraphicsLineItem*> li(ConstrLineItems) ;
    while (li.hasNext()) {
        QGraphicsLineItem* item=li.next();
        xScene->removeItem(item);
        delete item;
    }
    if(nearestLineItem){
        xScene->removeItem(nearestLineItem);
        delete nearestLineItem;
        nearestLineItem = NULL;
    }

    ConstrLineItems.clear();
    QList<QLineF *> ConstrLines(*xScene->getConstraintLines(pos));
    QListIterator<QLineF *> acl(ConstrLines);
    double mindistance = 100;
    QLineF* nearestLine = NULL;
    //QLineF* secondNearestLine = NULL;
    while(acl.hasNext())
    {

        QLineF* actLine = acl.next();
        if(((isLineHorizontal(actLine) && (Constraining == vertical) )
            ||((!isLineHorizontal(actLine))&&(Constraining == horizontal))
            || Constraining == none)
                &&(mindistance > getConstrLineDist(&pos,actLine))){
            nearestLine = actLine;
        }

    }

    acl.toFront();
    while(acl.hasNext())
    {
        QLineF* actLine=  acl.next();
        if(actLine != nearestLine) {
            ConstrLineItems.append(xScene->addLine(*actLine));
        }
    }

    if(nearestLine)
        nearestLineItem = xScene->addLine(*nearestLine,QPen(Qt::red));
    this->getXScene()->colors.at(0)->update();
    viewport()->update();

}
/**
* DrawGraphicsView::ClearConstraintLines
* torli a segédvonalakat
*/
void DrawGraphicsView::ClearConstraintLines()
{
    QListIterator<QGraphicsLineItem*> li(ConstrLineItems) ;
    while (li.hasNext()) {
        QGraphicsLineItem* item=li.next();
        xScene->removeItem(item);
        delete item;
    }
    if(nearestLineItem)
        delete nearestLineItem;
    nearestLineItem= NULL;
    ConstrLineItems.clear();
    if(constrainedPositionPoint){
        delete constrainedPositionPoint;
        constrainedPositionPoint = NULL;
    }
    if(constrainedPositionEllipse)
    {
        getXScene()->removeItem(constrainedPositionEllipse);
        delete constrainedPositionEllipse;
        constrainedPositionEllipse=NULL;
    }
}
/**
* DrawGraphicsView::mouseMoveEvent
* mozgas alapjan editmod es drawmode szerint vezerli az aktualis transzformaciot
*/
void DrawGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    bool constraining = QApplication::keyboardModifiers()==Qt::ShiftModifier /*|| MainWindow::ConstraintToConstraintLine || MainWindow::ConstraintToPoint*/;
    ClearConstraintLines();
    if(MainWindow::editMode==/*editmodes::*/draw){

        QPointF pt = mapToScene(event->pos());

        if(drawMode == curve){
            if(actCurve){

                if(event->buttons() == Qt::LeftButton){

                    actCurve->moveHandlePoint(pt);
                }
                else{
                    contstrainedMoveDirection dir = none;
                    if(constraining)
                        dir = moveConstrainly(pt,actCurve->getFirstEndPoint());
                    //                    DrawConstraintLines(pt,dir);
                    //                    constrainedPositionPoint = findNearestCurveByEndPoint(pt,QTransform());
                    //                   if(MainWindow::ConstraintToPoint && constrainedPositionPoint){
                    //                          pt = *constrainedPositionPoint;
                    //                          constrainedPositionEllipse = getXScene()->addEllipse(constrainedPositionPoint->x()-10,constrainedPositionPoint->y()-10,20,20,QPen(Qt::red));
                    //                   }
                    //                      /* else*/ if(MainWindow::ConstraintToConstraintLine){
                    //                        if(nearestLineItem){
                    //                            if(dir == horizontal)
                    //                                pt.setX(nearestLineItem->line().p1().x());
                    //                            else if(dir == vertical)
                    //                                pt.setY(nearestLineItem->line().p1().y());
                    //                            else{
                    //                                QLineF nearestLine = nearestLineItem->line();
                    //                                if(isLineHorizontal(&nearestLine))
                    //                                    pt.setY(nearestLineItem->line().p1().y());
                    //                                 else
                    //                                    pt.setX(nearestLineItem->line().p1().x());
                    //                            }

                    //                        }
                    //                    }


                    actCurve->moveEndPoint(pt);
                }

            }
            else if(firstHandlePoint)
                *firstHandlePoint=pt;
        }
        else if(drawMode == ellipse)
        {
            if(!actEllipse)
                return;
            if(constraining)
                moveConstrainly(pt,actEllipse->getFirstBoxPoint());
            actEllipse->moveSecondBoxPoint(pt);
        }
        else if(drawMode == rectangle)
        {
            if(!actRect)
                return;
            if(constraining)
                moveConstrainly(pt,actRect->getFirstBoxPoint());
            actRect->moveSecondBoxPoint(pt);
        }
        if(drawMode == mirror)
        {

            if(event->buttons() == Qt::LeftButton){
                if(mirrorAxis){
                    mirrorAxis->setP2(pt);
                    mirrorAxisLineItem->setLine(*mirrorAxis);
                }
            }

        }
        if(drawMode == rotateMode)
        {
            if(event->buttons() == Qt::LeftButton){
                if(origVector){
                    qDebug()<<"rotate";

                    newVector = new QVector2D(pt-*originalMiddlePoint);
                    QLineF origLine(0,0,origVector->x(),origVector->y());
                    QLineF newLine(0,0,newVector->x(),newVector->y());
                    double angle = newLine.angleTo(origLine);//qAcos((origVector->x()*newVector->x()+origVector->y()*newVector->y())/(origVector->length()*newVector->length()))*180;
                    qDebug()<<"angle: "<<angle;
                    QTransform* tr = XTransform::rotate(originalMiddlePoint,angle);

                    for(int i=0;i<xScene->selectedPoints.size();++i)
                        (*xScene->selectedPoints[i]) = (*tempPointList[i])*(*tr);
                    this->newVectorsImage->setLine(this->originalMiddlePoint->x(),this->originalMiddlePoint->y(),pt.x(),pt.y());


                }
            }

        }
        if(drawMode == moveMode)
        {
            if(event->buttons() == Qt::LeftButton){
                if(origVector){
                    newVector = new QVector2D(pt-*originalMiddlePoint);

                    QVector2D* moveVector =new QVector2D(*newVector - *origVector);
                    QTransform* tr = XTransform::move(moveVector);

                    for(int i=0;i<xScene->selectedPoints.size();++i)
                        (*xScene->selectedPoints[i]) = (*tempPointList[i])*(*tr);

                    QListIterator<XGraphicsPixmapItem*> pi(getXScene()->pixmaps);
                    while(pi.hasNext())
                        ((XGraphicsPixmapItem*)pi.next())->move(/*pli.next(),*/moveVector);
                    this->newVectorsImage->setLine(this->originalMiddlePoint->x(),this->originalMiddlePoint->y(),pt.x(),pt.y());


                }
            }

        }
        if(drawMode == scaleMode)
        {
            if(event->buttons() == Qt::LeftButton){
                if(origVector){
                    newVector = new QVector2D(pt-*originalMiddlePoint);

                    double scale = newVector->length() / origVector->length();
                    qDebug()<<"SCALING "<<newVector->length()<<" : "<<origVector->length();
                    QTransform* tr = XTransform::scale(originalMiddlePoint,scale);

                    for(int i=0;i<xScene->selectedPoints.size();++i){
                        QPointF ptForNanTest( (*tempPointList[i])*(*tr));
                        if(!(XTransform::isValidCoordinate(ptForNanTest.x()) && XTransform::isValidCoordinate(ptForNanTest.y())))
                            return;
                    }
                    for(int i=0;i<xScene->selectedPoints.size();++i)
                        (*xScene->selectedPoints[i]) = (*tempPointList[i])*(*tr);
                    QListIterator<XGraphicsPixmapItem*> pi(getXScene()->pixmaps);
                    QPointF* dummy = new QPointF(0,0);
                    while(pi.hasNext()){
                        XGraphicsPixmapItem* act = (XGraphicsPixmapItem*)pi.next();
                        QPointF actCp(act->pos().x() + act->getWidth()/2, act->pos().y() + act->getHeight()/2);
                        act->scale(&actCp,scale);
                    }
                    this->newVectorsImage->setLine(this->originalMiddlePoint->x(),this->originalMiddlePoint->y(),pt.x(),pt.y());


                }
            }

        }



    }
    else if(actCurve){
        QPointF pt = mapToScene(event->pos());
        bool moveAll = QApplication::keyboardModifiers()!=Qt::AltModifier;
        actCurve->mouseMoveEvent(pt,moveAll);
        DrawConstraintLines(pt,none);
    }
    viewport()->update();
    this->repaint();
    //actCurve->repaint();
}
/**
* DrawGraphicsView::findNearestCurveByControlPoint
* @param pt viszonyitasi pont
* @param transform dummy
* @return az adott poziciohoz legkozelebbi kontrolponttal rendelkezo gorbet adja vissza ha CatchPointDistance-nal kisebb a tavolsag
* a tavolsagokat e gorbektol keri el itt csak minimum kivalasztasa
*/
BezierCurve* DrawGraphicsView::findNearestCurveByControlPoint(QPointF pt,QTransform transform)
{
    BezierCurve* nearestCurve;
    BezierCurve* topMostItem=NULL;
    topMostItem=(BezierCurve*)xScene->itemAt(pt,transform);
    if(!topMostItem)
        return NULL;
    double dist=10000;
    if(( (XGraphicsItem*)topMostItem)->isBezierCurve()){
        nearestCurve=topMostItem;
        topMostItem->getNearestControlPoint(pt,dist);
    }
    double actDist=-1;
    if(!topMostItem->collidingItems().empty())
        for(int i=0; i<topMostItem->collidingItems().size();++i){
            if(!((XGraphicsItem*)topMostItem->collidingItems().at(i))->isBezierCurve())continue;
            ((BezierCurve*)topMostItem->collidingItems().at(i))->getNearestControlPoint(pt,actDist);
            if(actDist<dist)
            {
                dist=actDist;
                nearestCurve=(BezierCurve*)topMostItem->collidingItems().at(i);
            }
        }
    return dist>CatchPointDistance?NULL:nearestCurve;
}
/**
* DrawGraphicsView::findNearestCurveByCurvePoint
* @param pt viszonyitasi pont
* @param transform dummy
* @return az adott poziciohoz legkozelebbi gorbevonallal rendelkezo gorbet adja vissza ha CatchPointDistance-nal kisebb a tavolsag
* a tavolsagokat e gorbektol keri el itt csak minimum kivalasztasa
*/
BezierCurve* DrawGraphicsView::findNearestCurveByCurvePoint(QPointF pt,QTransform transform)
{
    BezierCurve* nearestCurve=NULL;
    BezierCurve* topMostItem=(BezierCurve*)xScene->itemAt(pt,transform);
    if(!topMostItem)
        return NULL;
    double dist=-1;
    if(( (XGraphicsItem*)topMostItem)->isBezierCurve())
        topMostItem->getNearestCurvePoint(pt,dist);
    else
        dist=100000;
    if(dist<5)
        nearestCurve=topMostItem;
    double actDist=-1;
    if(!topMostItem->collidingItems().empty())
        for(int i=0; i<topMostItem->collidingItems().size();++i){
            QPointF* p=NULL;
            if(( (XGraphicsItem*)topMostItem->collidingItems().at(i))->isBezierCurve())
                p =((BezierCurve*)topMostItem->collidingItems().at(i))->getNearestCurvePoint(pt,actDist);
            if(p && actDist<dist)
            {
                dist=actDist;
                nearestCurve=(BezierCurve*)topMostItem->collidingItems().at(i);
            }
        }
    return nearestCurve;

}
/**
* DrawGraphicsView::mouseDoubleClickEvent
* a szerkesztes alatt allo gorbesorozat lezarasa
*/
void DrawGraphicsView::mouseDoubleClickEvent(QMouseEvent *event )
{
    if ((MainWindow::editMode == draw) && (drawMode ==  curve)&& actCurve){
        BezierCurve* lastCurve=actCurve->getPreviousCurve();
        xScene->removeItem(actCurve);
        if(!lastCurve)
            xScene->StartCurves.removeOne(actCurve);
        delete actCurve;
        if(!lastCurve){
            actCurve=NULL;
            lastPoint=NULL;
            return;
        }
        lastCurve->getSecondEndPoint()=xScene->actLoopFirstCurve->getFirstEndPoint();
        lastCurve->SetNextCurve(xScene->actLoopFirstCurve);
        xScene->actLoopFirstCurve->setPreviousCurve(lastCurve);
    }

    actCurve=NULL;
    lastPoint=NULL;

}
/**
* DrawGraphicsView::mouseReleaseEvent
* a hasonlosagi transzformaciokanal az egermozgas szerinti folyamatos transzformavio vege, temp adatok torlese
* tukrozes vegrehajtasa mostanra megadott tengelyre
* gorbeknel a handle szerkesztesenek vege
*/
void DrawGraphicsView::mouseReleaseEvent(QMouseEvent *event )
{
    if(MainWindow::editMode==/*editmodes::*/draw){
        if(lastPoint){
            QPointF pt = mapToScene(event->pos());

            if(actCurve){

                actCurve->editing=false;
            }
            actCurve=new BezierCurve(*lastPoint,pt,this->xScene,this->xScene->getActualLayer(), actCurve);
            if(firstHandlePoint)
            {
                actCurve->getFirstHandlePoint()=*firstHandlePoint;
                xScene->StartCurves.append(actCurve);
                actCurve->isFirstCurve=true;
                xScene->actLoopFirstCurve=actCurve;
                delete firstHandlePoint;
                firstHandlePoint=NULL;
            }
            getXScene()->addItem(actCurve);

        }
        if(drawMode == mirror){
            QPointF* p1=xScene->getMiddlePointOfSelectedShape();
            QTransform* tr ;
            if(qAbs(mirrorAxis->p1().x()-mirrorAxis->p2().x())>qAbs(mirrorAxis->p1().y()-mirrorAxis->p2().y()))
                tr = XTransform::reflectX(p1);
            else
                tr = XTransform::reflectY(p1);
            for(int i= 0;i<xScene->selectedPoints.size();++i){
                (*xScene->selectedPoints[i]) = /*(*tr)**/(*xScene->selectedPoints[i])*(*tr);
                //                   qDebug()<<"after:"<<scene->selectedPoints[i]->x()<<" "<<scene->selectedPoints[i]->y();
                //                   ///QPointF* act = scene->selectedPoints[i];
            }
            delete mirrorAxis;
            mirrorAxis=NULL;
            xScene->removeItem(mirrorAxisLineItem);
            delete mirrorAxisLineItem;
            mirrorAxisLineItem=NULL;

        }

        if(drawMode == rotateMode || drawMode == moveMode || drawMode == scaleMode){
            delete origVector;
            delete newVector;
            delete originalMiddlePoint;
            qDeleteAll(tempPointList);
            if(this->newVectorsImage){
                this->getXScene()->removeItem(this->newVectorsImage);
                delete this->newVectorsImage;
                newVectorsImage = NULL;
            }
            if(this->origVectorsImage){
                this->getXScene()->removeItem(this->origVectorsImage);
                delete this->origVectorsImage;
                origVectorsImage = NULL;
            }
            tempPointList.clear();
        }

    }
    qDebug()<<"released/n";
    ClearConstraintLines();
    this->getXScene()->colors.at(0)->update();
    viewport()->update();
}
/**
* DrawGraphicsView::mouseReleaseEvent
* a hasonlosagi transzformaciokanal az egermozgas szerinti folyamatos transzformavio kezdete, temp adatok feltoltese
* tukrozes tengely elso pontjanak fixalasa
* gorbeknel a vegpont lerakasa handle-szerkesztes kezdete
*/
void DrawGraphicsView::mousePressEvent(QMouseEvent *event )
{

    if(MainWindow::editMode==/*editmodes::*/draw){
        //       if(!actCurve){
        QPointF pt = mapToScene(event->pos());
        if(drawMode == curve){
            if(!lastPoint){
                getXScene()->saveState();
                QTransform transform;
                BezierCurve* ClickedCurve=findNearestCurveByCurvePoint(pt,transform);
                xScene->addEllipse(pt.x(),pt.y(),0,0);
                if(ClickedCurve){
                    BezierCurve* newCurve=NULL;
                    newCurve=ClickedCurve->devideCurve();
                    if(newCurve){
                        xScene->addItem(newCurve);
                        this->getXScene()->colors.at(0)->update();
                        newCurve->update();
                    }
                    return;
                }
                firstHandlePoint=new QPointF(pt);
            }
            bool constraining = QApplication::keyboardModifiers()==Qt::ShiftModifier;
            if(constraining && actCurve)
            {

                moveConstrainly(pt,actCurve->getFirstEndPoint());

            }
            if(lastPoint)
                delete lastPoint;
            lastPoint=new QPointF(pt);

        }
        if(drawMode == ellipse)
        {
            if(!actEllipse){
                getXScene()->saveState();
                actEllipse = new XEllipseItem(pt,pt,getXScene(),getXScene()->getActualLayer()) ;
                xScene->addItem(actEllipse);
            }
            else actEllipse=NULL;
        }
        if(drawMode == rectangle)
        {
            if(!actRect){
                getXScene()->saveState();
                actRect = new XRectangleItem(pt,pt,getXScene(),getXScene()->getActualLayer()) ;
                xScene->addItem(actRect);
            }
            else actRect=NULL;
        }
        if(drawMode == mirror)
        {
            if(!mirrorAxis){
                getXScene()->saveState();
                mirrorAxis = new QLineF(pt,pt);
                mirrorAxisLineItem = xScene->addLine(*mirrorAxis);
            }

        }
        if(drawMode == rotateMode)
        {
            getXScene()->saveState();
            QListIterator<QPointF*> li(xScene->selectedPoints);
            while(li.hasNext())
                tempPointList.append(new QPointF(*((QPointF*)li.next())));
            originalMiddlePoint= xScene->getMiddlePointOfSelectedShape();
            origVector=new QVector2D(pt-*originalMiddlePoint);
            this->origVectorsImage = this->scene()->addLine(originalMiddlePoint->x(),originalMiddlePoint->y(),pt.x(),pt.y());
            this->newVectorsImage = this->scene()->addLine(originalMiddlePoint->x(),originalMiddlePoint->y(),pt.x(),pt.y());


        }
        if(drawMode == moveMode)
        {
            getXScene()->saveState();
            QListIterator<QPointF*> li(xScene->selectedPoints);
            while(li.hasNext())
                tempPointList.append(new QPointF(*((QPointF*)li.next())));
            originalMiddlePoint = xScene->getMiddlePointOfSelectedShape();
            origVector=new QVector2D(pt-*originalMiddlePoint);
            this->origVectorsImage = this->scene()->addLine(originalMiddlePoint->x(),originalMiddlePoint->y(),pt.x(),pt.y());
            this->newVectorsImage = this->scene()->addLine(originalMiddlePoint->x(),originalMiddlePoint->y(),pt.x(),pt.y());
            setSelectedPixmapsPos();

        }
        if(drawMode == scaleMode)
        {
            getXScene()->saveState();
            QListIterator<QPointF*> li(xScene->selectedPoints);
            originalMiddlePoint = xScene->getMiddlePointOfSelectedShape();
            while(li.hasNext())
                tempPointList.append(new QPointF(*((QPointF*)li.next())));

            origVector=new QVector2D(pt-*originalMiddlePoint);
            this->origVectorsImage = this->scene()->addLine(originalMiddlePoint->x(),originalMiddlePoint->y(),pt.x(),pt.y());
            this->newVectorsImage = this->scene()->addLine(originalMiddlePoint->x(),originalMiddlePoint->y(),pt.x(),pt.y());
            setSelectedPixmapsPos();
        }
        if(drawMode == selectMode)
        {
            QTransform transform;
            QPointF pt = mapToScene(event->pos());
            actCurve = findNearestCurveByCurvePoint(pt,transform);
            if(QApplication::keyboardModifiers()!=Qt::ControlModifier)
                getXScene()->clearShapeSelection();
            if(actCurve){
                if(actCurve->isCurveSelected())
                    actCurve->deSelectThisShape();
                else
                    actCurve->selectThisShape();
            }
            QListIterator<XGraphicsPixmapItem*> pi(this->getXScene()->pixmaps);
            while(pi.hasNext())
            {
                XGraphicsPixmapItem* act = (XGraphicsPixmapItem*)pi.next();
                if(act->boundingRect().contains(pt)){
                    act->mousePressEvent(event);
                }
            }
        }
        if(drawMode == waitForPastePos)
        {
            getXScene()->saveState();
            QPointF* offsetPoint = new QPointF(pt - *MainWindow::clipBoardOrigo);
            getXScene()->paste(offsetPoint,MainWindow::curveClipBoard,MainWindow::pixmapClipBoard);
            delete offsetPoint;
        }



    }
    else if(MainWindow::editMode == edit) {
        if(drawMode !=selectMode){
            if(!actCurve){
                getXScene()->saveState();
                QPointF pt = mapToScene(event->pos());
                QTransform transform;
                actCurve=findNearestCurveByControlPoint(pt, transform);
                if(actCurve)
                    actCurve->mousePressEvent(pt);
            }
            else{
                actCurve=NULL;
            }
        }
        ////// Bugos kijelölés
        //        else
        //        {
        //            QTransform transform;
        //            QPointF pt = mapToScene(event->pos());
        //            actCurve = findNearestCurveByCurvePoint(pt,transform);
        //            if(QApplication::keyboardModifiers()!=Qt::ControlModifier)
        //                getXScene()->clearShapeSelection();
        //            if(actCurve){
        //                if(actCurve->isCurveSelected())
        //                    actCurve->deSelectThisCurve();
        //                else
        //                    actCurve->selectThisCurve();
        //            }
        //            QListIterator<XGraphicsPixmapItem*> pi(this->getXScene()->pixmaps);
        //            while(pi.hasNext())
        //            {
        //                XGraphicsPixmapItem* act = (XGraphicsPixmapItem*)pi.next();
        //                if(act->boundingRect().contains(pt)){
        //                    act->mousePressEvent(event);
        //                }
        //            }
        //        }
    }


    this->getXScene()->colors.at(0)->update();
    viewport()->update();



}
void DrawGraphicsView::setSelectedPixmapsPos(){
    QListIterator<XGraphicsPixmapItem*> pli(xScene->selectedPixmaps);
    while(pli.hasNext()){
        ((XGraphicsPixmapItem*)pli.next())->setPositionBeforeTransform();
    }
}

void DrawGraphicsView::WaitingForPos()
{
    SetDrawMode(waitForPastePos);
}

void DrawGraphicsView::newSelectedItems()
{
    getXScene()->saveState();
    QPointF* offsetPoint = new QPointF(0,0/*pt - *//**MainWindow::clipBoardOrigo*/);
    getXScene()->paste(offsetPoint,MainWindow::curveClipBoard,MainWindow::pixmapClipBoard);
    delete offsetPoint;
}

double DrawGraphicsView::getConstrLineDist(QPointF* point, QLineF *line)
{
    //    bool horizontal = (line->p1().y() - line->p2().y())<0.001;
    if(isLineHorizontal(line))
        return qAbs(point->y()-line->p1().y() ) ;
    else
        return qAbs(point->x()-line->p1().x() ) ;

}

bool DrawGraphicsView::isLineHorizontal(QLineF* line)
{
    return (line->p1().y() - line->p2().y())<0.001;
}


void DrawGraphicsView::deselect()
{
    if(actCurve){
        xScene->removeItem(actCurve);
        if(actCurve->isFirstCurve)
            xScene->StartCurves.removeLast();
        delete actCurve;
    }
    //    if(actEllipse)
    //    {
    //            xScene->removeItem(actCurve);
    //            if(actCurve->isFirstCurve)
    //                xScene->StartCurves.removeLast();
    //            delete actCurve;
    //        }
    //    if(actRect)
    //    {
    //            xScene->removeItem(actRect);
    //            if(actCurve->isFirstCurve)
    //                xScene->StartCurves.removeLast();
    //            delete actCurve;
    //     }
    actCurve=NULL;
    lastPoint=NULL;
}

void DrawGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return)
    {
        deselect();
    }
    this->getXScene()->colors.at(0)->update();
    viewport()->update();
}

void DrawGraphicsView::wheelEvent ( QWheelEvent * event )
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
        xScene->scaleFactor*=scaleFactor;
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        xScene->scaleFactor*=1.0 / scaleFactor;
    }
    this->getXScene()->colors.at(0)->update();
    viewport()->update();
    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}

