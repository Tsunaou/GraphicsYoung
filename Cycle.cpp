#include "Cycle.h"

Cycle::Cycle()
{
    this->start = NULL;
    this->end = NULL;
    this->type = CYCLE;
}

Cycle::Cycle(QPoint *start, QPoint *end, FIGURE_TYPE type)
{
    this->start = start;
    this->end = end;
    this->type = CYCLE;
}

void Cycle::setStartPoint(Point p)
{
    this->startPoint = p;   //圆心
    this->centerPoint = p;  //圆心
    //接下来处理旋转点(R/2的点)
    int newCx = (startPoint.x+endPoint.x)/2;
    int newCy = (startPoint.y+endPoint.y)/2;
    Point rp(newCx,newCy);
    this->rotatePoint =rp;
}

void Cycle::setEndPoint(Point p)
{
    this->endPoint = p;
    this->centerPoint = this->startPoint;
    //接下来处理旋转点(R/2的点)
    int newCx = (startPoint.x+endPoint.x)/2;
    int newCy = (startPoint.y+endPoint.y)/2;
    Point rp(newCx,newCy);
    this->rotatePoint =rp;
}

double Cycle::getRadius()
{
    this->Ridius = (this->Ridius+startPoint.distanceToPoint(endPoint.getQPoint()))/2;
    return this->Ridius;
}
