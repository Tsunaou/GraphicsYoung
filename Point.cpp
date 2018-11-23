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

void Point::setPoint(int x, int y)
{
    this->point.setX(x);
    this->point.setY(y);
}

void Point::DrawCyclePoint(QPainter *painter,QPen &pen)
{
    QPen debugPen;
    debugPen.setWidth(1);
    debugPen.setColor(Qt::blue);
    painter->setPen(debugPen);

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
