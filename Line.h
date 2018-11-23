#ifndef LINE_H
#define LINE_H
#include "Figure.h"

class Line:public Figure
{
public:
    Line();
    Line(QPoint*start,QPoint*end,FIGURE_TYPE type);
    void setStartPoint(Point p);
    void setEndPoint(Point p);
    Point startPoint;   //起始点
    Point endPoint;     //终点
    Point centerPoint;  //中点
    Point rotatePoint;  //旋转点
};

#endif // LINE_H
