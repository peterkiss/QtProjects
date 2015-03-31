#include "blendersplitter.h"



BlenderSplitter::BlenderSplitter(QWidget *parent) : QSplitter(parent),root(true)
{

}
/**
 * @brief BlenderSplitter::BlenderSplitter
 * @param parent tartalmazo widget
 * @param PrimaryWidget, SecondaryWidget splitter ket reszenek widgetei
 *
 *
 */
BlenderSplitter::BlenderSplitter(BlenderSplitter *parent,WorkFrame* PrimaryWidget,WorkFrame* SecondaryWidget) :
    QSplitter(parent),root(false) //új splitter frame helyett
{

    this->primaryWidget=PrimaryWidget;
    this->secondaryWidget=SecondaryWidget;
    setOrientation(parent->orientation()==Qt::Vertical?Qt::Horizontal:Qt::Vertical);
    QList<int > sizes;
    if(orientation()==Qt::Vertical){
        QSplitter::addWidget(secondaryWidget);
        QSplitter::addWidget(primaryWidget);
        sizes.append(0);
        sizes.append(height());

    }
    else{
        QSplitter::addWidget(primaryWidget);
        QSplitter::addWidget(secondaryWidget);
        sizes.append(width());
        sizes.append(0);

    }
    setSizes(sizes);
    setFrameRect(((WorkFrame*)primaryWidget)->frameRect());
    primaryWidget->show();
    ((WorkFrame*)primaryWidget)->moveSplitterAfterReorg = true;
    ((WorkFrame*)primaryWidget)->reOrg=false;

}
/**
 * @brief BlenderSplitter::reOrganize
 * nem mukodo funkcio
 *
 *
 */
void BlenderSplitter::reOrganize(BlenderSplitter* splitter, WorkFrame* frame)
{
    int index = indexOf(splitter);
    splitter->hide();
    delete splitter;
    insertWidget(index,frame);

}
/**
 * @brief BlenderSplitter::moveSplitter
 * @param pos elmozdulas iranya
 * @param index dummy
 * atrendezes utan felosztas valtoztatasa
 *
 */
void BlenderSplitter::moveSplitter(QPointF pos, int index)
{
    if(root && count()==1){

        if(abs(pos.x())>abs(pos.y()))
            setOrientation(Qt::Horizontal);
        else
            setOrientation(Qt::Vertical);
        widget(1)->hide();
    }
    if(primaryWidget->isHidden())
        primaryWidget->show();
    if(secondaryWidget->isHidden())
        secondaryWidget->show();
    QList<int > sizes;
    if(orientation() == Qt::Horizontal){
        sizes.append(width()-pos.x()-handleWidth()/2);
        sizes.append(pos.x()-handleWidth()/2);

    }
    else{
        sizes.append(-pos.y()-handleWidth()/2);
        sizes.append(height()+pos.y()-handleWidth()/2);

    }
    setSizes(sizes);
    repaint();

}
/**
 * @brief BlenderSplitter::reOrganize
 * @param dif elmozdulas iranya
 * @param frame eredeti workFrame
 * frame lecserelese splitterre,frame-mel es masolataval feltoltve
 *
 */
void BlenderSplitter::reOrganize(QPointF dif, WorkFrame* frame)
{

    if(root){
        if(abs(dif.y())<abs(dif.x()))
            setOrientation(Qt::Vertical);
        else
            setOrientation(Qt::Horizontal);
    }
    QList<int> size =sizes();
    frame->hide();
    int ind = indexOf(frame);
    WorkFrame* newWf =  new WorkFrame(*frame);

    BlenderSplitter* newSpl=  new BlenderSplitter(this,frame,newWf);
    insertWidget(ind,(QWidget*)newSpl);
    setSizes(size);

}
/**
 * @brief BlenderSplitter::reOrganize
 * @param widget elso widget root splitterben
 * elso widget hozzaadasa splitterhez
 *
 */
void BlenderSplitter::addWidget(QWidget *widget)
{
    if(!root)
        return;
    QSplitter::addWidget(widget);
    primaryWidget=(WorkFrame*)widget;

}




BlenderSplitter::~BlenderSplitter()
{
    //    if(secondaryWidget)
    //        delete secondaryWidget;
}
