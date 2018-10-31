#ifndef ELLIPSECONTROLLER_H
#define ELLIPSECONTROLLER_H
#include "FigureController.h"


class EllipseController : public FigureController
{
public:
    EllipseController();
    void mousePressEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
    void mouseMoveEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
    void mouseReleaseEvent(QPainter* painter,QPoint *start, QPoint *end, QPixmap*pix);
    void drawQuarterEllipse(QPainter* painter,int x0, int y0, int x, int y);
    void MyDrawEllipse(QPainter* painter,QPoint &start,QPoint &end);
};

#endif // ELLIPSECONTROLLER_H
