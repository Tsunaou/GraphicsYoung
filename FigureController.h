#ifndef FIGURECONTROLLER_H
#define FIGURECONTROLLER_H

#include <QPoint>
#include <QPainter>

class FigureController
{
public:
    FigureController();
    virtual void mousePressEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix) =0;
    virtual void mouseMoveEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix) =0;
    virtual void mouseReleaseEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix) =0;
};

#endif // FIGURECONTROLLER_H
