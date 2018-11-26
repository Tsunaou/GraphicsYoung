#ifndef POLYGONCOTROLLER_H
#define POLYGONCOTROLLER_H

#include"FigureController.h"
#include"LineController.h"

class PolygonController : public FigureController
{
public:
    PolygonController();
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
    //本类特有的方法
    void setNextPoints(Point point); //设置下一个顶点
    void changeNextPoints(Point point);
    void drawPolygon(QPainter* painter); //画多边形
    void drawUpPolygon(QPainter* painter);//主界面多边形
    bool changeingVertexs(QMouseEvent *e);  //判断是否在对多边形顶点进行操作
    void changeVertexs(Point point);    //改变顶点位置
private:
    SETPOLYGON setPolygon; //更改多边形的起点/下一个点/任一点/不作操作
    Polygon *curPolyon; //当前聚焦的直线
    Polygon upStreamPolyon; //向上传递的直线
    LineController lineDrawer;//辅助画线
    bool firstEdgeFlag;     //好像没用到QAQ
    int indexChange; //当前正在操作的多边形的顶点

};

#endif // POLYGONCOTROLLER_H
