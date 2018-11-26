#ifndef ELLIPSECONTROLLER_H
#define ELLIPSECONTROLLER_H
#include "FigureController.h"
#include "LineController.h"

class EllipseController : public FigureController
{
public:
    EllipseController();
    //继承的抽象方法
    bool isOperationing(QMouseEvent *e,QPoint &start,QPoint &end);
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
    //该类的特有方法
    void drawQuarterEllipse(QPainter* painter,int x0, int y0, int x, int y);
    void MyDrawEllipse(QPainter* painter,QPoint &start,QPoint &end);
    void clearRotateAngle(){
        this->rotateAngle = 0;
    }
private:
    SETELLIPSE setEllipse; //更改椭圆的中心/外围/旋转点/不作操作
    Ellipse *curEllipse; //当前聚焦的椭圆
    LineController lineDrawer;
    qreal rotateAngle;
};

#endif // ELLIPSECONTROLLER_H
