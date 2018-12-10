#include "LineController.h"

LineController::LineController()
{
    this->curLine = NULL;
    this->painter = NULL;
    this->setLP =SETNULL;
    this->errorPara = 0;
}

void LineController::clearState()
{
    this->curLine = NULL;
    this->painter = NULL;
    this->setLP =SETNULL;
    this->errorPara = 0;
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

bool LineController::cutLineLiangBsrsky(QPoint cutStart, QPoint cutEnd, QPainter *painter, QPen pen)
{
    //在这里进行判断
    //对裁剪窗口预处理
    double xmin = std::min(cutStart.x(),cutEnd.x());     //决定裁剪窗口的参数
    double ymin = std::min(cutStart.y(),cutEnd.y());     //传入的是决定裁剪窗口的对角线上的顶点，因此做一下处理
    double xmax = std::max(cutStart.x(),cutEnd.x());
    double ymax = std::max(cutStart.y(),cutEnd.y());
    //得到待裁剪直线的各个端点
    double x1 = curLine->startPoint.getX();
    double y1 = curLine->startPoint.getY();
    double x2 = curLine->endPoint.getX();
    double y2 = curLine->endPoint.getY();
    //各个参数的定义
    double dx = x2-x1;  //△x
    double dy = y2-y1;  //△y
    double p[4] = {-dx,dx,-dy,dy};
    double q[4] = {x1-xmin,xmax-x1,y1-ymin,ymax-y1};
    double u1 = 0;
    double u2 = 1;

    //梁友栋裁剪算法，对p和q进行判断
    for(int i=0;i<4;i++){

        if(fabs(p[i])<1e-6 && q[i]<0){  //p=0且q＜0时，舍弃该线段
            this->clearState();
            return false;
        }

        double r = q[i]/p[i];
        if(p[i]<0){
            u1 = r>u1?r:u1; //u1取0和各个r值之中的最大值
        }else{
            u2 = r<u2?r:u2; //u2取1和各个r值之中的最小值
        }

        if(u1>u2){  //如果u1>u2，则线段完全落在裁剪窗口之外，应当被舍弃
            this->clearState();
            return false;
        }
    }
    qDebug()<<"梁友栋"<<endl;
    curLine->setStartPoint(Point(x1+int(u1*dx+0.5), y1+int(u1*dy+0.5)));
    curLine->setEndPoint(Point(x1+int(u2*dx+0.5), y1+int(u2*dy+0.5)));
    MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
    drawHandle(painter,pen);

    return true;

}

Point LineController::getTheAccurayRotatePoint(qreal ridus, int x, int y)
{
    int resX = x-ROTATE_ACCURACY;
    int resY = y-ROTATE_ACCURACY;
    double minDiff =100;
    for(int i=x-ROTATE_ACCURACY;i<=x+ROTATE_ACCURACY;i++){
        for(int j=y-ROTATE_ACCURACY;j<=y+ROTATE_ACCURACY;j++){
            Point tmp(i,j);
            double diffTmp = fabs(this->curLine->centerPoint.distanceToPoint(tmp.getQPoint())-ridus);
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
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

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
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

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
    //把x和y绕rx0和ry0旋转RotaryAngle度
    int x = curLine->startPoint.getX();
    int y = curLine->startPoint.getY();
    int rx0 = curLine->centerPoint.getX();
    int ry0 = curLine->centerPoint.getY();
    int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 ;
    int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 ;

    //curLine->setLength();
    qreal l = this->curLine->getLength();

    qDebug()<<"校正前:坐标为(" << rotateStartX<<","<<rotateStartY<< "),长度为"<<l<<endl;
    Point accurayP = this->getTheAccurayRotatePoint(l/2,rotateStartX,rotateStartY);
    rotateStartX = accurayP.getX();
    rotateStartY = accurayP.getY();

    curLine->setStartPoint(Point(rotateStartX,rotateStartY));
    curLine->setEndPoint(Point(2*rx0-rotateStartX,2*ry0-rotateStartY));
    qreal l2 = curLine->startPoint.distanceToPoint(curLine->endPoint.getQPoint());
    qDebug()<<"校正后:坐标为(" << rotateStartX<<","<<rotateStartY<< "),长度为"<<l2<<endl;
    this->errorPara += l2-l;
    qDebug()<<"旋转累积误差为"<<this->errorPara<<endl;
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



