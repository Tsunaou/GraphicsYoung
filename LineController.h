#ifndef LINECONTROLLER_H
#define LINECONTROLLER_H
#include "FigureController.h"


class LineController : public FigureController
{
public:
    LineController();
    void mousePressEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
    void mouseMoveEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
    void mouseReleaseEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
};

#endif // LINECONTROLLER_H
