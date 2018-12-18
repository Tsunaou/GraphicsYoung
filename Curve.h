#ifndef CURVE_H
#define CURVE_H
#include "Figure.h"

class Curve : public Figure
{
public:
    Curve();
    void setNextPoint(Point p);
    void changePoint(int index,Point p);
    void getRectangle();    //得到辅助矩形
    void getRectangleRotating(); //旋转时得保持稳定
    QVector<Point> vertex;  //存储端点

    //辅助矩形
    Point LeftUp;   //左上角（minX,minY）
    Point LeftDown; //左下角（minX,maxY）
    Point RightUp;  //右上角（maxX,minY）
    Point RightDown;//右下角（maxX,maxY）
    int xCenter;
    int yCenter;
};

#endif // CURVE_H
