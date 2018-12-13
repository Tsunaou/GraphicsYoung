#ifndef CURVECONTROLLER_H
#define CURVECONTROLLER_H

#include "FigureController.h"

class PointD{
public:
    PointD(){
        this->x=0.0;
        this->y=0.0;
    }
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
    void drawnode(QVector<PointD>& nodes,QPainter *painter, QPen pen);
    PointD getBezierPoint(PointD a,PointD b,double t);
    int fact(int n);
private:
    QVector<PointD> ctrlPoints;
    QVector<PointD> bezierNodes;
    double t;
};

#endif // CURVECONTROLLER_H
