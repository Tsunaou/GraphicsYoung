#include "Ellipse.h"

Ellipse::Ellipse()
{
    this->start = NULL;
    this->end = NULL;
    this->type = ELLIPSE;
}

Ellipse::Ellipse(QPoint *start, QPoint *end)
{
    this->start = start;
    this->end = end;
    this->type = ELLIPSE;
}

void Ellipse::setStartPoint(Point p)
{
    this->startPoint = p;   //圆心
    this->centerPoint = p;  //圆心

    //2018/11/26对旋转点的定义采取多边形的旋转点定义法
    setRotatePoint();


}

void Ellipse::setEndPoint(Point p)
{
    this->endPoint = p;
    this->centerPoint = this->startPoint;

    setRotatePoint();

}

void Ellipse::setRotatePoint()
{
    double cX = this->centerPoint.getX();
    double cY = this->centerPoint.getY();
    qDebug()<<"中点坐标为: "<<"("<<(int)cX<<","<<(int)cY<<")"<<endl;

    double halfR = startPoint.distanceToPoint(endPoint.getQPoint())/3;

    double offSetX = cX + halfR*qSin(rotateAngle);
    double offSetY = cY - halfR*qCos(rotateAngle);

    rotatePoint.setPoint(offSetX,offSetY); //旋转点
}

void Ellipse::getRectangle()
{
    int rx0 = this->centerPoint.getX(); //基准,圆心
    int ry0 = this->centerPoint.getY();
    //-------------------------------------------------------------------------
    int x = this->endPoint.getX();      //右下
    int y = this->endPoint.getY();
    int rx = (x - rx0)*cos(-rotateAngle) + (y - ry0)*sin(-rotateAngle) + rx0 + 0.5;
    int ry = -(x - rx0)*sin(-rotateAngle) + (y - ry0)*cos(-rotateAngle) + ry0 + 0.5;
    //-------------------------------------------------------------------------
    int x2 = x;                         //右上
    int y2 = 2*ry0-y;
    int rx2 = (x - rx0)*cos(-rotateAngle) + (y2 - ry0)*sin(-rotateAngle) + rx0 + 0.5;
    int ry2 =-(x - rx0)*sin(-rotateAngle) + (y2 - ry0)*cos(-rotateAngle) + ry0 + 0.5;
    //-------------------------------------------------------------------------
    LeftUp.setPoint(2*rx0-rx,2*ry0-ry);   //左上角（minX,minY）
    LeftDown.setPoint(2*rx0-rx2,2*ry0-ry2); //左下角（minX,maxY）
    RightUp.setPoint(rx2,ry2);  //右上角（maxX,minY）
    RightDown.setPoint(rx,ry);//右下角（maxX,maxY）

}
