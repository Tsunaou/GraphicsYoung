﻿#include "LineController.h"

LineController::LineController()
{
    this->curLine = NULL;
    this->painter = NULL;
    this->setLP =SETNULL;
}

void LineController::clearState()
{
    this->curLine = NULL;
    this->painter = NULL;
    this->setLP =SETNULL;
}

void LineController::getStartAndEnd(QPoint &start, QPoint &end)
{
    start = curLine->startPoint.getQPoint(); //将直线信息存储下来
    end = curLine->endPoint.getQPoint();    //将最终绘制的直线信息存储下来
}

void LineController::setBigger(QPainter* painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"Before Bigger,start.x is"<< this->curLine->startPoint.getX() <<endl;
    this->curLine->setStartPoint(this->curLine->startPoint*ZOOM_IN);
    this->curLine->setEndPoint(this->curLine->endPoint*ZOOM_IN);
    qDebug()<<"Line bigger"<<endl;
    qDebug()<<"After Bigger,start.x is"<< this->curLine->startPoint.getX() <<endl;
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    drawHandle(painter,pen);
}

void LineController::setSmaller(QPainter* painter, QMouseEvent *e, QPen pen)
{
    this->curLine->setStartPoint(this->curLine->startPoint*ZOOM_OUT);
    this->curLine->setEndPoint(this->curLine->endPoint*ZOOM_OUT);
    qDebug()<<"Line smaller"<<endl;
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    drawHandle(painter,pen);
}

int clipTest(float p,float q,float *u1,float *u2)
{
    int flag = 1;
    float r;
    if(p<0.0){
        r = q/p;
        if(r>*u2)
            flag=0;
        else if(r>*u1)
            *u1=r;
    }
    else if(p>0.0){
        r = q/p;
        if(r<*u1)
            flag = 0;
        else if(r<*u2)
            *u2=r;
    }else if(q<0.0){
        flag = 0;
    }
    return flag;

}

void LineController::cutLineLiangBsrsky(QPoint cutStart, QPoint cutEnd, QPainter *painter, QPen pen)
{
    qDebug()<<"直线裁剪！！"<<endl;
    //在这里进行判断
    int xwmin = cutStart.x();
    int ywmin = cutStart.y();
    int xwmax = cutEnd.x();
    int ywmax = cutEnd.y();
    int x1 = curLine->startPoint.getX();
    int y1 = curLine->startPoint.getY();
    int x2 = curLine->endPoint.getX();
    int y2 = curLine->endPoint.getY();
    //在这里进行判断
    float u1=0.0,u2=1.0,dx=x2-x1,dy;
    if(clipTest(-dx,x1-xwmin,&u1,&u2))
     if(clipTest(dx,xwmax-x1,&u1,&u2))
     { dy=y2-y1;
      if(clipTest(-dy,y1-ywmin,&u1,&u2))
       if(clipTest(dy,ywmax-y1,&u1,&u2))
        {
        if(u2<1.0)
        {
         x2=x1+u2*dx; /*通过u2求得裁剪后的p2端点*/
         y2=y1+u2*dy;
        }
        if(u1>0.0)
        {
         x1=x1+u1*dx; /*通过u1求得裁剪后的p1端点*/
         y1=y1+u1*dy;
        }
        curLine->setStartPoint(Point(x1,y1));
        curLine->setEndPoint(Point(x2,y2));
        MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
        drawHandle(painter,pen);       }
     }

}

Point LineController::getTheAccurayRotatePoint(qreal ridus, int x, int y)
{
    int resX = x-ROTATE_ACCURACY;
    int resY = y-ROTATE_ACCURACY;
    double minDiff =100;
    for(int i=x-ROTATE_ACCURACY;i<=x+ROTATE_ACCURACY;i++){
        for(int j=y-ROTATE_ACCURACY;j<=y+ROTATE_ACCURACY;j++){
            Point tmp(i,j);
            double diffTmp = fabs((2*this->curLine->endPoint.distanceToPoint(tmp.getQPoint()))-ridus);
            if(diffTmp<minDiff){
                resX=i;
                resY=j;
                minDiff = diffTmp;
            }

        }
    }
    return Point(resX,resY);
}

bool LineController::isOperationing(QMouseEvent *e,QPoint &start,QPoint &end)
{
    if(e->button()==Qt::LeftButton){
        if(curLine->startPoint.distanceToPoint(e->pos())<=5)
        {
            qDebug()<<"SETBEGIN"<<endl;
            setLP = SETBEGIN;
            return true;
        }
        else if(curLine->endPoint.distanceToPoint(e->pos())<=5)
        {
            qDebug()<<"SETEND"<<endl;
            setLP = SETEND;
            return true;
        }
        else if(curLine->centerPoint.distanceToPoint(e->pos())<=5)
        {
            setLP = SETCENTER;
            return true;
        }
        else if(curLine->rotatePoint.distanceToPoint(e->pos())<=5)
        {
            setLP = SETHANDLE;
            return true;
        }
    }

    setLP=SETNULL;
    *state = UNDO;
    start = curLine->startPoint.getQPoint(); //将直线信息存储下来
    end = curLine->endPoint.getQPoint();    //将最终绘制的直线信息存储下来
    curLine = NULL;
    return false;
}

void LineController::mousePressEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"LineController::mousePressEvent"<<endl;
    if(curLine!=NULL){
        qDebug()<<"curLine!=NULL"<<endl;
    }else{
        qDebug()<<"curLine==NULL"<<endl;
    }
    if(e->button()==Qt::LeftButton || e->button()== Qt::RightButton)
    {
        if(e->button()==Qt::LeftButton && curLine!=NULL)
        {
            if(curLine->startPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"SETBEGIN"<<endl;
                setLP = SETBEGIN;
                return;
            }
            else if(curLine->endPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"SETEND"<<endl;
                setLP = SETEND;
                return;
            }
            else if(curLine->centerPoint.distanceToPoint(e->pos())<=5)
            {
                setLP = SETCENTER;
                return;
            }
            else if(curLine->rotatePoint.distanceToPoint(e->pos())<=5)
            {
                setLP = SETHANDLE;
                return;
            }
            setLP = SETNULL;
            *state = UNDO;
            curLine = NULL;
            return;
        }
        Point curPoint(e->pos().x(),e->pos().y());
        curLine = new Line();
        curLine->setStartPoint(curPoint);
        curLine->setEndPoint(curPoint);
        setLP = SETEND;
        *state = DRAWING;
    }
}

void LineController::mouseMoveEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"LineController::mouseMoveEvent"<<endl;
    this->painter = painter;
    Point curPoint(e->pos().x(),e->pos().y());
    if (curLine == NULL)
        return;
    switch(setLP){
        case SETBEGIN: this->setStartPoint(curPoint); break;
        case SETEND: this->setEndPoint(curPoint); break;
        case SETCENTER: this->moveToPoint(curPoint); break;
        case SETHANDLE: this->rotateToPoint(curPoint); break;
        default:
            qDebug()<<"Error setLP"<<endl;
    }
}

void LineController::mouseReleaseEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"LineController::mouseReleaseEvent"<<endl;
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
    qDebug()<<"LineController::mouseReleaseEvent"<<endl;
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    drawHandle(painter,pen);
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
    qDebug()<< "before"<<endl;
    int x1 = start.x();
    qDebug()<< "after"<<endl;
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

void LineController::setStartPoint(Point point)
{
    qDebug() << "setStartPoint("<<endl;
    curLine->setStartPoint(point);
    qDebug() << "StartPoint(" <<curLine->startPoint.point.x()<<","<<curLine->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<curLine->endPoint.point.x()<<","<<curLine->endPoint.point.y()<<")"<<endl;
    //顺序不知道为啥会影响粗细。。
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    drawHandle(painter,pen);
}

void LineController::setEndPoint(Point point)
{
    qDebug() << "setEndPoint("<<endl;

    curLine->setEndPoint(point);
    qDebug() << "StartPoint(" <<curLine->startPoint.point.x()<<","<<curLine->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<curLine->endPoint.point.x()<<","<<curLine->endPoint.point.y()<<")"<<endl;
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    drawHandle(painter,pen);
}

void LineController::moveToPoint(Point point)
{
    int offsetX = point.getX() - curLine->centerPoint.getX();
    int offsetY = point.getY() - curLine->centerPoint.getY();
    curLine->setStartPoint(Point(curLine->startPoint.getX()+offsetX,curLine->startPoint.getY()+offsetY));
    curLine->setEndPoint(Point(curLine->endPoint.getX()+offsetX,curLine->endPoint.getY()+offsetY));
    qDebug() << "StartPoint(" <<curLine->startPoint.point.x()<<","<<curLine->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<curLine->endPoint.point.x()<<","<<curLine->endPoint.point.y()<<")"<<endl;
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    drawHandle(painter,pen);
}

void LineController::rotateToPoint(Point point)
{
    double RotaryAngle = getRotaryAngle(curLine->centerPoint,curLine->rotatePoint,point);
    bool wiseFlag = clockWise(curLine->centerPoint,curLine->rotatePoint,point);
    if(wiseFlag)
        qDebug()<<"顺时针"<<endl;
    else
        qDebug()<<"逆时针"<<endl;

    if(wiseFlag){//顺时针转
        RotaryAngle *= -1;
    }
    int x = curLine->startPoint.getX();
    int y = curLine->startPoint.getY();
    int rx0 = curLine->centerPoint.getX();
    int ry0 = curLine->centerPoint.getY();
    int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 ;
    int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 ;

//    qreal l = this->curLine->getLength();

//    Point accurayP = this->getTheAccurayRotatePoint(l,rotateStartX,rotateStartY);
//    rotateStartX = accurayP.getX();
//    rotateStartY = accurayP.getY();

    curLine->setStartPoint(Point(rotateStartX,rotateStartY));
    curLine->setEndPoint(Point(2*rx0-rotateStartX,2*ry0-rotateStartY));
    qDebug() << "StartPoint(" <<curLine->startPoint.point.x()<<","<<curLine->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<curLine->endPoint.point.x()<<","<<curLine->endPoint.point.y()<<")"<<endl;
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    drawHandle(painter,pen);
}

void LineController::setState(DRAW_STATE *state)
{
    this->state = state;
}

void LineController::drawHandle(QPainter *painter, QPen pen)
{
    curLine->startPoint.DrawCyclePoint(painter,pen);
    curLine->endPoint.DrawCyclePoint(painter,pen);
    curLine->centerPoint.DrawWarnPoint(painter,pen);
    curLine->rotatePoint.DrawCyclePoint(painter,pen);
}

void LineController::drawWhileCutting(QPainter *painter, QPen pen)
{
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    curLine->startPoint.DrawCyclePoint(painter,pen);
    curLine->endPoint.DrawCyclePoint(painter,pen);
    curLine->centerPoint.DrawWarnPoint(painter,pen);
    curLine->rotatePoint.DrawCyclePoint(painter,pen);
}



