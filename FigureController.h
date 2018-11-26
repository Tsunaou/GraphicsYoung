#ifndef FIGURECONTROLLER_H
#define FIGURECONTROLLER_H

#include <QPoint>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <cmath>
#include <QtMath>
#include <QMouseEvent>
#include <QPen>
#include <QVector>
#include "Line.h"
#include "Cycle.h"
#include "Ellipse.h"
#include "Polygon.h"
#include "constParams.h"

class FigureController
{
public:
    FigureController();
    //抽象函数
    virtual void mousePressEvent(QPainter* painter, QMouseEvent *e, QPen pen) =0;
    virtual void mouseMoveEvent(QPainter* painter, QMouseEvent *e, QPen pen) =0;
    virtual void mouseReleaseEvent(QPainter* painter,QMouseEvent *e,  QPen pen) =0;
    virtual bool isOperationing(QMouseEvent *e,QPoint &start,QPoint &end)=0;    //判断是否有在对图形进行绘制操作
    virtual void setStartPoint(Point point) =0; //设置起始点
    virtual void setEndPoint(Point point) =0;   //设置终点
    virtual void moveToPoint(Point point) =0;   //平移
    virtual void rotateToPoint(Point point) =0; //旋转
    virtual void setState(DRAW_STATE *state) =0;    //设置状态
    virtual void drawHandle(QPainter* painter, QPen pen) =0;    //描绘辅助信息
    virtual void clearState() =0;   //情况状态
    virtual void getStartAndEnd(QPoint &start,QPoint &end) =0;
    virtual void setBigger(QPainter* painter, QMouseEvent *e, QPen pen) =0;   //放大
    virtual void setSmaller(QPainter* painter, QMouseEvent *e, QPen pen) =0;  //缩小
    //非抽象函数
    double getRotaryAngle(Point center,Point a,Point b); //得到夹角（返回值是弧度）
    bool clockWise(Point center,Point a,Point b);    //判断是否顺时针
    double getLength(QPoint &start,QPoint &end);    //得到两点间距离
    void drawOutlineToDebug(QPainter* painter,QPoint &start,QPoint &end);   //描绘轮廓
    void drawOutlineToDebug(QPainter* painter,QPoint a,QPoint b,QPoint c,QPoint d);   //描绘轮廓
    void printCtrlDebugMessage(QString msg){
        qDebug()<<msg<<endl;
    }

//protected:
    QPainter *painter;  //画板
    QPen pen;           //画笔
    DRAW_STATE *state;  //绘画状态
};

#endif // FIGURECONTROLLER_H
