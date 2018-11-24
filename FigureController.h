#ifndef FIGURECONTROLLER_H
#define FIGURECONTROLLER_H

#include <QPoint>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <cmath>
#include <QtMath>
#include <QMouseEvent>
#include <QPen>
#include <QVector>

class FigureController
{
public:
    FigureController();
    virtual void mousePressEvent(QPainter* painter, QMouseEvent *e, QPen pen) =0;
    virtual void mouseMoveEvent(QPainter* painter, QMouseEvent *e, QPen pen) =0;
    virtual void mouseReleaseEvent(QPainter* painter,QMouseEvent *e,  QPen pen) =0;
    double getLength(QPoint &start,QPoint &end);
    void drawOutlineToDebug(QPainter* painter,QPoint &start,QPoint &end);

    QPainter *painter;
    QPen pen;
};

#endif // FIGURECONTROLLER_H
