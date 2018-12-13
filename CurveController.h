#ifndef CURVECONTROLLER_H
#define CURVECONTROLLER_H

#include "FigureController.h"

class PointD{
    PointD(double x,double y){
        this->x=x;
        this->y=y;
    }
    double x;
    double y;

public:
    double getX() const;
    double getY() const;
};

class CurveController
{
public:
    CurveController();
    void drawCurve(QPainter* painter, QPen pen);
    void drawBezier(QPainter *painter, QPen pen);
    void drawnode(QVector<Point>& nodes,QPainter *painter, QPen pen);
    Point getBezierPoint(Point a,Point b,double t);
    int fact(int n);
private:
    QVector<Point> ctrlPoints;
    QVector<Point> bezierNodes;
    double t;
};

#endif // CURVECONTROLLER_H
