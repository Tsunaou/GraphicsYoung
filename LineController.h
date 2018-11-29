#ifndef LINECONTROLLER_H
#define LINECONTROLLER_H
#include "FigureController.h"


class LineController : public FigureController
{
public:
    LineController();
    bool isOperationing(QMouseEvent *e,QPoint &start,QPoint &end);
    void mousePressEvent(QPainter* painter, QMouseEvent *e, QPen pen);
    void mouseMoveEvent(QPainter* painter, QMouseEvent *e, QPen pen);
    void mouseReleaseEvent(QPainter* painter,QMouseEvent *e,  QPen pen);
    void MyDrawLineBresenham(QPainter* painter,QPoint &start,QPoint &end);
    void MyDrawLineDDA(QPainter* painter,QPoint &start,QPoint &end);
    void setStartPoint(Point point);
    void setEndPoint(Point point);
    void moveToPoint(Point point);
    void rotateToPoint(Point point);
    void setState(DRAW_STATE *state);
    void drawHandle(QPainter* painter, QPen pen);
    void drawWhileCutting(QPainter* painter, QPen pen);
    void clearState();
    void getStartAndEnd(QPoint &start,QPoint &end);
    void setBigger(QPainter* painter, QMouseEvent *e, QPen pen);   //放大
    void setSmaller(QPainter* painter, QMouseEvent *e, QPen pen);  //缩小
    bool cutLineLiangBsrsky(QPoint cutStart,QPoint cutEnd,QPainter* painter, QPen pen); //梁友栋裁剪直线算法
    Point getTheAccurayRotatePoint(qreal ridus,int x,int y);//得到旋转后误差最小的点
    //
private:
    SETLINE setLP; //更改直线的起点/终点/中点/旋转点/不作操作
    Line *curLine; //当前聚焦的直线
    qreal errorPara;    //旋转累积误差
};

#endif // LINECONTROLLER_H
