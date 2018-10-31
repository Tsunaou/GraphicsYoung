#ifndef CYCLECONTROLLER_H
#define CYCLECONTROLLER_H
#include "FigureController.h"

class CycleController : public FigureController
{
public:
    CycleController();
    void mousePressEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
    void mouseMoveEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
    void mouseReleaseEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
};

#endif // CYCLECONTROLLER_H
