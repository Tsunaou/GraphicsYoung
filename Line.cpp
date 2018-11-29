#include "Line.h"

Line::Line()
{
    this->start = NULL;
    this->end = NULL;
    this->type = LINE;
    this->length = 0;
}

Line::Line(QPoint *start, QPoint *end, FIGURE_TYPE type)
{
    this->start = start;
    this->end = end;
    this->type = LINE;
    this->length = 0;
}


void Line::setStartPoint(Point p)
{
    this->startPoint = p;
    int newCx = (startPoint.x+endPoint.x)/2;
    int newCy = (startPoint.y+endPoint.y)/2;
    Point cp(newCx,newCy);
    this->centerPoint = cp;
    //接下来处理旋转点
    int newPx = (newCx+endPoint.x)/2;
    int newPy = (newCy+endPoint.y)/2;
    Point rp(newPx,newPy);
    this->rotatePoint =rp;
    //setLength();
}

void Line::setEndPoint(Point p)
{
    this->endPoint = p;
    int newCx = (startPoint.x+endPoint.x)/2;
    int newCy = (startPoint.y+endPoint.y)/2;
    Point cp(newCx,newCy);
    this->centerPoint = cp;
    //接下来处理旋转点
    int newPx = (newCx+endPoint.x)/2;
    int newPy = (newCy+endPoint.y)/2;
    Point rp(newPx,newPy);
    this->rotatePoint =rp;
    //setLength();
}

void Line::setLength()
{
    this->length = startPoint.distanceToPoint(endPoint.getQPoint());
}

double Line::getLength()
{
//    if(fabs(this->length)<1e-6){
//        this->length = startPoint.distanceToPoint(endPoint.getQPoint());
//    }else{
//        this->length = (this->length+(startPoint.distanceToPoint(endPoint.getQPoint())))/2;
//    }
    this->length = startPoint.distanceToPoint(endPoint.getQPoint());
    return  this->length;
}
