#include "Polygon.h"

Polygon::Polygon()
{
    this->start = NULL;
    this->end = NULL;
    this->type = POLYGON;
}

Polygon::Polygon(QPoint *start, QPoint *end, FIGURE_TYPE type)
{
    this->start = start;
    this->end = end;
    this->type = POLYGON;
    this->vertex.push_back(Point(start->x(),start->y()));   //起始点进去
}

void Polygon::setNextPoint(Point p)
{
    vertex.push_back(p);
}
