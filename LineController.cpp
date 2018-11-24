#include "LineController.h"

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

bool LineController::isOperationing(QMouseEvent *e,QPoint &start,QPoint &end)
{
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
    qDebug()<<"2"<<endl;
    if(curLine!=NULL){
        qDebug()<<"curLine!=NULL"<<endl;
    }else{
        qDebug()<<"curLine==NULL"<<endl;
    }
    if(e->button()==Qt::LeftButton)
    {
        if(curLine!=NULL/* && setLP == SETNULL*/)
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
        qDebug()<<"3"<<endl;
        setLP = SETEND;
        *state = DRAWING;
    }
    qDebug()<<"4"<<endl;
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
//        drawHandle(painter,pen);
//        MyDrawLineDDA(painter,curLine->startPoint.point,curLine->endPoint.point);
//        return;
    }
    int x = curLine->startPoint.getX();
    int y = curLine->startPoint.getY();
    int rx0 = curLine->centerPoint.getX();
    int ry0 = curLine->centerPoint.getY();
    int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 ;
    int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 ;
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
    curLine->centerPoint.DrawCyclePoint(painter,pen);
    curLine->rotatePoint.DrawCyclePoint(painter,pen);
}

double LineController::getRotaryAngle(Point center, Point a, Point b)
{
    double ab = a.distanceToPoint(b.getQPoint());
    double ac = a.distanceToPoint(center.getQPoint());
    double bc = b.distanceToPoint(center.getQPoint());
    qreal cosC = (bc*bc+ac*ac-ab*ab)/(2*bc*ac);
    double theta = qAcos(cosC);
    qDebug()<<"Angle is"<<theta<<endl;
    return theta; //弧度制
}

bool LineController::clockWise(Point center, Point a, Point b)
{
    //k=0
    if(center.getY()==a.getY()){
        if(a.getX()>center.getX())// -------->型向量
        {
            if(b.getY() > a.getY()){
                return true;
            }else{
                return false;
            }
        }
        else// <--------型向量
        {
            if(b.getY() < a.getY()){
                return true;
            }else{
                return false;
            }
        }


    }
    //k不存在
    if(center.getX()==a.getX()){
        if(a.getY()<center.getY())//竖直向上
        {
            if(b.getX()>a.getX()){
                return true;
            }else{
                return  false;
            }
        }
        else //竖直向下
        {
            if(b.getX()<a.getX()){
                return true;
            }else{
                return  false;
            }
        }
    }
    //斜率存在切不为零
    double x0 = 0;
    double y0 = 0;
    double x1 = a.getQPoint().x() - center.getQPoint().x(); //把中点当原点
    double y1 = a.getQPoint().y() - center.getQPoint().y(); //把中点当原点
    double k = (y1-y0)/(x1-x0);
    double x2 = b.getX() - center.getX();   //把中点当原点（要用坐标直接比较还得标准化）
    double y2 = b.getY() - center.getY();   //把中点当原点
    qDebug()<<"Rotate:: start("<<x1<<","<<y1<<endl;
    qDebug()<<"Rotate:: end("<<x2<<","<<y2<<endl;
//    Point stdPoint(x2,y2);
//    double stdR = a.distanceToPoint(center.getQPoint());
//    double stdPara = stdR / stdPoint.distanceToPoint(center.getQPoint());
//    x2 *= stdPara;
//    y2 *= stdPara;
    //对斜率和向量方向进行判断。。有点麻烦
    if(a.getX()>center.getX())//方向向右
    {
        if(y2>(k*x2)){ //y > kx
            return true;
        }else{
            return false;
        }
    }
    else //方向朝左
    {
        if(y2<(k*x2)){ //y > kx
            return true;
        }else{
            return false;
        }
    }


}


