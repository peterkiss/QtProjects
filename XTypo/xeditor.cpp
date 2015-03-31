#include "xeditor.h"
#include <QVBoxLayout>
#include "xgraphicsscene.h"
#include "xcolorgraphicsitem.h"
#include "drawarea.h"
#include <QPainterPathStroker>
#include <QDebug>

extern QMap<char,XGraphicsScene*> charMap;
//QGraphicsScene* XEditor::scene = NULL ;
//QTextEdit* XEditor::editBox = NULL;

XEditor::XEditor(WorkFrame *parentFrame, QWidget *parent ) :
    QFrame(parent)
{

    FrameModeCombo = new QComboBox;
    FrameModeCombo->addItem("Design");
    FrameModeCombo->addItem("Try");
    FrameModeCombo->setCurrentIndex(1);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    containingFrame = parentFrame;
  //  if(!scene)
        scene = new QGraphicsScene;
  //  if(!editBox)
        editBox = new QTextEdit;
    graphicsView = new QGraphicsView;
    graphicsView->setScene(scene);
    mainLayout->addWidget(this->FrameModeCombo);
    mainLayout->addWidget(graphicsView);
    mainLayout->addWidget(editBox);

    fillColor = new QColor(Qt::black);
    connect(editBox,SIGNAL(textChanged()),this,SLOT(updateTextImage()));
     connect(FrameModeCombo,SIGNAL(currentIndexChanged (int)),this,SLOT(FrameModeChange()));
     setLayout(mainLayout);
}

void XEditor::updateTextImage()
{
    QString editText = editBox->toPlainText();
    QByteArray editBoxByteArray = editText.toLocal8Bit();
    QPen pen(Qt::black);
    pen.setWidth(1);
    QBrush brush(Qt::gray);
    qDeleteAll(scene->items());
    scene->clear();
    int ypos = 0;
    for(int i = 0;i < editBoxByteArray.count(); ++i ){
        QPainterPathStroker stroker;
        stroker.setWidth(charWidth);
        stroker.setJoinStyle(Qt::MiterJoin);

        if(charMap[editBoxByteArray.at(i)]){
           // QPainterPath newpath = (stroker.createStroke(*charMap[editBoxByteArray.at(i)]->colors.at(0)->getPath())).simplified();
            QPainterPath newpath = *charMap[editBoxByteArray.at(i)]->colors.at(0)->getPath();
            ypos +=newpath.boundingRect().width();
//            qDebug()<<"chw: "<<ypos;
//            qDebug()<<"ypos: "<<ypos;
            newpath.translate(ypos  ,0);
             this->scene->addPath(newpath,pen,brush);
        }
        else
            this->scene->addRect(i*30 +10,0,30,50,QPen(Qt::black));
    }

}

void XEditor::FrameModeChange()
{
    if(this->FrameModeCombo->currentIndex() == 0){
        this->hide();
        this->containingFrame->FrameModeCombo->setCurrentIndex(0);
        this->containingFrame->drawFrame->show();
    }
    else{
        this->show();        
        this->containingFrame->drawFrame->hide();

    }
}
