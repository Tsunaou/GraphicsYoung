#include "Curve.h"

Curve::Curve()
{
    vertex.clear();
}

void Curve::setNextPoint(Point p)
{
    this->vertex.push_back(p);
}
