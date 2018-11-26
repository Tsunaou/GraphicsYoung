#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "Figure.h"

class Ellipse : public Figure
{
public:
    Ellipse();
    Ellipse(QPoint* start,QPoint* end);
    void setStartPoint(Point p);
    void setEndPoint(Point p);
    void setRotatePoint();
};

#endif // ELLIPSE_H
