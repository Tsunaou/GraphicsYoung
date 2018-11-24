#include "EllipseController.h"

EllipseController::EllipseController()
{

}

bool EllipseController::isOperationing(QMouseEvent *e, QPoint &start, QPoint &end)
{

}

void EllipseController::mousePressEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{

}

void EllipseController::mouseMoveEvent(QPainter* painter, QMouseEvent *e, QPen pen)
{

}

void EllipseController::mouseReleaseEvent(QPainter* painter,QMouseEvent *e,  QPen pen)
{

}

void EllipseController::setStartPoint(Point point)
{

}

void EllipseController::setEndPoint(Point point)
{

}

void EllipseController::moveToPoint(Point point)
{

}

void EllipseController::rotateToPoint(Point point)
{

}

void EllipseController::setState(DRAW_STATE *state)
{

}

void EllipseController::drawHandle(QPainter *painter, QPen pen)
{

}

void EllipseController::clearState()
{

}

void EllipseController::drawQuarterEllipse(QPainter *painter, int x0, int y0, int x, int y)
{
    QPoint temPt1(x0+x,y0+y);
    QPoint temPt2(x0+x,y0-y);
    QPoint temPt3(x0-x,y0+y);
    QPoint temPt4(x0-x,y0-y);

    painter->drawPoint(temPt1);
    painter->drawPoint(temPt2);
    painter->drawPoint(temPt3);
    painter->drawPoint(temPt4);
}

void EllipseController::MyDrawEllipse(QPainter *painter, QPoint &start, QPoint &end)
{
    //用一个虚线框来解决
//    drawOutlineToDebug(painter,start,end);
    //绘制一个点
    if(start == end){
        painter->drawPoint(start);
        return;
    }

    //首先先在这里实现我的椭圆算法
    qDebug()<<"MyDrawEllipse "<<endl;
    int x0 = start.x(); //椭圆中心
    int y0 = start.y();
    int rx = abs(end.x()-x0); //椭圆长短轴
    int ry = abs(end.y()-y0);

    double rx_2 = rx*rx;
    double ry_2 = ry*ry;

    double pl = ry_2 - rx_2*ry + rx_2/4; //区域1中决策参数
    int x = 0;
    int y = ry;
    drawQuarterEllipse(painter,x0, y0, x, y);//第一个点

    //区域一 切线斜率k<=1
    while (ry_2*x <= rx_2*y){

        if (pl < 0){
            pl += 2*ry_2*x +3*ry_2;
        }else{
            pl += 2*ry_2*x - 2*rx_2*y + 2*rx_2+ 3*ry_2;
            y--;
        }
        x++;
        drawQuarterEllipse(painter,x0, y0, x, y);
    }
    //区域二 切线斜率k > 1
    pl = ry_2*(x+1/2)*(x+1/2)+rx_2*(y-1)*(y-1)-rx_2*ry_2;  //使用区域1中最后点(x0,y0)来计算区域2中参数初值

    while (y > 0){
        if (pl < 0){
            pl += 2*ry_2*x - 2*rx_2*y + 2*ry_2 + 3*rx_2;
            x++;
        }
        else{
            pl += 3*rx_2 - 2*rx_2*y;
        }
        y--;
        drawQuarterEllipse(painter,x0, y0, x, y);
    }
}
