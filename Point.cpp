#include "Point.h"

Point::Point()
{
    x = 0;
    y = 0;
    ridus = 5;
    point.setX(x);
    point.setY(y);
}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
    ridus = 6;
    point.setX(x);
    point.setY(y);
}

Point::Point(QPoint p)
{
    this->x = p.x();
    this->y = p.y();
    ridus = 6;
    this->point = p;
}

void Point::setPoint(int x, int y)
{
    this->point.setX(x);
    this->point.setY(y);
    this->x = x;
    this->y = y;
}

void Point::DrawCyclePoint(QPainter *painter,QPen &pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    QPen debugPen;
    debugPen.setWidth(1);
    debugPen.setColor(Qt::blue);
    painter->setPen(debugPen);

    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    for(int i=x-ridus;i<x+ridus;i++){
        for(int j=y-ridus;j<y+ridus;j++){
            if(distanceToPoint(i,j)<=ridus){
                painter->drawPoint(i,j);
            }
        }
    }

    painter->setPen(pen);


}

void Point::DrawWarnPoint(QPainter *painter, QPen &pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    QPen debugPen;
    debugPen.setWidth(1);
    debugPen.setColor(Qt::red);
    painter->setPen(debugPen);

    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    for(int i=x-ridus;i<x+ridus;i++){
        for(int j=y-ridus;j<y+ridus;j++){
            if(distanceToPoint(i,j)<=ridus){
                painter->drawPoint(i,j);
            }
        }
    }

    painter->setPen(pen);
}

void Point::DrawMovingPoint(QPainter *painter, QPen &pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    QPen debugPen;
    debugPen.setWidth(1);
    debugPen.setColor(Qt::gray);
    painter->setPen(debugPen);

    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    int ridusReg = ridus;
    ridus--;

    for(int i=x-ridus;i<x+ridus;i++){
        for(int j=y-ridus;j<y+ridus;j++){
            if(distanceToPoint(i,j)<=ridus){
                painter->drawPoint(i,j);
            }
        }
    }

    ridus = ridusReg;

    painter->setPen(pen);
}

void Point::DrawColorPoint(QPainter *painter, QPen &pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    QPen debugPen;
    debugPen.setWidth(1);
    debugPen.setColor(pen.color());
    painter->setPen(debugPen);

    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行


    for(int i=x-ridus;i<x+ridus;i++){
        for(int j=y-ridus;j<y+ridus;j++){
            if(distanceToPoint(i,j)<=ridus){
                painter->drawPoint(i,j);
            }
        }
    }


    painter->setPen(pen);
}

double Point::distanceToPoint(int x, int y)
{
    return sqrt((this->x-x)*
                (this->x-x)+
                (this->y-y)*
                (this->y-y));
}

double Point::distanceToPoint(QPoint p)
{
    return distanceToPoint(p.x(),p.y());
}

Point Point::operator*(const double size)
{
    int xn = this->x * size;
    int yn = this->y * size;
    return Point(xn,yn);
}




