#ifndef LINECONTROLLER_H
#define LINECONTROLLER_H
#include "FigureController.h"
#include "Line.h"
#include "constParams.h"

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
    double getRotaryAngle(Point center,Point a,Point b);
    bool clockWise(Point center,Point a,Point b);
    void clearState();
private:
    enum SetLP{SETBEGIN, SETEND, SETCENTER, SETHANDLE, SETNULL} setLP; //目前更改的是直线的起点和终点
    Line *curLine; //当前聚焦的直线
//    QPainter *painter;
//    QPen pen;
    DRAW_STATE *state;
};

#endif // LINECONTROLLER_H
