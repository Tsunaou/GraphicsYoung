#include "Line.h"

Line::Line()
{
    this->start = NULL;
    this->end = NULL;
    this->type = LINE;
}

Line::Line(QPoint *start, QPoint *end, FIGURE_TYPE type)
{
    this->start = start;
    this->end = end;
    this->type = LINE;
}

void Line::setStartPoint(Point p)
{
    this->startPoint = p;
    int newCx = (startPoint.x+endPoint.x)/2;
    int newCy = (startPoint.y+endPoint.y)/2;
    Point cp(newCx,newCy);
    this->centerPoint = cp;
    //this->centerPoint.setPoint(newCx,newCy);
}

void Line::setEndPoint(Point p)
{
    this->endPoint = p;
    int newCx = (startPoint.x+endPoint.x)/2;
    int newCy = (startPoint.y+endPoint.y)/2;
    Point cp(newCx,newCy);
    this->centerPoint = cp;
    //this->centerPoint.setPoint(newCx,newCy);
}
