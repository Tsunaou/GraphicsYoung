#ifndef LINECONTROLLER_H
#define LINECONTROLLER_H
#include "FigureController.h"
#include "Line.h"
#include "constParams.h"
#include <QMouseEvent>
#include <QPen>
#include <QVector>
class LineController/* : public FigureController*/
{
public:
    LineController();
    bool isOperationing(QMouseEvent *e);
    void mousePressEvent(QPainter* painter, QMouseEvent *e, QPen pen);
    void mouseMoveEvent(QPainter* painter, QMouseEvent *e, QPen pen);
    void mouseReleaseEvent(QPainter* painter,QMouseEvent *e,  QPen pen);
    void MyDrawLineBresenham(QPainter* painter,QPoint &start,QPoint &end);
    void MyDrawLineDDA(QPainter* painter,QPoint &start,QPoint &end);
    void setStartPoint(Point point);
    void setEndPoint(Point point);
    void setState(DRAW_STATE *state);
private:
    enum SetLP{SETBEGIN, SETEND, SETCENTER, SETHANDLE, SETNULL} setLP; //目前更改的是直线的起点和终点
    Line *curLine; //当前聚焦的直线
    QPainter *painter;
    QPen pen;
    DRAW_STATE *state;
};

#endif // LINECONTROLLER_H
