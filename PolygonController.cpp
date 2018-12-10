#include "PolygonController.h"

PolygonController::PolygonController()
{
    this->curPolyon = nullptr;
    this->painter = nullptr;
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
    else if(curPolyon->centerPoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
        qDebug()<<"POLYGON_MOVE"<<endl;
        setPolygon = POLYGON_MOVE;
        return true;
    }
    else if(curPolyon->rotatePoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
        qDebug()<<"POLYGON_ROTATE"<<endl;
        setPolygon = POLYGON_ROTATE;
        return true;
    }

    setPolygon = POLYGON_NULL;
    *state = UNDO;
    start = curPolyon->startPoint.getQPoint(); //将多边形信息存储下来
    end = curPolyon->endPoint.getQPoint();    //将最终绘制的多边形信息存储下来
    this->upStreamPolyon = *curPolyon;
    Polygon* p = curPolyon;   //防止野指针
    delete p;
    curPolyon = nullptr;
    return false;
}

void PolygonController::mousePressEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"PolygonController::mousePressEvent"<<endl;
    if(curPolyon!=nullptr){
        qDebug()<<"curPolyon!=NULL"<<endl;
    }else{
        qDebug()<<"curPolyon==NULL"<<endl;
    }
    if(e->button()==Qt::LeftButton || e->button()== Qt::RightButton)
    {
        if(curPolyon!=nullptr)
        {
            bool returnFlag = false; //暂时没有什么用
            if((curPolyon->colseFlag==false && curPolyon->startPoint.distanceToPoint(e->pos())<=25)
                    || e->button() == Qt::RightButton)
            {
                qDebug()<<"POLYGON_START The Edges Limits"<<endl;
                setPolygon = POLYGON_START;
                returnFlag = true;
                curPolyon->colseFlag = true;
                Point curPoint = curPolyon->startPoint;
                this->curPolyon->setNextPoint(curPoint);
                this->curPolyon->getRectangle();
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
            else if(curPolyon->centerPoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
                qDebug()<<"POLYGON_MOVE"<<endl;
                setPolygon = POLYGON_MOVE;
                return;
            }
            else if(curPolyon->rotatePoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
                qDebug()<<"POLYGON_ROTATE"<<endl;
                setPolygon = POLYGON_ROTATE;
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
            curPolyon = nullptr;
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
    if (curPolyon == nullptr)
        return;
    switch(setPolygon){
        case POLYGON_START: this->setStartPoint(curPoint); break;
        case POLYGON_NEXT: this->changeNextPoints(curPoint); break;
        case POLYGON_CHANGE: this->changeVertexs(curPoint); break;
        case POLYGON_MOVE: this->moveToPoint(curPoint);break;
        case POLYGON_ROTATE: this->rotateToPoint(curPoint);break;
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
        this->curPolyon->getRectangle();
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
    int offsetX = point.getX() - curPolyon->centerPoint.getX();
    int offsetY = point.getY() - curPolyon->centerPoint.getY();
    for(int i=0;i<this->curPolyon->vertex.size();i++){
        this->curPolyon->vertex[i].addX(offsetX);
        this->curPolyon->vertex[i].addY(offsetY);
    }
    curPolyon->getRectangle();
    drawPolygon(painter);
    drawHandle(painter,pen);
}

void PolygonController::rotateToPoint(Point point)
{
    //先得到与先前旋转点的夹角，用一般方法把所有的点都转了
    double RotaryAngle = getRotaryAngle(curPolyon->centerPoint,curPolyon->rotatePoint,point);   //旋转角
    bool wiseFlag = clockWise(curPolyon->centerPoint,curPolyon->rotatePoint,point);             //是否顺时针
    if(wiseFlag)
        qDebug()<<"顺时针"<<endl;
    else
        qDebug()<<"逆时针"<<endl;

    if(wiseFlag){//顺时针转
        RotaryAngle *= -1;
    }
    //基准点
    int rx0 = curPolyon->centerPoint.getX();
    int ry0 = curPolyon->centerPoint.getY();
    //对每个点进行处理(这里有BUG，但是不知道为什么会有)
    for(int i=0;i<this->curPolyon->vertex.size();i++){
        int x = this->curPolyon->vertex[i].getX();
        int y = this->curPolyon->vertex[i].getY();
        int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 ;
        int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 ;
        this->curPolyon->vertex[i].setPoint(rotateStartX,rotateStartY);
    }
    //对旋转点处理（用内置的旋转角）
    Point up(this->curPolyon->centerPoint.getX(),0); //表示竖直向上的点
    qreal newAngel = getRotaryAngle(this->curPolyon->centerPoint,up,point);//与竖直的夹角
    if(point.getX()<up.getX()){
        newAngel *= -1;
    }
    this->curPolyon->rotateAngle = newAngel;
    //绘制旋转后图形
    drawPolygon(painter);
    drawHandle(painter,pen);

}

void PolygonController::setState(DRAW_STATE *state)
{
    this->state = state;
}

void PolygonController::drawHandle(QPainter *painter, QPen pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    for(Point i : this->curPolyon->vertex){
        i.DrawCyclePoint(painter,pen);
    }
    curPolyon->vertex.first().DrawWarnPoint(painter,pen);
    curPolyon->centerPoint.DrawWarnPoint(painter,pen);
    curPolyon->rotatePoint.DrawCyclePoint(painter,pen);
    lineDrawer.MyDrawLineDDA(painter,curPolyon->centerPoint.getQPoint(),curPolyon->rotatePoint.getQPoint());
}

void PolygonController::clearState()
{
    this->curPolyon = nullptr;
    this->painter = nullptr;
    this->setPolygon = POLYGON_NULL;
    this->firstEdgeFlag = false;
    //*this->state = UNDO;
}

void PolygonController::getStartAndEnd(QPoint &start, QPoint &end)
{
    this->upStreamPolyon = *curPolyon;
}

void PolygonController::setBigger(QPainter *painter, QMouseEvent *e, QPen pen)
{
    for(int i=0;i<this->curPolyon->vertex.size();i++){
        this->curPolyon->vertex[i].mulX(ZOOM_IN);
        this->curPolyon->vertex[i].mulY(ZOOM_IN);
    }
    curPolyon->getRectangle();
    drawPolygon(painter);
    drawHandle(painter,pen);
}

void PolygonController::setSmaller(QPainter *painter, QMouseEvent *e, QPen pen)
{
    for(int i=0;i<this->curPolyon->vertex.size();i++){
        this->curPolyon->vertex[i].mulX(ZOOM_OUT);
        this->curPolyon->vertex[i].mulY(ZOOM_OUT);
    }
    curPolyon->getRectangle();
    drawPolygon(painter);
    drawHandle(painter,pen);
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
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    if(this->setPolygon != POLYGON_ROTATE){
        this->curPolyon->getRectangle();
        this->drawOutlineToDebug(painter,curPolyon->centerPoint.getQPoint(),curPolyon->LeftUp.getQPoint());
    }else{
        this->curPolyon->getRectangleRotating();
        this->drawOutlineToDebug(painter,   curPolyon->LeftUp.getQPoint(),
                                            curPolyon->RightUp.getQPoint(),
                                            curPolyon->RightDown.getQPoint(),
                                            curPolyon->LeftDown.getQPoint());
    }
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
