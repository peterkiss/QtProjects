#ifndef XTRANSFORM_H
#define XTRANSFORM_H

#include <QTransform>
#include <QMatrix>

class XTransform
{
public:
    XTransform();
    static QTransform* reflectX(QPointF * pt1);
    static QTransform* reflectY(QPointF * pt1);
    static QTransform* centeredMirroring(QPointF * cpt);
    static QTransform* rotate(QPointF * cpt, double angle);
    static QTransform* move(QVector2D* v);
    static QTransform* scale(QPointF *cpt, double scale);
    static bool isValidCoordinate(qreal value);
};

#endif // XTRANSFORM_H
