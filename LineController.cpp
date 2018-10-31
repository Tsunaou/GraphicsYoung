#include "LineController.h"

LineController::LineController()
{

}

void LineController::mousePressEvent(QPainter *painter, QPoint *start, QPoint *end, QPixmap *pix)
{

}

void LineController::mouseMoveEvent(QPainter *painter, QPoint *start, QPoint *end, QPixmap *pix)
{

}

void LineController::mouseReleaseEvent(QPainter *painter, QPoint *start, QPoint *end, QPixmap *pix)
{

}

void LineController::MyDrawLineBresenham(QPainter *painter, QPoint &start, QPoint &end)
{
    //首先先在这里实现我的直线算法
    qDebug()<<"MyDrawLine Bresenham"<<endl;

    int x1 = start.x();
    int y1 = start.y();
    int x2 = end.x();
    int y2 = end.y();

    int x,y,dx,dy,p;
    x=x1;
    y=y1;
    dx=x2-x1;
    dy=y2-y1;
    p=2*dy-dx;
    for(;x<=x2;x++)
    {
        QPoint temPt(x,y);
        painter->drawPoint(temPt);
        if(p>=0)
        {     y++;
            p+=2*(dy-dx);
        }
        else
        {
            p+=2*dy;
        }
    }
}

void LineController::MyDrawLineDDA(QPainter *painter, QPoint &start, QPoint &end)
{
    //首先先在这里实现我的直线算法
    qDebug()<<"MyDrawLine DDA"<<endl;

    int x1 = start.x();
    int y1 = start.y();
    int x2 = end.x();
    int y2 = end.y();

    double dx=x2-x1;
    double dy=y2-y1;
    double e=(fabs(dx)>fabs(dy))?fabs(dx):fabs(dy);
    double x=x1;
    double y=y1;

    dx/=e;
    dy/=e;

    for(int i=1;i<=e;i++){

        QPoint temPt((int)(x+0.5), (int)(y+0.5));
        painter->drawPoint(temPt);

        x+=dx;
        y+=dy;
    }
}
