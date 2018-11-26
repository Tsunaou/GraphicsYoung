#include "PolygonController.h"

PolygonController::PolygonController()
{
    painter = NULL;
}

bool PolygonController::isOperationing(QMouseEvent *e, QPoint &start, QPoint &end)
{
    if(curPolyon->startPoint.distanceToPoint(e->pos())<=5)
    {
        qDebug()<<"POLYGON_START"<<endl;
        setPolygon = POLYGON_START;
        return true;
    }
//    else if(curPolyon->colseFlag==false && *state!=UNDO){ //暂时苟且一下
//        qDebug()<<"POLYGON_NEXT"<<endl;
//        setPolygon = POLYGON_NEXT;
//        return true;
//    }

    setPolygon = POLYGON_NULL;
    *state = UNDO;
    start = curPolyon->startPoint.getQPoint(); //将多边形信息存储下来
    end = curPolyon->endPoint.getQPoint();    //将最终绘制的多边形信息存储下来
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
            if(curPolyon->startPoint.distanceToPoint(e->pos())<=5)
            {
                qDebug()<<"POLYGON_START"<<endl;
                setPolygon = POLYGON_START;
                return;
            }
//            else if(curPolyon->colseFlag==false && *state!=UNDO){ //暂时苟且一下
//                qDebug()<<"POLYGON_NEXT"<<endl;
//                setPolygon = POLYGON_NEXT;
//                return;
//            }

            setPolygon = POLYGON_NULL;
            *state = UNDO;
            Polygon* p = curPolyon;   //防止野指针
            delete p;
            curPolyon = NULL;
            return;
        }
        QPoint curPoint(e->pos().x(),e->pos().y());
        curPolyon = new Polygon(&curPoint,&curPoint,POLYGON);
        setPolygon = POLYGON_NEXT;
        *state = DRAWING;
    }
}

void PolygonController::mouseMoveEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"PolygonController::mouseMoveEvent"<<endl;
    this->painter = painter;
    printCtrlDebugMessage("mouseMoveEvent 1");
    Point curPoint(e->pos().x(),e->pos().y());
    printCtrlDebugMessage("mouseMoveEvent 2");
    if (curPolyon == NULL)
        return;
    switch(setPolygon){
        case POLYGON_START: this->setStartPoint(curPoint); break;
        case POLYGON_NEXT: this->setNextPoints(curPoint); break;
        case POLYGON_CHANGE: this->rotateToPoint(curPoint); break;
        default:
            qDebug()<<"Error setPolygon"<<endl;
    }
    printCtrlDebugMessage("mouseMoveEvent 3");
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
    drawPolygon(painter);
    drawHandle(painter,pen);
}

void PolygonController::setStartPoint(Point point)
{

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

}

void PolygonController::drawHandle(QPainter *painter, QPen pen)
{

}

void PolygonController::clearState()
{

}

void PolygonController::getStartAndEnd(QPoint &start, QPoint &end)
{

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

void PolygonController::drawPolygon(QPainter* painter)
{
    qDebug()<<"drawPolygon"<<endl;
    return ;
    for(int i=0;i<curPolyon->vertex.size();i++){
        if((i+1)<curPolyon->vertex.size()){
            lineDrawer.MyDrawLineDDA(painter,
                                     curPolyon->vertex[i].getQPoint(),
                                     curPolyon->vertex[i+1].getQPoint()
                                     );
        }
    }
}
