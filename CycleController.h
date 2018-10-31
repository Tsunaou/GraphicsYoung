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
    void MyDrawCycle(QPainter* painter,QPoint &start,QPoint &end);
    void drawEighthCycle(QPainter* painter,int x0, int y0, int x, int y);
};

#endif // CYCLECONTROLLER_H
