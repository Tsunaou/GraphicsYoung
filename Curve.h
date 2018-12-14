#ifndef CURVE_H
#define CURVE_H
#include "Figure.h"

class Curve : public Figure
{
public:
    Curve();
    void setNextPoint(Point p);
    QVector<Point> vertex;  //存储端点

};

#endif // CURVE_H
