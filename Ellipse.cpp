#include "Ellipse.h"

Ellipse::Ellipse()
{
    this->start = NULL;
    this->end = NULL;
    this->type = ELLIPSE;
}

Ellipse::Ellipse(QPoint *start, QPoint *end)
{
    this->start = start;
    this->end = end;
    this->type = ELLIPSE;
}

void Ellipse::setStartPoint(Point p)
{
    this->startPoint = p;   //圆心
    this->centerPoint = p;  //圆心
    //接下来处理旋转点(R/2的点)
    int newCx = (startPoint.x+endPoint.x)/2;
    int newCy = (startPoint.y+endPoint.y)/2;
    Point rp(newCx,newCy);
    this->rotatePoint =rp;
}

void Ellipse::setEndPoint(Point p)
{
    this->endPoint = p;
    this->centerPoint = this->startPoint;
    //接下来处理旋转点(R/2的点)
    int newCx = (startPoint.x+endPoint.x)/2;
    int newCy = (startPoint.y+endPoint.y)/2;
    Point rp(newCx,newCy);
    this->rotatePoint =rp;
}
