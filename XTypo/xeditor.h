#ifndef XEDITOR_H
#define XEDITOR_H

#include <QFrame>
#include <QTextEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QComboBox>
class WorkFrame;
class XEditor : public QFrame
{
    Q_OBJECT
public:
    explicit XEditor(WorkFrame* parentFrame = NULL, QWidget *parent = 0);
    WorkFrame* containingFrame;
    QTextEdit* editBox;
    QGraphicsView* graphicsView;
    QComboBox* FrameModeCombo;
    QGraphicsScene* scene;
    QColor* fillColor;
    int charWidth = 30;

signals:

public slots:
    void updateTextImage();
    void FrameModeChange();

};

#endif // XEDITOR_H
