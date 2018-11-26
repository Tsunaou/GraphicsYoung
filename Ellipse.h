#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "Figure.h"

class Ellipse : public Figure
{
public:
    Ellipse();
    Ellipse(QPoint* start,QPoint* end);
    void setStartPoint(Point p);
    void setEndPoint(Point p);
    void setRotatePoint();
    void getRectangle();    //得到辅助矩形
    //辅助矩形
    Point LeftUp;   //左上角（minX,minY）
    Point LeftDown; //左下角（minX,maxY）
    Point RightUp;  //右上角（maxX,minY）
    Point RightDown;//右下角（maxX,maxY）
};

#endif // ELLIPSE_H
