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
    void changePoint(int index,Point p);
    void getRectangle();    //得到辅助矩形
    void getRectangleRotating(); //旋转时得保持稳定
    QVector<Line> edges;    //存储边,其实都没用到。。不过也不占太大空间吧
    QVector<Point> vertex;  //存储端点
    //这样edges[i]的两个端点便是 vertex[i]和[i+1]（起点特殊处理）
    bool colseFlag; //是否是否闭合
    //辅助矩形
    Point LeftUp;   //左上角（minX,minY）
    Point LeftDown; //左下角（minX,maxY）
    Point RightUp;  //右上角（maxX,minY）
    Point RightDown;//右下角（maxX,maxY）
    int xCenter;
    int yCenter;
};

#endif // POLYGON_H
