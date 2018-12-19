#ifndef CURVECONTROLLER_H
#define CURVECONTROLLER_H

#include "FigureController.h"
#include "Curve.h"


class CurveController: public FigureController
{
public:
    CurveController();
    //继承的抽象方法
    bool isOperationing(QMouseEvent *e,QPoint &start,QPoint &end);    //判断是否有在对图形进行绘制操作
    void mousePressEvent(QPainter* painter, QMouseEvent *e, QPen pen);
    void mouseMoveEvent(QPainter* painter, QMouseEvent *e, QPen pen);
    void mouseReleaseEvent(QPainter* painter,QMouseEvent *e,  QPen pen);
    void setStartPoint(Point point);
    void setEndPoint(Point point);
    void moveToPoint(Point point);
    void rotateToPoint(Point point);
    void setState(DRAW_STATE *state);
    void drawHandle(QPainter* painter, QPen pen);
    void clearState();
    void getStartAndEnd(QPoint &start,QPoint &end);
    void setBigger(QPainter* painter, QMouseEvent *e, QPen pen);   //放大
    void setSmaller(QPainter* painter, QMouseEvent *e, QPen pen);  //缩小
    //本类独特方法
    void drawCurve(QPainter* painter, QPen pen);
    void drawFirstMovingCurve(QPainter* painter, QPen pen);
    void drawBezier(QPainter *painter, QPen pen);
    void drawnode(QVector<PointD>& nodes,QPainter *painter, QPen pen);
    PointD getBezierPoint(PointD a,PointD b,double t);
    int fact(int n);    //阶乘hansu
    void initTestPoints();
    bool changeingVertexs(QMouseEvent *e);
    bool closeSettingPoints();
    bool getIsSettingPoints();
    void changeVertexs(Point point);    //改变控制点位置
    bool isNullCurve();
private:
    QVector<PointD> ctrlPoints;
    QVector<PointD> bezierNodes;
    double t;


    SETCURVE setCurve;  //绘画状态
    Curve * curve;  //当前聚焦的曲线

    bool isSettingPoints;   //正在设置控制点
    bool isFirstDrawing;    //首次绘制，用于动态
    int indexChange; //当前正在操作的曲线的控制顶点
};

#endif // CURVECONTROLLER_H
