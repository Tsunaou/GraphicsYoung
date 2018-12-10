#include "EllipseController.h"

EllipseController::EllipseController()
{
    this->curEllipse = NULL;
    this->painter = NULL;
    this->setEllipse = ELLIPSE_NULL;
    this->rotateAngle = 0;
}

void EllipseController::clearState()
{
    this->curEllipse = NULL;
    this->painter = NULL;
    this->setEllipse = ELLIPSE_NULL;
    this->rotateAngle = 0;
}

void EllipseController::getStartAndEnd(QPoint &start, QPoint &end)
{
    start = curEllipse->startPoint.getQPoint(); //将椭圆信息存储下来
    end = curEllipse->endPoint.getQPoint();    //将最终绘制的椭圆信息存储下来
}

void EllipseController::setBigger(QPainter* painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"Before Bigger,start.x is"<< this->curEllipse->startPoint.getX() <<endl;
    this->curEllipse->setStartPoint(this->curEllipse->startPoint*ZOOM_IN);
    this->curEllipse->setEndPoint(this->curEllipse->endPoint*ZOOM_IN);
    qDebug()<<"Line bigger"<<endl;
    qDebug()<<"After Bigger,start.x is"<< this->curEllipse->startPoint.getX() <<endl;
    MyDrawEllipse(painter,curEllipse->startPoint.point,curEllipse->endPoint.point);
    drawHandle(painter,pen);
}

void EllipseController::setSmaller(QPainter* painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"Before Bigger,start.x is"<< this->curEllipse->startPoint.getX() <<endl;
    this->curEllipse->setStartPoint(this->curEllipse->startPoint*ZOOM_OUT);
    this->curEllipse->setEndPoint(this->curEllipse->endPoint*ZOOM_OUT);
    qDebug()<<"Line bigger"<<endl;
    qDebug()<<"After Bigger,start.x is"<< this->curEllipse->startPoint.getX() <<endl;
    MyDrawEllipse(painter,curEllipse->startPoint.point,curEllipse->endPoint.point);
    drawHandle(painter,pen);
}


bool EllipseController::isOperationing(QMouseEvent *e, QPoint &start, QPoint &end)
{
    if(e->button()==Qt::LeftButton){
        if(curEllipse->startPoint.distanceToPoint(e->pos())<=5)
        {
            qDebug()<<"ELLIPSE_CENTER"<<endl;
            setEllipse = ELLIPSE_CENTER;
            return true;
        }
        else if(curEllipse->RightDown.distanceToPoint(e->pos())<=5)
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
    if(e->button()==Qt::LeftButton || e->button()== Qt::RightButton)
    {
        if(e->button()==Qt::LeftButton && curEllipse!=NULL)
        {
            if(curEllipse->startPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"ELLIPSE_CENTER"<<endl;
                setEllipse = ELLIPSE_CENTER;
                return;
            }
            else if(curEllipse->RightDown.distanceToPoint(e->pos())<=5)
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
    //把point转回去，再赋给endPoint
    int rx0 = curEllipse->centerPoint.getX(); //基准,圆心
    int ry0 = curEllipse->centerPoint.getY();
    //-------------------------------------------------------------------------
    int x = point.getX();      //右下
    int y = point.getY();
    int rx = (x - rx0)*cos(rotateAngle) + (y - ry0)*sin(rotateAngle) + rx0 + 0.5;
    int ry = -(x - rx0)*sin(rotateAngle) + (y - ry0)*cos(rotateAngle) + ry0 + 0.5;
    point.setPoint(rx,ry);
    //--------------------------------------
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
    //基准点
    int rx0 = curEllipse->centerPoint.getX();    //基准点
    int ry0 = curEllipse->centerPoint.getY();    //基准点
    //对每个点进行处理(这里有BUG，但是不知道为什么会有)

    //对旋转点处理（用内置的旋转角）
    Point up(this->curEllipse->centerPoint.getX(),0); //表示竖直向上的点
    qreal newAngel = getRotaryAngle(this->curEllipse->centerPoint,up,point);//与竖直的夹角
    if(point.getX()<up.getX()){
        newAngel *= -1;
    }
    this->curEllipse->rotateAngle = newAngel;
    this->curEllipse->setRotatePoint();
    this->rotateAngle = newAngel;   //为椭圆旋转做准备
//    int x = curEllipse->endPoint.getX();         //旋转点
//    int y = curEllipse->endPoint.getY();         //旋转点

//    int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 + 0.5;
//    int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 + 0.5;
//    curEllipse->setEndPoint(Point(rotateStartX,rotateStartY));
//    //有精度损失。。导致可能会变大变小
//    qDebug() << "StartPoint(" <<curEllipse->startPoint.point.x()<<","<<curEllipse->startPoint.point.y()<<")"<<endl;
//    qDebug() << "EndPoint(" <<curEllipse->endPoint.point.x()<<","<<curEllipse->endPoint.point.y()<<")"<<endl;
    MyDrawEllipse(painter,curEllipse->startPoint.getQPoint(),curEllipse->endPoint.getQPoint());
    drawHandle(painter,pen);
}

void EllipseController::setState(DRAW_STATE *state)
{
    this->state = state;
}

void EllipseController::drawHandle(QPainter *painter, QPen pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    this->curEllipse->getRectangle();
    lineDrawer.MyDrawLineDDA(painter,curEllipse->centerPoint.getQPoint(),curEllipse->rotatePoint.getQPoint());
    curEllipse->startPoint.DrawWarnPoint(painter,pen);
    //curEllipse->endPoint.DrawCyclePoint(painter,pen);
    curEllipse->RightDown.DrawCyclePoint(painter,pen);
    curEllipse->rotatePoint.DrawCyclePoint(painter,pen);
    this->drawOutlineToDebug(painter,   curEllipse->LeftUp.getQPoint(),
                                        curEllipse->RightUp.getQPoint(),
                                        curEllipse->RightDown.getQPoint(),
                                        curEllipse->LeftDown.getQPoint());}





void EllipseController::drawQuarterEllipse(QPainter *painter, int x0, int y0, int x, int y)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行


    int rotateStartX = (x)*cos(rotateAngle) + (y)*sin(rotateAngle)  + 0.5;
    int rotateStartY = -(x)*sin(rotateAngle) + (y)*cos(rotateAngle) + 0.5;

    int rotateStartX2 = (x)*cos(-rotateAngle) + (y)*sin(-rotateAngle)  + 0.5;
    int rotateStartY2 = -(x)*sin(-rotateAngle) + (y)*cos(-rotateAngle) + 0.5;

//    rotateStartX = x;
//    rotateStartY = y;

    QPoint temPt1(x0+rotateStartX2,y0+rotateStartY2);
    QPoint temPt2(x0+rotateStartX,y0-rotateStartY);
    QPoint temPt3(x0-rotateStartX,y0+rotateStartY);
    QPoint temPt4(x0-rotateStartX2,y0-rotateStartY2);




    if(fabs(rotateAngle)>1e-6){

        QPen savePen;
        savePen.setStyle(Qt::SolidLine);
        savePen.setWidth(1);
        savePen.setColor(Qt::black);

        QPen debugPen;
        debugPen.setWidth(2);
        debugPen.setColor(Qt::black);
        painter->setPen(debugPen);

        painter->drawPoint(temPt1);
        painter->drawPoint(temPt2);
        painter->drawPoint(temPt3);
        painter->drawPoint(temPt4);

        painter->setPen(savePen);

        return ;
    }

    painter->drawPoint(temPt1);
    painter->drawPoint(temPt2);
    painter->drawPoint(temPt3);
    painter->drawPoint(temPt4);



}

void EllipseController::MyDrawEllipse(QPainter *painter, QPoint &start, QPoint &end)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

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
