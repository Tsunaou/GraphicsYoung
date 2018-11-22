#ifndef FIGURECONTROLLER_H
#define FIGURECONTROLLER_H

#include <QPoint>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <cmath>

class FigureController
{
public:
    FigureController();
    virtual void mousePressEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix) =0;
    virtual void mouseMoveEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix) =0;
    virtual void mouseReleaseEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix) =0;
    double getLength(QPoint &start,QPoint &end);
    void drawOutlineToDebug(QPainter* painter,QPoint &start,QPoint &end);

    QPainter *painter;
    QPen pen;
};

#endif // FIGURECONTROLLER_H
