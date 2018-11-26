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
