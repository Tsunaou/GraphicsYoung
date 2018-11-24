#ifndef POINT_H
#define POINT_H
#include <QPoint>
#include <QPainter>
#include <QPen>
class Point
{
public:
    Point();
    Point(int x,int y);
    void setX(int x);
    void setY(int y);
    void setPoint(int x,int y);
    void DrawCyclePoint(QPainter *painter,QPen &pen);
    double distanceToPoint(int x,int y);
    double distanceToPoint(QPoint p);
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
    QPoint& getQPoint(){
        return point;
    }
    Point operator*(const double size);
//private:
    int x;  //横坐标
    int y;  //纵坐标
    int ridus;  //辅助实心点半径
    QPoint point;   //点
};

#endif // POINT_H
