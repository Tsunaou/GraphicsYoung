#include "PolygonController.h"

PolygonController::PolygonController()
{
    this->curPolyon = NULL;
    this->painter = NULL;
    this->setPolygon = POLYGON_NULL;
    firstEdgeFlag = true;
}

bool PolygonController::isOperationing(QMouseEvent *e, QPoint &start, QPoint &end)
{
    if(curPolyon->colseFlag==false && curPolyon->startPoint.distanceToPoint(e->pos())<=25)
    {
        qDebug()<<"POLYGON_START"<<endl;
        setPolygon = POLYGON_START;
        return true;
    }
    else if(curPolyon->colseFlag==false && *state!=UNDO){ //暂时苟且一下
        qDebug()<<"POLYGON_NEXT"<<endl;
        setPolygon = POLYGON_NEXT;
        return true;
    }
    else if(changeingVertexs(e)){
        qDebug()<<"POLYGON_CHANGE"<<endl;
        setPolygon = POLYGON_CHANGE;
        return true;
    }

    setPolygon = POLYGON_NULL;
    *state = UNDO;
    start = curPolyon->startPoint.getQPoint(); //将多边形信息存储下来
    end = curPolyon->endPoint.getQPoint();    //将最终绘制的多边形信息存储下来
    this->upStreamPolyon = *curPolyon;
    Polygon* p = curPolyon;   //防止野指针
    delete p;
    curPolyon = NULL;
    return false;
}

void PolygonController::mousePressEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"PolygonController::mousePressEvent"<<endl;
    if(curPolyon!=NULL){
        qDebug()<<"curPolyon!=NULL"<<endl;
    }else{
        qDebug()<<"curPolyon==NULL"<<endl;
    }
    if(e->button()==Qt::LeftButton)
    {
        if(curPolyon!=NULL)
        {
            bool returnFlag = false; //暂时没有什么用
            if(curPolyon->colseFlag==false && curPolyon->startPoint.distanceToPoint(e->pos())<=25)
            {
                qDebug()<<"POLYGON_START The Edges Limits"<<endl;
                setPolygon = POLYGON_START;
                returnFlag = true;
                curPolyon->colseFlag = true;
                Point curPoint = curPolyon->startPoint;
                this->curPolyon->setNextPoint(curPoint);
                this->drawPolygon(painter);
                this->drawHandle(painter,pen);
                return;
            }
            else if(curPolyon->colseFlag==false && *state!=UNDO){ //暂时苟且一下
                qDebug()<<"POLYGON_NEXT"<<endl;
                setPolygon = POLYGON_NEXT;
                returnFlag = true;
                Point curPoint(e->pos().x(),e->pos().y());
                this->curPolyon->setNextPoint(curPoint);
                return;
            }
            else if(changeingVertexs(e)){
                qDebug()<<"POLYGON_CHANGE"<<endl;
                setPolygon = POLYGON_CHANGE;
                return;
            }
            if(returnFlag){
                return;
            }
            curPolyon->colseFlag = false;


            setPolygon = POLYGON_NULL;
            *state = UNDO;
            Polygon* p = curPolyon;   //防止野指针
            delete p;
            curPolyon = NULL;
            return;
        }
        //printCtrlDebugMessage("PolygonController::mousePressEvent end when cur is null 1");
        QPoint curPoint(e->pos().x(),e->pos().y());
        curPolyon = new Polygon(&curPoint,&curPoint,POLYGON);
        curPolyon->setNextPoint(Point());
        //printCtrlDebugMessage("PolygonController::mousePressEvent end when cur is null 2");
        setPolygon = POLYGON_NEXT;
        //printCtrlDebugMessage("PolygonController::mousePressEvent end when cur is null 3");
        *state = DRAWING;
        //printCtrlDebugMessage("PolygonController::mousePressEvent end when cur is null 4");
    }
}

void PolygonController::mouseMoveEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"PolygonController::mouseMoveEvent"<<endl;
    this->painter = painter;
    //printCtrlDebugMessage("mouseMoveEvent 1");
    Point curPoint(e->pos().x(),e->pos().y());
    //printCtrlDebugMessage("mouseMoveEvent 2");
    if (curPolyon == NULL)
        return;
    switch(setPolygon){
        case POLYGON_START: this->setStartPoint(curPoint); break;
        case POLYGON_NEXT: this->changeNextPoints(curPoint); break;
        case POLYGON_CHANGE: this->changeVertexs(curPoint); break;
        default:
            qDebug()<<"Error setPolygon"<<endl;
    }
    //printCtrlDebugMessage("mouseMoveEvent 3");
}


void PolygonController::mouseReleaseEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"PolygonController::mouseReleaseEvent"<<endl;
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

    if(curPolyon->startPoint.distanceToPoint(e->pos())<=25)
    {
        qDebug()<<"POLYGON_START The Edges Limits"<<endl;
        setPolygon = POLYGON_START;
        curPolyon->colseFlag = true;
        Point curPoint = curPolyon->startPoint;
        this->curPolyon->changeNextPoint(curPoint);
        this->drawPolygon(painter);
        this->drawHandle(painter,pen);
        return;
    }

    drawPolygon(painter);
    drawHandle(painter,pen);
}

void PolygonController::setStartPoint(Point point)
{
    qDebug() << "setStartPoint("<<endl;
    this->curPolyon->setNextPoint(point);
    //顺序不知道为啥会影响粗细。。
    drawPolygon(painter);
    drawHandle(painter,pen);
}

void PolygonController::setEndPoint(Point point)
{

}

void PolygonController::moveToPoint(Point point)
{

}

void PolygonController::rotateToPoint(Point point)
{

}

void PolygonController::setState(DRAW_STATE *state)
{
    this->state = state;
}

void PolygonController::drawHandle(QPainter *painter, QPen pen)
{
    for(Point i : this->curPolyon->vertex){
        i.DrawCyclePoint(painter,pen);
    }
    curPolyon->vertex.first().DrawWarnPoint(painter,pen);
}

void PolygonController::clearState()
{
    this->curPolyon = NULL;
    this->painter = NULL;
    this->setPolygon = POLYGON_NULL;
}

void PolygonController::getStartAndEnd(QPoint &start, QPoint &end)
{
    this->upStreamPolyon = *curPolyon;
}

void PolygonController::setBigger(QPainter *painter, QMouseEvent *e, QPen pen)
{

}

void PolygonController::setSmaller(QPainter *painter, QMouseEvent *e, QPen pen)
{

}

void PolygonController::setNextPoints(Point point)
{
    qDebug() << "setNextPoints("<<endl;
    this->curPolyon->setNextPoint(point);
    //顺序不知道为啥会影响粗细。。
    drawPolygon(painter);
    drawHandle(painter,pen);
}

void PolygonController::changeNextPoints(Point point)
{
    qDebug() << "setNextPoints("<<endl;
    this->curPolyon->changeNextPoint(point);
    //顺序不知道为啥会影响粗细。。
    drawPolygon(painter);
    drawHandle(painter,pen);
}

void PolygonController::drawPolygon(QPainter* painter)
{
    qDebug()<<"drawPolygon"<<endl;
    //return ;
    for(int i=0;i<curPolyon->vertex.size();i++){
        curPolyon->vertex[i].printPointCoodinate();
    }
    for(int i=0;i<curPolyon->vertex.size();i++){
        if((i+1)<curPolyon->vertex.size()){
            lineDrawer.MyDrawLineDDA(painter,
                                     curPolyon->vertex[i].getQPoint(),
                                     curPolyon->vertex[i+1].getQPoint()
                                     );
        }
    }
}

void PolygonController::drawUpPolygon(QPainter *painter)
{
    qDebug()<<"drawPolygon"<<endl;
    //return ;
    for(int i=0;i<upStreamPolyon.vertex.size();i++){
        upStreamPolyon.vertex[i].printPointCoodinate();
    }
    for(int i=0;i<upStreamPolyon.vertex.size();i++){
        if((i+1)<upStreamPolyon.vertex.size()){
            lineDrawer.MyDrawLineDDA(painter,
                                     upStreamPolyon.vertex[i].getQPoint(),
                                     upStreamPolyon.vertex[i+1].getQPoint()
                                     );
        }
    }
}

bool PolygonController::changeingVertexs(QMouseEvent *e)
{
    for(int i=0;i<curPolyon->vertex.size();i++){
        if(curPolyon->vertex[i].distanceToPoint(e->pos())<= JUDGE_RIDUS){
            this->indexChange = i;
            printCtrlDebugMessage("当前操作的顶点是");
            qDebug()<<i<<"号顶点"<<endl;
            return true;
        }
        if(curPolyon->startPoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
            this->indexChange = 0;
            printCtrlDebugMessage("当前操作的顶点是0号顶点");
            return true;
        }
    }
    return false;
}

void PolygonController::changeVertexs(Point point)
{
    qDebug() << "changeVertexs("<<endl;
    this->curPolyon->changePoint(indexChange,point);
    //顺序不知道为啥会影响粗细。。
    drawPolygon(painter);
    drawHandle(painter,pen);
}
