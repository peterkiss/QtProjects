#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>






#include <qlabel.h>

QMap<char,XGraphicsScene*> charMap;

editmodes MainWindow::MainWindow::editMode = draw;
bool MainWindow::ConstraintToConstraintLine = false;
bool MainWindow::ConstraintToPoint = false;
QPointF* MainWindow::clipBoardOrigo = NULL;
XGraphicsScene* MainWindow::lastModifiedScene = NULL;
QList<BezierCurve*>* MainWindow::curveClipBoard = NULL;
QList<XGraphicsPixmapItem*>* MainWindow::pixmapClipBoard = NULL;
QList<WorkFrame*> MainWindow::frames;
double actScale=1;

/**
 * @brief MainWindow::fillCharMap
 *  user interface felepiteste
 *
 */


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QPixmap* appPm = new QPixmap(":/typo/icon/logo.png");
    QIcon* appIcon = new QIcon(*appPm);
    this->setWindowIcon(*appIcon);

    QPixmap* stcPm = new QPixmap(":/typo/icon/snap_to_curve.png");
    QIcon* stcIcon = new QIcon(*stcPm);
    QPixmap* stpPm = new QPixmap(":/typo/icon/snap_to_point.png");
    QIcon* stpIcon = new QIcon(*stpPm);
    ui->setupUi(this);
    this->setWindowTitle("XTypo") ;
    createActions();
    createMenus();
    designFrame = new QFrame;
    h1Splitter = new BlenderSplitter;

    fillCharMap();

    WorkFrame *wf = new WorkFrame("Top left view",scene);

    h1Splitter->addWidget(wf);

    h1Splitter->addWidget(wf);

    QHBoxLayout *labelLayout = new QHBoxLayout;
    QHBoxLayout* designFrameLayout = new QHBoxLayout;
    modeCombobox = new QComboBox;
    modeCombobox->addItem("Draw");
    modeCombobox->addItem("Modify");
    labelLayout->addWidget(modeCombobox);
    modeCombobox->setFixedWidth(200);
    labelLayout->addStretch();

    PointConstraintToolButton = new QToolButton;
    PointConstraintToolButton->setIcon(*stpIcon);
    PointConstraintToolButton->setToolTip("Constr. to Point - out of order");
    PointConstraintToolButton->setCheckable(true);
    PointConstraintToolButton->setFixedSize(40,40);
    LineConstraintButton = new QToolButton;
    LineConstraintButton->setIcon(*stcIcon);
    LineConstraintButton->setToolTip("Constr. to C.Line - out of order");
    LineConstraintButton->setCheckable(true);
    LineConstraintButton->setFixedSize(40,40);


    constraintFrame = new QFrame;
    constraintFrame->setFixedWidth(120);
    QHBoxLayout* constrLayout = new QHBoxLayout;
    constrLayout->setMargin(0);
    constrLayout->addWidget(PointConstraintToolButton);
    constrLayout->addWidget(LineConstraintButton);
    constraintFrame->setLayout(constrLayout);
    labelLayout->addWidget(constraintFrame);
    labelLayout->setMargin(3);
    connect(modeCombobox,SIGNAL(currentIndexChanged( int   )),this,SLOT(setMode(   int   )));
    connect(PointConstraintToolButton,SIGNAL(clicked(bool)),this,SLOT(setConstraintToPoint(bool)));
    connect(LineConstraintButton,SIGNAL(clicked(bool)),this,SLOT(setConstraintToConstraintLine(bool)));
    modeFrame = new QFrame;
    modeFrame->setLayout(labelLayout);
    modeFrame->setFixedHeight(45);
    designFrameLayout->addWidget(h1Splitter);
    designFrame->setLayout(designFrameLayout);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(modeFrame);
    layout->addWidget(designFrame);

    ui->centralWidget->setLayout(layout);



}
void MainWindow::setConstraintToPoint(bool checkstate)
{
    ConstraintToPoint = checkstate;
}
void MainWindow::setConstraintToConstraintLine(bool checkstate)
{
    ConstraintToConstraintLine = checkstate;
}

void MainWindow::setMode(int index)
{
    editMode=(editmodes)index;

}
/**
 * @brief MainWindow::fillCharMap
 *
 */
void MainWindow::fillCharMap()
{
    for(int i =0;i<256;++i)
        charMap.insert((char)i,NULL);
    scene = new XGraphicsScene(NULL,'a');
    charMap['a']=scene;

}

void MainWindow::ClearMap()
{
    qDeleteAll(charMap);
    for(int i =0;i<256;++i)
        charMap[(char)i]=NULL;
}
/**
 * @brief MainWindow::newSession
 *  uj munkamenet, regi torlese
 *
 */

void MainWindow::newSession()
{
    QMessageBox msgBox;
    msgBox.setText("New session");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText("If you did not save your work, your changes will be lost./nDou you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret ==QMessageBox::Cancel )
        return;
    ClearMap();
    scene = new XGraphicsScene(NULL,'a');
    charMap['a']=scene;
    QListIterator<WorkFrame*> frameIt(frames);
    while(frameIt.hasNext())
        frameIt.next()->setScene(charMap['a']);

}
/**
 * @brief MainWindow::saveSession
 *   munkamenet fajlba mentese
 *
 */
void MainWindow::saveSession()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"), "C:\\", tr("TTX Files (*.ttx )"));
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QMapIterator<char,XGraphicsScene*> mapiterator(charMap);
    while(mapiterator.hasNext()){
        XGraphicsScene*  sc = mapiterator.next().value();
        if(sc){
            QStringList strl = sc->toString();
            QStringListIterator strlIt(strl);
            while(strlIt.hasNext())
                out<<strlIt.next()<</*std::*/endl;
        }
    }
    file.close();

}
/**
 * @brief MainWindow::openSession
 *   munkamenet fajlbol begtoltese
 *
 */
void MainWindow::openSession()
{
    QMessageBox msgBox;
    msgBox.setText("New session");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText("If you did not save your work, your changes willbe lost./nDou you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret ==QMessageBox::Cancel )
        return;

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), "C:\\", tr("TTX Files (*.ttx )"));
    if(fileName.isEmpty())
        return;
    try{
        ClearMap();
        QFile file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QStringList fontDataStringList;
            bool foundChar = false;
            QString str;
            bool endofFont = false;
            char associatedChar;
            while(!endofFont){
                str = in.readLine();
                if(!foundChar){
                    associatedChar =  str.toLocal8Bit()[0];
                    foundChar= true;
                }
                fontDataStringList.append(str);
                if(str == "#")
                    endofFont = true;
            }
            if(foundChar)
            {
                if(!charMap[associatedChar])
                    charMap[associatedChar]= new XGraphicsScene(NULL,associatedChar);
                charMap[associatedChar]->clear();
                charMap[associatedChar]->fromString(fontDataStringList);
            }



        }
        file.close();
    }catch(...)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Error");
        msgBox.setInformativeText("An error occured during reading file.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    bool foundExistingCharDescription = false;
    XGraphicsScene* sc = NULL;
    QMapIterator<char,XGraphicsScene*> mapiterator(charMap);
    while(mapiterator.hasNext() && !foundExistingCharDescription){
        sc = mapiterator.next().value() ;
        if(sc)
            foundExistingCharDescription = true;

    }
    if(!sc){
        sc = new XGraphicsScene(NULL,'a');
        charMap['a'] = sc;
    }
    QListIterator<WorkFrame*> frameIt(frames);
    while(frameIt.hasNext())
        frameIt.next()->setScene(sc);

}

void MainWindow::exportToOTF()
{

}

void MainWindow::setModifyModeAllFrames()
{
    this->editMode = edit;

}

void MainWindow::setDrawModeAllFrames()
{
    this->editMode = draw;
}

void MainWindow::setConstrainToPointModeAllFrames()
{
    this->ConstraintToPoint = true;
}

void MainWindow::setConstrainToLineModeAllFrames()
{
    this->ConstraintToConstraintLine = true;
}

void MainWindow::setScaleModeAllFrames()
{
    QListIterator<WorkFrame*> frIt(frames);
    while(frIt.hasNext())
        frIt.next()->setScaleMode();
}

void MainWindow::setRectangleDrawModeAllFrames()
{
    QListIterator<WorkFrame*> frIt(frames);
    while(frIt.hasNext())
        frIt.next()->setRectangleMode();
}

void MainWindow::setCurveDrawModeAllFrames()
{
    QListIterator<WorkFrame*> frIt(frames);
    while(frIt.hasNext())
        frIt.next()->setCurveMode();
}

void MainWindow::copySelectedItems()
{
    QListIterator<WorkFrame*> frIt(frames);
    while(frIt.hasNext())
        frIt.next()->copySelectedItems();
}

void MainWindow::setSelectedItemsPasteModeAllFrames()
{
    QListIterator<WorkFrame*> frIt(frames);
    while(frIt.hasNext())
        frIt.next()->pasteSelectedItems();
}

void MainWindow::deleteSelectedItemsAllFrames()
{
    QListIterator<WorkFrame*> frIt(frames);
    while(frIt.hasNext()){
        WorkFrame* actFr = frIt.next();
        if(this->lastModifiedScene==actFr->getScene())
            actFr->deleteSelectedItems();
    }
}

void MainWindow::setRotateModeAllFrames()
{
    QListIterator<WorkFrame*> frIt(frames);
    while(frIt.hasNext())
        frIt.next()->setRotateMode();
}

void MainWindow::setEllipseDrawModeAllFrames()
{
    QListIterator<WorkFrame*> frIt(frames);
    while(frIt.hasNext())
        frIt.next()->setEllipseMode();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    newAct = new QAction(/*tr*/("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(/*tr*/("Create a new session"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newSession()));
    openAct = new QAction/*(tr*/("&Open",this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(/*tr*/("Open session"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openSession()));
    saveAct = new QAction/*(tr*/("&Save",this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveSession()));

    setModifyModeAct = new QAction/*(tr*/("Modifying mode",this);
    //QKeySequence modifySeq("Ctrl+M");
    setModifyModeAct->setShortcut(QKeySequence("Ctrl+Shift+M"));
    setModifyModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setModifyModeAct, SIGNAL(triggered()), this, SLOT(setModifyModeAllFrames()));

    setDrawModeAct = new QAction/*(tr*/("Draw mode",this);
    setDrawModeAct->setShortcut(QKeySequence("Ctrl+Shift+D"));
    setDrawModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setDrawModeAct, SIGNAL(triggered()), this, SLOT(setDrawModeAllFrames()));

    setConstrainToPointModeAct = new QAction/*(tr*/("Constraint to Point",this);
    setConstrainToPointModeAct->setShortcut(QKeySequence("Ctrl+Shift+P"));
    setConstrainToPointModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setConstrainToPointModeAct, SIGNAL(triggered()), this, SLOT(setConstrainToPointModeAllFrames()));

    setConstrainToLineModeAct = new QAction/*(tr*/("Constraint to Line",this);
    setConstrainToLineModeAct->setShortcut(QKeySequence("Ctrl+Shift+L"));
    setConstrainToLineModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setConstrainToLineModeAct, SIGNAL(triggered()), this, SLOT(setConstrainToLineModeAllFrames()));

    setScaleModeAct = new QAction/*(tr*/("Scale Mode",this);
    setScaleModeAct->setShortcut(QKeySequence("Alt+S"));
    setScaleModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setScaleModeAct, SIGNAL(triggered()), this, SLOT(setScaleModeAllFrames()));

    setSelectionModeAct = new QAction/*(tr*/("Select Mode",this);
    setScaleModeAct->setShortcut(QKeySequence("Alt+A"));
    setScaleModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setScaleModeAct, SIGNAL(triggered()), this, SLOT(setScaleModeAllFrames()));


    copySelectedItemsAct = new QAction/*(tr*/("Copy Selected Items",this);
    copySelectedItemsAct->setShortcut(QKeySequence("Ctrl+C"));
    copySelectedItemsAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(copySelectedItemsAct, SIGNAL(triggered()), this, SLOT(copySelectedItems()));


    setSelectedItemsPasteModeAct = new QAction/*(tr*/("Paste copied items",this);
    setSelectedItemsPasteModeAct->setShortcut(QKeySequence("Ctrl+V"));
    setSelectedItemsPasteModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setSelectedItemsPasteModeAct, SIGNAL(triggered()), this, SLOT(setConstrainToLineModeAllFrames()));

    setSelectedItemsDuplicateModeAct = new QAction/*(tr*/("Duplicate selected items",this);
    setSelectedItemsDuplicateModeAct->setShortcut(QKeySequence("Ctrl+D"));
    setSelectedItemsDuplicateModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setSelectedItemsDuplicateModeAct, SIGNAL(triggered()), this, SLOT(setSelectedItemsPasteModeAllFrames()));

    deleteSelectedItemsAct = new QAction/*(tr*/("Delete selected items",this);
    deleteSelectedItemsAct->setShortcut(QKeySequence("Del"));
    deleteSelectedItemsAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(deleteSelectedItemsAct, SIGNAL(triggered()), this, SLOT(deleteSelectedItemsAllFrames()));

    setRotateModeAct = new QAction/*(tr*/("Rotate selected items",this);
    setRotateModeAct->setShortcut(QKeySequence("Ctrl+R"));
    setRotateModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setRotateModeAct, SIGNAL(triggered()), this, SLOT(setConstrainToLineModeAllFrames()));

    setTranslateModeAct = new QAction/*(tr*/("Translate selected items",this);
    setTranslateModeAct->setShortcut(QKeySequence("Ctrl+T"));
    setTranslateModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setTranslateModeAct, SIGNAL(triggered()), this, SLOT(setRotateModeAllFrames()));

    setRectangleDrawModeAct = new QAction/*(tr*/("Draw rectangle",this);
    setRectangleDrawModeAct->setShortcut(QKeySequence("Alt+R"));
    setRectangleDrawModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setRectangleDrawModeAct, SIGNAL(triggered()), this, SLOT(setRectangleDrawModeAllFrames()));

    setEllipseDrawModeAct = new QAction/*(tr*/("Draw ellipse",this);
    setEllipseDrawModeAct->setShortcut(QKeySequence("Alt+E"));
    setEllipseDrawModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setEllipseDrawModeAct, SIGNAL(triggered()), this, SLOT(setEllipseDrawModeAllFrames()));

    setCurveDrawModeAct = new QAction/*(tr*/("Draw curve",this);
    setCurveDrawModeAct->setShortcut(QKeySequence("Ctrl+C+L"));
    setCurveDrawModeAct->setStatusTip(/*tr*/("Save sesseion"));
    connect(setCurveDrawModeAct, SIGNAL(triggered()), this, SLOT(setCurveDrawModeAllFrames()));


    setEllipseDrawModeAct;



}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(/*tr*/("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu = menuBar()->addMenu(/*tr*/("&Edit"));

    fileMenu->addAction(setDrawModeAct);
    fileMenu->addAction(setModifyModeAct);
    fileMenu->addAction(setConstrainToPointModeAct);
    fileMenu->addAction(setConstrainToLineModeAct);
    fileMenu = menuBar()->addMenu(/*tr*/("&Tools"));
    fileMenu->addAction(setCurveDrawModeAct);
    fileMenu->addAction(setRectangleDrawModeAct);
    fileMenu->addAction(setEllipseDrawModeAct);

    fileMenu->addAction(setSelectionModeAct);
    fileMenu->addAction(copySelectedItemsAct);
    fileMenu->addAction(setSelectedItemsPasteModeAct);
    fileMenu->addAction(setSelectedItemsDuplicateModeAct);
    fileMenu->addAction(deleteSelectedItemsAct);
    fileMenu->addAction(setRotateModeAct);
    fileMenu->addAction(setTranslateModeAct);
    fileMenu->addAction(setScaleModeAct);

}
