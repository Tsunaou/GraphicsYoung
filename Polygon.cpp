#include "Polygon.h"

Polygon::Polygon()
{
    this->start = NULL;
    this->end = NULL;
    this->type = POLYGON;
    this->colseFlag = false;
}

Polygon::Polygon(QPoint *start, QPoint *end, FIGURE_TYPE type)
{
    this->start = start;
    this->end = end;
    this->type = POLYGON;
    this->vertex.push_back(Point(start->x(),start->y()));   //起始点进去
    this->colseFlag = false;

    //对于起始点的处理
    this->startPoint = Point(*start);
}

void Polygon::setNextPoint(Point p)
{
    vertex.push_back(p);
}

void Polygon::changeNextPoint(Point p)
{
    vertex[vertex.size()-1] = p;
}
