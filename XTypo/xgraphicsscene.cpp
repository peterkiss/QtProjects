#include "xgraphicsscene.h"
#include "xgrphicspixmapitem.h"
#include "beziercurve.h"
#include "drawarea.h"
#include <QDebug>

// szerkesztési segédvonalak egyelőre statikusan
void XGraphicsScene::createBaseLines()
{
    this->addLine(0,0,180,0,QPen(Qt::blue));
    this->addLine(0,20,180,20,QPen(Qt::blue));
    this->addLine(0,100,180,100,QPen(Qt::blue));
    this->addLine(0,200,180,200,QPen(Qt::green));
    this->addLine(0,220,180,220,QPen(Qt::blue));
    this->addLine(0,280,180,280,QPen(Qt::blue));
    this->addLine(0,300,180,300,QPen(Qt::blue));

    this->addLine(0,0,0,300,QPen(Qt::blue));
    this->addLine(20,0,20,300,QPen(Qt::blue));
    this->addLine(90,0,90,300,QPen(Qt::green));
    this->addLine(160,0,160,300,QPen(Qt::blue));
    this->addLine(180,0,180,300,QPen(Qt::blue));
}

XGraphicsScene::XGraphicsScene(QObject* parent, char associatedChar,QList<XLayer*>* otherslayers) : QGraphicsScene(parent),scaleFactor(1)
{

    this->associatedChar=associatedChar;
    lineColor = NULL;
    fillColor = NULL;
    detailedMode = false;
    this->lineColor = new QColor(Qt::black);
    this->fillColor = new QColor(Qt::gray);
    createBaseLines();
    if(!otherslayers)
        addLayer();
    colors.append(new XColorGraphicsItem(this));
    addItem(colors.at(0));
}

QList<QLineF*>*  XGraphicsScene::getConstraintLines(QPointF pos)
{
    QList<QLineF*>* cLines=new QList<QLineF*>;
    QListIterator<BezierCurve*> loopIterator(this->StartCurves);
    while(loopIterator.hasNext())
    {
        BezierCurve* firstCurve=loopIterator.next();
        if(firstCurve->editing) continue;
        cLines->append(*firstCurve->getHorizontalConstraintLines(pos));
        cLines->append(*firstCurve->getVerticalConstraintLines(pos));
        BezierCurve* act=firstCurve->getNextCurve();

        while(act!=firstCurve)
        {
            if(!act || act->editing) break;
            cLines->append(*act->getHorizontalConstraintLines(pos));
            cLines->append(*act->getVerticalConstraintLines(pos));
            act=act->getNextCurve();
        }

    }
    return cLines;
}

void XGraphicsScene::addItem(QGraphicsItem *item)
{
    //    if(((XGraphicsItem*)item)->isBezierCurve())
    //        colors.at(0)->addItem((XGraphicsItem*)item);

    QGraphicsScene::addItem(item);
    //    saveState();
}

void XGraphicsScene::addPixmap(QPixmap *pm)
{    
    QGraphicsScene::addPixmap(*pm);
}

void XGraphicsScene::undo()
{
    redoStack.push(this->toString());
    clear();
    this->fromString(undoStack.pop()/*.split("\n")*/);
    QListIterator<WorkFrame* > frameIt(this->frames);
    while(frameIt.hasNext())
        frameIt.next()->updateFromUndoStack();
}

void XGraphicsScene::redo()
{
    undoStack.push(this->toString());
    clear();
    this->fromString(redoStack.pop()/*.split("\n")*/);
    QListIterator<WorkFrame* > frameIt(this->frames);
    while(frameIt.hasNext())
        frameIt.next()->updateFromUndoStack();
}

void XGraphicsScene::clear()
{
    //    qDeleteAll(this->ellipses);
    //    qDeleteAll(this->rectangles);
    qDeleteAll(this->layers);
    //    qDeleteAll(this->pixmaps);
    qDeleteAll(this->items());
    this->items().clear();
    this->layers.clear();

    this->selectedCurves.clear();
    this->selectedPixmaps.clear();
    this->selectedPoints.clear();

    this->StartCurves.clear();
    this->ellipses.clear();
    this->rectangles.clear();
    this->pixmaps.clear();
    this->colors.clear();

    delete fillColor;
    delete lineColor;
}

bool XGraphicsScene::undoStackIsEmpty()
{
    return undoStack.isEmpty();
}

bool XGraphicsScene::redoStackIsEmpty()
{
    return redoStack.isEmpty();
}

void XGraphicsScene::saveState()
{
    undoStack.push(this->toString());
    redoStack.clear();

    QListIterator<WorkFrame* > frameIt(this->frames);
    while(frameIt.hasNext())
        frameIt.next()->updateFromUndoStack();
}

XLayer *XGraphicsScene::addLayer(QString name )
{
    //saveState();
    XLayer* newLayer = new XLayer(this);
    layers.append(newLayer);
    actLayer = newLayer;
    QListIterator<WorkFrame* > frameIt(this->frames);
    while(frameIt.hasNext())
        frameIt.next()->updateLayersFromScene(/*true*/);
    return newLayer;
}

void XGraphicsScene::removeLayer(XLayer *layerToRemove)
{
    this->layers.removeAll(layerToRemove);
}

void XGraphicsScene::setLineColor(const QColor *newColor){
    if(lineColor)
        delete lineColor;
    lineColor=new QColor(*newColor);
    QListIterator<WorkFrame* > frameIt(this->frames);
    while(frameIt.hasNext())
        frameIt.next()->updateColorsFromScene();
}

void XGraphicsScene::setFillColor(const QColor *newColor){
    if(fillColor)
        delete fillColor;
    fillColor=new QColor(*newColor);
    QListIterator<WorkFrame* > frameIt(this->frames);
    while(frameIt.hasNext())
        frameIt.next()->updateColorsFromScene();}

void XGraphicsScene::addShowingWorkFrame(WorkFrame *f)
{
    this->frames.append(f);
}

void XGraphicsScene::removeShowingWorkFrame(WorkFrame *f)
{
    this->frames.removeAll(f);
}

QPointF *XGraphicsScene::getMiddlePointOfSelectedShape()
{
    QPointF* mp= new QPointF(0,0);
    QListIterator<QPointF*> it(this->selectedPoints);
    while(it.hasNext())
        *mp+=*it.next();
    QListIterator<XGraphicsPixmapItem*> pit(this->selectedPixmaps);
    while(pit.hasNext())
        *mp+=*(pit.next())->getMiddlePoint();
    *mp/=this->selectedPoints.count()+this->selectedPixmaps.count();
    return mp;

}
// a beillesztes koordinataihoz a kijelolt pontok ettol valo tavolsagat adjuk hozza
QPointF *XGraphicsScene::getTopLeftPointOfSelectedShape()
{
    QPointF* pt = new QPointF(9999,9999);
    QListIterator<BezierCurve*> bci(this->selectedCurves);
    while(bci.hasNext())
    {
        BezierCurve* act = bci.next();
        QListIterator<QPointF> pi(*act->getControlPoints());
        while (pi.hasNext()) {
            QPointF actp =pi.next();
            if(actp.x()<pt->x())
                pt->setX(actp.x());
            if(actp.y()<pt->y())
                pt->setY(actp.y());
        }

    }
    QListIterator<XGraphicsPixmapItem*> pxi(this->selectedPixmaps);
    while(pxi.hasNext()){
        XGraphicsPixmapItem* actpx = pxi.next();
        if(actpx->pos().x()<pt->x())
            pt->setX(actpx->pos().x());
        if(actpx->pos().y()<pt->y())
            pt->setY(actpx->pos().y());
    }
    return pt;

}

void XGraphicsScene::paste(QPointF *offset, QList<BezierCurve*>* curves,QList<XGraphicsPixmapItem*>* pixmaps)
{
    QList<QPair<BezierCurve*,BezierCurve*> > list;
    QListIterator<BezierCurve*>  ci(*curves);
    while(ci.hasNext()){
        BezierCurve* actOriginal = ci.next();
        BezierCurve* copy = new BezierCurve(offset,actOriginal,this->actLayer,this);
        addItem(copy);
        list.append(qMakePair(actOriginal,copy));
    }
    // összeláncolás
    QListIterator<QPair<BezierCurve*,BezierCurve*> >  pairIterator(list);
    while(pairIterator.hasNext())
    {
        QPair<BezierCurve*,BezierCurve*> actPair = pairIterator.next();
        QListIterator<QPair<BezierCurve*,BezierCurve*> >  findConnectionIterator(list);
        while(findConnectionIterator.hasNext())
        {
            QPair<BezierCurve*,BezierCurve*> actConnPair = findConnectionIterator.next();
            if(actPair.first->getPreviousCurve() == actConnPair.first)
                actPair.second->SetPreviousCurve(actConnPair.second);
            if(actPair.first->getNextCurve() == actConnPair.first)
                actPair.second->SetNextCurve(actConnPair.second);

        }


    }
    //színezéshez
    pairIterator.toFront();
    while(pairIterator.hasNext())
    {
        QPair<BezierCurve*,BezierCurve*> actPair = pairIterator.next();
        if(!actPair.second->getPreviousCurve() || actPair.second->isFirstCurve)
            this->StartCurves.append(actPair.second);
    }

    QListIterator<XGraphicsPixmapItem*> pixmapIterator(*pixmaps);
    while(pixmapIterator.hasNext())
    {
        XGraphicsPixmapItem* actOrig = pixmapIterator.next();
        addItem(new XGraphicsPixmapItem(this,this->actLayer,actOrig,offset));
    }



}

void XGraphicsScene::duplicate(QList<BezierCurve *> *curves, QList<XGraphicsPixmapItem *> *pixmaps)
{
    QPointF* offset = new QPointF(0,0);
    paste(offset,curves,pixmaps);
}

void XGraphicsScene::deleteItems(QList<BezierCurve *> *curves, QList<XGraphicsPixmapItem *> *pixmaps)
{
    saveState();
    QListIterator<BezierCurve *> bci(*curves);
    while(bci.hasNext())
    {
        BezierCurve *act = bci.next();
        this->removeItem(act);
        if(act->isFirstCurve)
            this->StartCurves.removeAll(act);
        if(act->getNextCurve() && this->items().contains(act->getNextCurve())){
            act->getNextCurve()->isFirstCurve = true;
            act->getNextCurve()->setPreviousCurve(NULL);
            this->StartCurves.append(act->getNextCurve());
        }
        if(act->getPreviousCurve())
            act->getPreviousCurve()->setNextCurve(NULL);

    }
    QListIterator<XGraphicsPixmapItem *> pmi(*pixmaps);
    while(pmi.hasNext())
    {
        this->removeItem(pmi.next());
    }
    qDeleteAll(*curves);
    qDeleteAll(*pixmaps);
    this->selectedCurves.clear();
    this->selectedPixmaps.clear();
    curves = NULL;
    pixmaps = NULL;


}

void XGraphicsScene::clearShapeSelection()
{
    QListIterator<BezierCurve*> sci(this->selectedCurves);
    while(sci.hasNext())
        ((BezierCurve*)sci.next())->deSelectThisShape();
    selectedPoints.clear();
    selectedCurves.clear();
    QListIterator<XGraphicsPixmapItem*> pi(this->selectedPixmaps);
    while(pi.hasNext())
        ((XGraphicsPixmapItem*)pi.next())->setSelected(false);
    selectedCurves.clear();
}

void XGraphicsScene::addPixmpItem(QString fileName)
{
    QPixmap test(fileName);
    if(test.isNull())
        return;
    saveState();
    XGraphicsPixmapItem* pixmap = new XGraphicsPixmapItem(this,this->getActualLayer(),fileName);
    pixmaps.append(pixmap);
    addItem(pixmap);
}

QString XGraphicsScene::getAssociatedCharAsString()
{
    QByteArray array;
    array.append(getAssociatedChar());
    return QString::fromLocal8Bit(array);
}

QStringList XGraphicsScene::toString()
{
    //kell: színek, layerek száma, nevük, görbék
    QStringList out;
    QChar associatedChar(this->associatedChar);
    QString chStr; chStr.append(associatedChar);
    out.append(chStr);

    out.append(QString::number(this->layers.count())/* + QString("\n")*/);
    QListIterator<XLayer*> layerIt(this->layers);
    while(layerIt.hasNext())
        out.append(layerIt.next()->getName()/*+"\n"*/);
    out.append(QString::number(this->fillColor->red())+" "+ QString::number(this->fillColor->green())+" "+QString::number(this->fillColor->blue()));
    out.append(QString::number(this->lineColor->red())+" " + QString::number(this->lineColor->green())+" " +QString::number(this->lineColor->blue()));
    out.append(QString::number(this->StartCurves.count())/* +  QString("\n")*/)  ;
    QListIterator<BezierCurve*> startCurveIterator(this->StartCurves);
    while(startCurveIterator.hasNext()){
        int curveInLoopCounter = 1;
        BezierCurve* firsCurveOfLoop = startCurveIterator.next();
        BezierCurve* actCurve = firsCurveOfLoop;
        QString curvesData;
        QListIterator<QPointF > pointIT(*actCurve->getControlPoints());
        while(pointIT.hasNext()){
            QPointF actPoint = pointIT.next();
            curvesData.append(QString::number((int)actPoint.x()));
            curvesData.append(" ");
            curvesData.append(QString::number((int)actPoint.y()));
            curvesData.append(" ");
        }
        while(actCurve->getNextCurve() && actCurve->getNextCurve()!=firsCurveOfLoop){
            actCurve = actCurve->getNextCurve();
            curveInLoopCounter++;
            QListIterator<QPointF > pointIT(*actCurve->getControlPoints());
            while(pointIT.hasNext()){
                QPointF actPoint = pointIT.next();
                curvesData.append(QString::number((int)actPoint.x()));
                curvesData.append(" ");
                curvesData.append(QString::number((int)actPoint.y()));
                curvesData.append(" ");
            }
            //actCurve = actCurve->getNextCurve();
        }
        bool isCurveClosed = actCurve->getNextCurve() == firsCurveOfLoop;
        curvesData.prepend(QString::number(this->layers.lastIndexOf(firsCurveOfLoop->getLayer()))+" ");
        curvesData.prepend(QString::number(isCurveClosed)+" ");
        curvesData.prepend(QString::number(curveInLoopCounter)+" ");
        qDebug()<<"write: "<<curvesData;
        out.append(curvesData);

    }
    out.append(QString::number(this->pixmaps.count())/* + QString("\n")*/)  ;
    QListIterator<XGraphicsPixmapItem*> pixmapIterator(this->pixmaps);
    while(pixmapIterator.hasNext()){
        XGraphicsPixmapItem* actPixmap = pixmapIterator.next();
        QString pixmapData =actPixmap->getImagePath() +"#"+QString::number(this->layers.lastIndexOf(actPixmap->getLayer()))/*+"#"+ QString::number(actPixmap->getGeometry()->topLeft().x())*/
                +"#"+ QString::number((int)actPixmap->getGeometry()->topLeft().x())
                +"#"+ QString::number((int)actPixmap->getGeometry()->topLeft().y())
                +"#"+ QString::number((int)actPixmap->getGeometry()->width())
                +"#"+ QString::number((int)actPixmap->getGeometry()->height());
        qDebug()<<"pixmapData: "<<pixmapData;
        out.append(pixmapData);

    }
    out.append(QString::number(this->colors.count()))  ;
    QListIterator<XColorGraphicsItem*> colorIterator(this->colors);
    while(colorIterator.hasNext()){
        colorIterator.next(); //egyelőre csak egy van, ami mindent színez

    }

    out.append( QString("#") /*+ QString("\n")*/);
    return out;

}

void XGraphicsScene::fromString(QStringList sceneData)
{
    createBaseLines();
    int ptr = 0;
    QStringListIterator dataIterator(sceneData);
    this->associatedChar = dataIterator.next().toLocal8Bit()[0];
    int layerCount = dataIterator.next().toInt();
    for(int i = 0;i<layerCount;++i)
        this->addLayer(dataIterator.next());

    QStringList fillColorStringList = dataIterator.next().split(" ");
    this->fillColor = new QColor(fillColorStringList.at(0).toInt(),fillColorStringList.at(1).toInt(),fillColorStringList.at(2).toInt());
    QStringList lineColorStringList = dataIterator.next().split(" ");
    this->lineColor = new QColor(lineColorStringList.at(0).toInt(),lineColorStringList.at(1).toInt(),lineColorStringList.at(2).toInt());

    int loopCount = dataIterator.next().toInt();
    for(int i = 0;i<loopCount;++i)
    {
        QString LoopData = dataIterator.next();
        QStringList curveLoopDataStringList = LoopData.split(" ");
        qDebug()<<"read: "<<LoopData;
        QStringListIterator loopIterator(curveLoopDataStringList);
        int curveInLoopCount = loopIterator.next().toInt();
        bool isClosed = loopIterator.next().toInt();
        XLayer* layer = layers.at(loopIterator.next().toInt());
        BezierCurve* lastAddedCurve = NULL,* startCurve = NULL;
        for(int i = 0; i<curveInLoopCount; ++i)
        {
            QList<int> coordinates;
            for(int i =0; i<8; i++)
                coordinates.append(loopIterator.next().toInt());
            BezierCurve* actCurve = new BezierCurve(QPointF(coordinates.at(0),coordinates.at(1)),
                                                    QPointF(coordinates.at(2),coordinates.at(3)),
                                                    QPointF(coordinates.at(4),coordinates.at(5)),
                                                    QPointF(coordinates.at(6),coordinates.at(7)),
                                                    this,layer);
            addItem(actCurve);
            if(lastAddedCurve)
                actCurve->SetPreviousCurve(lastAddedCurve);
            else
                startCurve = actCurve;
            lastAddedCurve = actCurve;
        }
        if(isClosed)
            startCurve->SetPreviousCurve(lastAddedCurve);
        this->StartCurves.append(startCurve);

    }
    int pixmapCount =  dataIterator.next().toInt();
    for(int i = 0; i<pixmapCount;++i)
    {
        QStringList pixmapData= dataIterator.next().split("#");
        QStringListIterator pixmapDataIterator(pixmapData);
        QString path = pixmapDataIterator.next();
        int layerID = pixmapDataIterator.next().toInt();
        QPixmap* test;
        try
        {
            test = new QPixmap(path);
            if(test->isNull())
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText("Error");
                msgBox.setInformativeText("An error occured during reading image file. Skipped.");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        catch(...)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Error");
            msgBox.setInformativeText("An error occured during reading image file. Skipped.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
        XGraphicsPixmapItem* act = new XGraphicsPixmapItem(this,this->layers.at(layerID),path);
        QList<int> coordinates;
        for(int i =0; i<4; i++)
            coordinates.append(pixmapDataIterator.next().toInt());
        *act->getGeometry() = QRectF(coordinates.at(0),coordinates.at(1),coordinates.at(2),coordinates.at(3));
        this->addItem(act);
        pixmaps.append(act);
    }
    int colorItemCount = dataIterator.next().toInt();
    for(int i = 0; i<colorItemCount;++i)
    {
        XColorGraphicsItem* coloritem = new XColorGraphicsItem(this);
        this->colors.append(coloritem);
        this->addItem(coloritem);
    }






}
