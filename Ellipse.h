#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "Figure.h"

class Ellipse : public Figure
{
public:
    Ellipse();
    Ellipse(QPoint* start,QPoint* end);
};

#endif // ELLIPSE_H
