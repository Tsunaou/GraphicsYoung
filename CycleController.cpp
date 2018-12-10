#include "CycleController.h"

CycleController::CycleController()
{
    this->cycle = NULL;
    this->painter = NULL;
    this->setCycle = CYCLE_NULL;
}

void CycleController::clearState()
{
    this->cycle = NULL;
    this->painter = NULL;
    this->setCycle = CYCLE_NULL;
}

void CycleController::getStartAndEnd(QPoint &start, QPoint &end)
{
    start = cycle->startPoint.getQPoint(); //将圆信息存储下来
    end = cycle->endPoint.getQPoint();    //将最终绘制的圆信息存储下来
}

void CycleController::setBigger(QPainter* painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"Before Bigger,start.x is"<< this->cycle->startPoint.getX() <<endl;
    this->cycle->setStartPoint(this->cycle->startPoint*ZOOM_IN);
    this->cycle->setEndPoint(this->cycle->endPoint*ZOOM_IN);
    qDebug()<<"Line bigger"<<endl;
    qDebug()<<"After Bigger,start.x is"<< this->cycle->startPoint.getX() <<endl;
    MyDrawCycleMidpoint(painter,cycle->startPoint.point,cycle->endPoint.point);
    drawHandle(painter,pen);
}

void CycleController::setSmaller(QPainter* painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"Before Bigger,start.x is"<< this->cycle->startPoint.getX() <<endl;
    this->cycle->setStartPoint(this->cycle->startPoint*ZOOM_OUT);
    this->cycle->setEndPoint(this->cycle->endPoint*ZOOM_OUT);
    qDebug()<<"Line bigger"<<endl;
    qDebug()<<"After Bigger,start.x is"<< this->cycle->startPoint.getX() <<endl;
    MyDrawCycleMidpoint(painter,cycle->startPoint.point,cycle->endPoint.point);
    drawHandle(painter,pen);
}

bool CycleController::isOperationing(QMouseEvent *e, QPoint &start, QPoint &end)
{
    if(e->button()==Qt::LeftButton){
        if(cycle->startPoint.distanceToPoint(e->pos())<=5)
        {
            qDebug()<<"CYCLE_CENTER"<<endl;
            setCycle = CYCLE_CENTER;
            return true;
        }
        else if(cycle->endPoint.distanceToPoint(e->pos())<=5)
        {
            qDebug()<<"CYCLE_OUT"<<endl;
            setCycle = CYCLE_OUT;
            return true;
        }
        else if(cycle->rotatePoint.distanceToPoint(e->pos())<=5)
        {
            qDebug()<<"CYCLE_OUT"<<endl;
            setCycle = CYCLE_HANDLE;
            return true;
        }
    }

    setCycle=CYCLE_NULL;
    *state = UNDO;
    start = cycle->startPoint.getQPoint(); //将圆信息存储下来
    end = cycle->endPoint.getQPoint();    //将最终绘制的圆信息存储下来
    Cycle* p = cycle;   //防止野指针
    delete p;
    cycle = NULL;
    return false;
}

void CycleController::mousePressEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"CycleController::mousePressEvent"<<endl;
    if(cycle!=NULL){
        qDebug()<<"cycle!=NULL"<<endl;
    }else{
        qDebug()<<"cycle==NULL"<<endl;
    }
    if(e->button()==Qt::LeftButton || e->button()== Qt::RightButton)
    {
        if(e->button()==Qt::LeftButton && cycle!=NULL)
        {
            if(cycle->startPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"CYCLE_CENTER"<<endl;
                setCycle = CYCLE_CENTER;
                return;
            }
            else if(cycle->endPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"CYCLE_OUT"<<endl;
                setCycle = CYCLE_OUT;
                return;
            }
            else if(cycle->rotatePoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"CYCLE_HANDLE_ROATATE"<<endl;
                setCycle = CYCLE_HANDLE;
                return;
            }
            setCycle=CYCLE_NULL;
            *state = UNDO;
            Cycle* p = cycle;   //防止野指针
            delete p;
            cycle = NULL;
            return;
        }
        Point curPoint(e->pos().x(),e->pos().y());
        cycle = new Cycle();
        cycle->setStartPoint(curPoint);
        cycle->setEndPoint(curPoint);
        setCycle = CYCLE_OUT;
        *state = DRAWING;
    }
}

void CycleController::mouseMoveEvent(QPainter* painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"CycleController::mouseMoveEvent"<<endl;
    this->painter = painter;
    Point curPoint(e->pos().x(),e->pos().y());
    if (cycle == NULL)
        return;
    switch(setCycle){
        case CYCLE_OUT: this->setEndPoint(curPoint); break;
        case CYCLE_CENTER: this->moveToPoint(curPoint); break;
        case CYCLE_HANDLE: this->rotateToPoint(curPoint); break;
        default:
            qDebug()<<"Error setLP"<<endl;
    }
}

void CycleController::mouseReleaseEvent(QPainter* painter,QMouseEvent *e,  QPen pen)
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
    MyDrawCycleMidpoint(painter,cycle->startPoint.getQPoint(),cycle->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void CycleController::setStartPoint(Point point)
{
    qDebug() << "setStartPoint("<<endl;
    cycle->setStartPoint(point);
    qDebug() << "StartPoint(" <<cycle->startPoint.point.x()<<","<<cycle->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<cycle->endPoint.point.x()<<","<<cycle->endPoint.point.y()<<")"<<endl;
    //顺序不知道为啥会影响粗细。。
    MyDrawCycleMidpoint(painter,cycle->startPoint.getQPoint(),cycle->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void CycleController::setEndPoint(Point point)
{
    qDebug() << "setEndPoint("<<endl;

    cycle->setEndPoint(point);
    qDebug() << "StartPoint(" <<cycle->startPoint.point.x()<<","<<cycle->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<cycle->endPoint.point.x()<<","<<cycle->endPoint.point.y()<<")"<<endl;
    MyDrawCycleMidpoint(painter,cycle->startPoint.getQPoint(),cycle->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void CycleController::moveToPoint(Point point)
{
    int offsetX = point.getX() - cycle->centerPoint.getX();
    int offsetY = point.getY() - cycle->centerPoint.getY();
    cycle->setStartPoint(Point(cycle->startPoint.getX()+offsetX,cycle->startPoint.getY()+offsetY));
    cycle->setEndPoint(Point(cycle->endPoint.getX()+offsetX,cycle->endPoint.getY()+offsetY));
    qDebug() << "StartPoint(" <<cycle->startPoint.point.x()<<","<<cycle->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<cycle->endPoint.point.x()<<","<<cycle->endPoint.point.y()<<")"<<endl;
    MyDrawCycleMidpoint(painter,cycle->startPoint.getQPoint(),cycle->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void CycleController::rotateToPoint(Point point)
{
    qDebug()<<"圆旋转起始：半径为"<<cycle->getRadius()<<endl;
    double RotaryAngle = getRotaryAngle(cycle->centerPoint,cycle->rotatePoint,point);
    bool wiseFlag = clockWise(cycle->centerPoint,cycle->rotatePoint,point);
    if(wiseFlag)
        qDebug()<<"顺时针旋转"<<endl;
    else
        qDebug()<<"逆时针"<<endl;

    if(wiseFlag){//顺时针转
        RotaryAngle *= -1;
    }
    int x = cycle->endPoint.getX();         //旋转点
    int y = cycle->endPoint.getY();         //旋转点
    int rx0 = cycle->centerPoint.getX();    //基准点
    int ry0 = cycle->centerPoint.getY();    //基准点
    if(cycle->centerPoint.getQPoint()!=cycle->startPoint.getQPoint()){
        qDebug()<<"圆心不一"<<endl;
    }
    qDebug()<<"圆旋转中：半径为"<<cycle->getRadius()<<endl;
    int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 + 0.5;
    int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 + 0.5;

    qreal r = this->cycle->getRadius();

    cycle->setEndPoint(getTheAccurayRotatePoint(r,rotateStartX,rotateStartY));
    qDebug()<<"圆旋转后：半径为"<<cycle->getRadius()<<endl;
    //有精度损失。。导致圆可能会变大变小
    qDebug() << "StartPoint(" <<cycle->startPoint.point.x()<<","<<cycle->startPoint.point.y()<<")"<<endl;
    qDebug() << "EndPoint(" <<cycle->endPoint.point.x()<<","<<cycle->endPoint.point.y()<<")"<<endl;
    MyDrawCycleMidpoint(painter,cycle->startPoint.getQPoint(),cycle->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void CycleController::setState(DRAW_STATE *state)
{
    this->state = state;
}

void CycleController::drawHandle(QPainter *painter, QPen pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行
    cycle->startPoint.DrawWarnPoint(painter,pen);
    cycle->endPoint.DrawCyclePoint(painter,pen);
    //cycle->centerPoint.DrawCyclePoint(painter,pen);
    cycle->rotatePoint.DrawCyclePoint(painter,pen);
}


void CycleController::MyDrawCycleBresenham(QPainter *painter, QPoint &start, QPoint &end)//
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    //首先先在这里实现我的画圆算法
    qDebug()<<"MyDrawCycleBresenham "<<endl;
    int x0 = start.x();
    int y0 = start.y();
    double R = this->getLength(start,end);
    int x,y,p;
    x=0;
    y=R;
    p=3-2*R;
    for(;x<=y;x++)
    {
        this->drawEighthCycle(painter,x0,y0,x,y);
        if(p>=0){
           p+=4*(x-y)+10;
           y--;
        }else{
            p+=4*x+6;
        }
    }
}

void CycleController::MyDrawCycleMidpoint(QPainter *painter, QPoint &start, QPoint &end)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    //首先先在这里实现我的画圆算法
    qDebug()<<"MyDrawCycleMidpoint "<<endl;

    int x0 = start.x();
    int y0 = start.y();
    double R = this->getLength(start,end);
    int x,y;
    double d;
    x = 0;
    y = R;
    d = 1-R;
    int deltaX = 3;
    int deltaY = 5-2*R;
    this->drawEighthCycle(painter,x0,y0,x,y);

    while(x<y){
        if(d<0){
            d+=deltaX;
            deltaX+=2;
            deltaY+=2;
            x++;
        }
        else
        {
            d+=deltaY;
            deltaX+=2;
            deltaY+=4;
            x++;
            y--;
        }
        this->drawEighthCycle(painter,x0,y0,x,y);
    }
}


void CycleController::drawEighthCycle(QPainter *painter, int x0, int y0, int x, int y)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    QPoint temPt1(x0+x,y0+y);
    QPoint temPt2(x0+y,y0+x);
    QPoint temPt3(x0+x,y0-y);
    QPoint temPt4(x0+y,y0-x);
    QPoint temPt5(x0-x,y0-y);
    QPoint temPt6(x0-y,y0-x);
    QPoint temPt7(x0-x,y0+y);
    QPoint temPt8(x0-y,y0+x);

    painter->drawPoint(temPt1);
    painter->drawPoint(temPt2);
    painter->drawPoint(temPt3);
    painter->drawPoint(temPt4);
    painter->drawPoint(temPt5);
    painter->drawPoint(temPt6);
    painter->drawPoint(temPt7);
    painter->drawPoint(temPt8);
}

Point CycleController::getTheAccurayRotatePoint(qreal ridus, int x, int y)
{
    int resX = x-ROTATE_ACCURACY;
    int resY = y-ROTATE_ACCURACY;
    double minDiff =100;
    for(int i=x-ROTATE_ACCURACY;i<=x+ROTATE_ACCURACY;i++){
        for(int j=y-ROTATE_ACCURACY;j<=y+ROTATE_ACCURACY;j++){
            Point tmp(i,j);
            double diffTmp = fabs(this->cycle->centerPoint.distanceToPoint(tmp.getQPoint())-ridus);
            if(diffTmp<minDiff){
                resX=i;
                resY=j;
                minDiff = diffTmp;
            }

        }
    }
    return Point(resX,resY);
}
