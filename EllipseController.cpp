#include "EllipseController.h"

EllipseController::EllipseController()
{
    this->curEllipse = NULL;
    this->painter = NULL;
    this->setEllipse = ELLIPSE_NULL;
}

void EllipseController::clearState()
{
    this->curEllipse = NULL;
    this->painter = NULL;
    this->setEllipse = ELLIPSE_NULL;
}

void EllipseController::getStartAndEnd(QPoint &start, QPoint &end)
{
    start = curEllipse->startPoint.getQPoint(); //将椭圆信息存储下来
    end = curEllipse->endPoint.getQPoint();    //将最终绘制的椭圆信息存储下来
}

void EllipseController::setBigger(QPainter* painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"Before Bigger,start.x is"<< this->curEllipse->startPoint.getX() <<endl;
    this->curEllipse->setStartPoint(this->curEllipse->startPoint*2);
    this->curEllipse->setEndPoint(this->curEllipse->endPoint*2);
    qDebug()<<"Line bigger"<<endl;
    qDebug()<<"After Bigger,start.x is"<< this->curEllipse->startPoint.getX() <<endl;
    MyDrawEllipse(painter,curEllipse->startPoint.point,curEllipse->endPoint.point);
    drawHandle(painter,pen);
}

void EllipseController::setSmaller(QPainter* painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"Before Bigger,start.x is"<< this->curEllipse->startPoint.getX() <<endl;
    this->curEllipse->setStartPoint(this->curEllipse->startPoint*0.5);
    this->curEllipse->setEndPoint(this->curEllipse->endPoint*0.5);
    qDebug()<<"Line bigger"<<endl;
    qDebug()<<"After Bigger,start.x is"<< this->curEllipse->startPoint.getX() <<endl;
    MyDrawEllipse(painter,curEllipse->startPoint.point,curEllipse->endPoint.point);
    drawHandle(painter,pen);
}


bool EllipseController::isOperationing(QMouseEvent *e, QPoint &start, QPoint &end)
{
 if(curEllipse->startPoint.distanceToPoint(e->pos())<=5)
    {
        qDebug()<<"ELLIPSE_CENTER"<<endl;
        setEllipse = ELLIPSE_CENTER;
        return true;
    }
    else if(curEllipse->endPoint.distanceToPoint(e->pos())<=5)
    {
        qDebug()<<"ELLIPSE_OUT"<<endl;
        setEllipse = ELLIPSE_OUT;
        return true;
    }
    else if(curEllipse->rotatePoint.distanceToPoint(e->pos())<=5)
    {
        qDebug()<<"ELLIPSE_HANDLE"<<endl;
        setEllipse = ELLIPSE_HANDLE;
        return true;
    }
    setEllipse = ELLIPSE_NULL;
    *state = UNDO;
    start = curEllipse->startPoint.getQPoint(); //将圆信息存储下来
    end = curEllipse->endPoint.getQPoint();    //将最终绘制的圆信息存储下来
    Ellipse* p = curEllipse;   //防止野指针
    delete p;
    curEllipse = NULL;
    return false;
}


void EllipseController::mousePressEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
  qDebug()<<"CycleController::mousePressEvent"<<endl;
    if(curEllipse!=NULL){
        qDebug()<<"cycle!=NULL"<<endl;
    }else{
        qDebug()<<"cycle==NULL"<<endl;
    }
    if(e->button()==Qt::LeftButton)
    {
        if(curEllipse!=NULL)
        {
            if(curEllipse->startPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"ELLIPSE_CENTER"<<endl;
                setEllipse = ELLIPSE_CENTER;
                return;
            }
            else if(curEllipse->endPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"ELLIPSE_OUT"<<endl;
                setEllipse = ELLIPSE_OUT;
                return;
            }
            else if(curEllipse->rotatePoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"ELLIPSE_HANDLE"<<endl;
                setEllipse = ELLIPSE_HANDLE;
                return;
            }
            setEllipse = ELLIPSE_NULL;
            *state = UNDO;
            Ellipse* p = curEllipse;   //防止野指针
            delete p;
            curEllipse = NULL;
            return;
        }
        Point curPoint(e->pos().x(),e->pos().y());
        curEllipse = new Ellipse();
        curEllipse->setStartPoint(curPoint);
        curEllipse->setEndPoint(curPoint);
        setEllipse = ELLIPSE_OUT;
        *state = DRAWING;
    }
}

void EllipseController::mouseMoveEvent(QPainter* painter, QMouseEvent *e, QPen pen)
{
 qDebug()<<"CycleController::mouseMoveEvent"<<endl;
    this->painter = painter;
    Point curPoint(e->pos().x(),e->pos().y());
    if (curEllipse == NULL)
        return;
    switch(setEllipse){
        case ELLIPSE_OUT: this->setEndPoint(curPoint); break;
        case ELLIPSE_CENTER: this->moveToPoint(curPoint); break;
        case ELLIPSE_HANDLE: this->rotateToPoint(curPoint); break;
        default:
            qDebug()<<"Error setLP"<<endl;
    }
}

void EllipseController::mouseReleaseEvent(QPainter* painter,QMouseEvent *e,  QPen pen)
{
  qDebug()<<"CycleController::mouseReleaseEvent"<<endl;
    qDebug()<<"state is";
    switch (*state) {
        case UNDO:
            qDebug()<<"UNDO"<<endl;
            break;
        case DRAWING :
            qDebug()<<"DRAWING"<<endl;
            break;
    }
    this->painter = painter;
    MyDrawEllipse(painter,curEllipse->startPoint.getQPoint(),curEllipse->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void EllipseController::setStartPoint(Point point)
{
 qDebug() << "setStartPoint("<<endl;
    curEllipse->setStartPoint(point);
    qDebug() << "StartPoint(" <<curEllipse->startPoint.point.x()<<","<<curEllipse->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<curEllipse->endPoint.point.x()<<","<<curEllipse->endPoint.point.y()<<")"<<endl;
    //顺序不知道为啥会影响粗细。。
    MyDrawEllipse(painter,curEllipse->startPoint.getQPoint(),curEllipse->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void EllipseController::setEndPoint(Point point)
{
    qDebug() << "setEndPoint("<<endl;

    curEllipse->setEndPoint(point);
    qDebug() << "StartPoint(" <<curEllipse->startPoint.point.x()<<","<<curEllipse->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<curEllipse->endPoint.point.x()<<","<<curEllipse->endPoint.point.y()<<")"<<endl;
    MyDrawEllipse(painter,curEllipse->startPoint.getQPoint(),curEllipse->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void EllipseController::moveToPoint(Point point)
{
    int offsetX = point.getX() - curEllipse->centerPoint.getX();
    int offsetY = point.getY() - curEllipse->centerPoint.getY();
    curEllipse->setStartPoint(Point(curEllipse->startPoint.getX()+offsetX,curEllipse->startPoint.getY()+offsetY));
    curEllipse->setEndPoint(Point(curEllipse->endPoint.getX()+offsetX,curEllipse->endPoint.getY()+offsetY));
    qDebug() << "StartPoint(" <<curEllipse->startPoint.point.x()<<","<<curEllipse->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<curEllipse->endPoint.point.x()<<","<<curEllipse->endPoint.point.y()<<")"<<endl;
    MyDrawEllipse(painter,curEllipse->startPoint.getQPoint(),curEllipse->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void EllipseController::rotateToPoint(Point point)
{
    double RotaryAngle = getRotaryAngle(curEllipse->centerPoint,curEllipse->rotatePoint,point);
    bool wiseFlag = clockWise(curEllipse->centerPoint,curEllipse->rotatePoint,point);
    if(wiseFlag)
        qDebug()<<"顺时针旋转"<<endl;
    else
        qDebug()<<"逆时针"<<endl;

    if(wiseFlag){//顺时针转
        RotaryAngle *= -1;
    }
    int x = curEllipse->endPoint.getX();         //旋转点
    int y = curEllipse->endPoint.getY();         //旋转点
    int rx0 = curEllipse->centerPoint.getX();    //基准点
    int ry0 = curEllipse->centerPoint.getY();    //基准点

    int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 + 0.5;
    int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 + 0.5;
    curEllipse->setEndPoint(Point(rotateStartX,rotateStartY));
    //有精度损失。。导致可能会变大变小
    qDebug() << "StartPoint(" <<curEllipse->startPoint.point.x()<<","<<curEllipse->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<curEllipse->endPoint.point.x()<<","<<curEllipse->endPoint.point.y()<<")"<<endl;
    MyDrawEllipse(painter,curEllipse->startPoint.getQPoint(),curEllipse->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void EllipseController::setState(DRAW_STATE *state)
{
    this->state = state;
}

void EllipseController::drawHandle(QPainter *painter, QPen pen)
{
    curEllipse->startPoint.DrawCyclePoint(painter,pen);
    curEllipse->endPoint.DrawCyclePoint(painter,pen);
    //cycle->centerPoint.DrawCyclePoint(painter,pen);
    curEllipse->rotatePoint.DrawCyclePoint(painter,pen);
    this->drawOutlineToDebug(painter,curEllipse->startPoint.getQPoint(),curEllipse->endPoint.getQPoint());
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
