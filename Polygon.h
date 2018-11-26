#ifndef POLYGON_H
#define POLYGON_H
#include "Figure.h"
#include "Line.h"

class Polygon : public Figure
{
public:
    Polygon();
    Polygon(QPoint*start,QPoint*end,FIGURE_TYPE type); //对多边形来说，关心的是他的起点
    void setNextPoint(Point p);
    void changeNextPoint(Point p);
    QVector<Line> edges;    //存储边
    QVector<Point> vertex;  //存储端点
    //这样edges[i]的两个端点便是 vertex[i]和[i+1]（起点特殊处理）
    bool colseFlag; //是否是否闭合
};

#endif // POLYGON_H
