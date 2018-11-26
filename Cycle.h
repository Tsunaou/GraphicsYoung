#ifndef CYCLE_H
#define CYCLE_H
#include "Figure.h"

class Cycle : public Figure
{
public:
    Cycle();
    Cycle(QPoint*start,QPoint*end,FIGURE_TYPE type);
    void setStartPoint(Point p);
    void setEndPoint(Point p);
    double getRadius();
private:
    qreal Ridius;
};

#endif // CYCLE_H
