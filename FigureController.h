#ifndef FIGURECONTROLLER_H
#define FIGURECONTROLLER_H

#include <QPoint>
#include <QPainter>
#include <QDebug>
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
};

#endif // FIGURECONTROLLER_H
