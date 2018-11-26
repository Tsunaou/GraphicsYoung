#ifndef CYCLECONTROLLER_H
#define CYCLECONTROLLER_H
#include "FigureController.h"

class CycleController : public FigureController
{
public:
    CycleController();
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
    //该类特有方法
    void MyDrawCycleMidpoint(QPainter* painter,QPoint &start,QPoint &end);
    void MyDrawCycleBresenham(QPainter* painter,QPoint &start,QPoint &end);
    void drawEighthCycle(QPainter* painter,int x0, int y0, int x, int y);
    Point getTheAccurayRotatePoint(qreal ridus,int x,int y);//得到旋转后误差最小的点
private:
    SETCYCLE setCycle; //更改圆的圆心/外围/旋转点/不作操作
    Cycle *cycle; //当前聚焦的圆

};

#endif // CYCLECONTROLLER_H
