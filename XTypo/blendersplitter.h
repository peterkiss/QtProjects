#ifndef BLENDERSPLITTER_H
#define BLENDERSPLITTER_H

#include <QSplitter>
#include "drawarea.h"
#include <QDebug>

class BlenderSplitter : public QSplitter
{
    Q_OBJECT
public:
    explicit BlenderSplitter(QWidget *parent = 0);
    void reOrganize(QPointF dif, WorkFrame *frame);
    void addWidget(QWidget *widget) ;
    ~BlenderSplitter();
    const bool root;
    void reOrganize(BlenderSplitter *splitter, WorkFrame *frame);
    void moveSplitter(QPointF pos, int index);
    BlenderSplitter(BlenderSplitter *parent, WorkFrame* PrimaryWidget, WorkFrame* SecondaryWidget);
signals:

public slots:
    void mouseMoveEvent(QMouseEvent *me){if(me->buttons()==Qt::LeftButton) qDebug()<<"mouseMove";}
    void mouseReleaseEvent(QMouseEvent *){ qDebug()<<"mouseRel";}
private:
    QWidget* primaryWidget;
    QWidget* secondaryWidget;

};

#endif // BLENDERSPLITTER_H
