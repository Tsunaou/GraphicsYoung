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

bool CycleController::isOperationing(QMouseEvent *e, QPoint &start, QPoint &end)
{
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
    else if(cycle->centerPoint.distanceToPoint(e->pos())<=5)
    {
        qDebug()<<"CYCLE_CENTER"<<endl;
        setCycle = CYCLE_CENTER;
        return true;
    }
    else if(cycle->rotatePoint.distanceToPoint(e->pos())<=5)
    {
        setCycle = CYCLE_HANDLE;
        return true;
    }
    setCycle=CYCLE_NULL;
    *state = UNDO;
    start = cycle->startPoint.getQPoint(); //将圆信息存储下来
    end = cycle->endPoint.getQPoint();    //将最终绘制的圆信息存储下来
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
    if(e->button()==Qt::LeftButton)
    {
        if(cycle!=NULL)
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
            else if(cycle->centerPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"CYCLE_CENTER"<<endl;
                setCycle = CYCLE_CENTER;
                return;
            }
            else if(cycle->rotatePoint.distanceToPoint(e->pos())<=5)
            {
                setCycle = CYCLE_HANDLE;
                return;
            }
            setCycle=CYCLE_NULL;
            *state = UNDO;
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
        //case CYCLE_CENTER: this->setStartPoint(curPoint); break;
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
    double RotaryAngle = getRotaryAngle(cycle->centerPoint,cycle->rotatePoint,point);
    bool wiseFlag = clockWise(cycle->centerPoint,cycle->rotatePoint,point);
    if(wiseFlag)
        qDebug()<<"顺时针"<<endl;
    else
        qDebug()<<"逆时针"<<endl;

    if(wiseFlag){//顺时针转
        RotaryAngle *= -1;
    }
    int x = cycle->startPoint.getX();
    int y = cycle->startPoint.getY();
    int rx0 = cycle->centerPoint.getX();
    int ry0 = cycle->centerPoint.getY();
    int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 ;
    int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 ;
    cycle->setStartPoint(Point(rotateStartX,rotateStartY));
    cycle->setEndPoint(Point(2*rx0-rotateStartX,2*ry0-rotateStartY));
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
    cycle->startPoint.DrawCyclePoint(painter,pen);
    cycle->endPoint.DrawCyclePoint(painter,pen);
    cycle->centerPoint.DrawCyclePoint(painter,pen);
    cycle->rotatePoint.DrawCyclePoint(painter,pen);
}


void CycleController::MyDrawCycleBresenham(QPainter *painter, QPoint &start, QPoint &end)//
{
    //首先先在这里实现我的画圆算法
    qDebug()<<"MyDrawCycle "<<endl;
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
    //首先先在这里实现我的画圆算法
    qDebug()<<"MyDrawCycle "<<endl;

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
