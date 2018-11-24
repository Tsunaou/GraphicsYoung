#ifndef FIGURE_H
#define FIGURE_H

#include "constParams.h"
#include "Point.h"
#include <QPoint>

class Figure
{
public:
    Figure();
    Figure(QPoint* start,QPoint* end);
    QPoint* start;
    QPoint* end;
    FIGURE_TYPE type;
    //继承用
    Point startPoint;   //起始点
    Point endPoint;     //终点
    Point centerPoint;  //中点
    Point rotatePoint;  //旋转点
};

#endif // FIGURE_H
