#include "CurveController.h"

CurveController::CurveController()
{
    this->curve = nullptr;
    this->painter = nullptr;
    this->setCurve = CURVE_NULL;

    this->isSettingPoints = true;
    this->isFirstDrawing = false;
    t = 0;  //参数

    initTestPoints();
}

bool CurveController::isOperationing(QMouseEvent *e, QPoint &start, QPoint &end)
{
    if(curve != nullptr && isSettingPoints)
    {
        qDebug()<<"CURVE_NEXT"<<endl;
        setCurve = CURVE_NEXT;
        return true;
    }
    else if(changeingVertexs(e)){
        qDebug()<<"CURVE_CHANGE"<<endl;
        setCurve = CURVE_CHANGE;
        return true;
    }
    else if(curve->centerPoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
        qDebug()<<"CURVE_MOVE"<<endl;
        setCurve = CURVE_MOVE;
        return true;
    }
    else if(curve->rotatePoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
        qDebug()<<"CURVE_ROTATE"<<endl;
        setCurve = CURVE_ROTATE;
        return true;
    }

    setCurve = CURVE_NULL;
    *state = UNDO;
    Curve* p = curve;   //防止野指针
    delete p;
    isSettingPoints = true;
    curve = nullptr;
//    //可能有bug
//        this->clearState();
    //
    return false;
}

void CurveController::mousePressEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    printCtrlDebugMessage("CurveController::mousePressEvent");
    if(curve!=nullptr)
        qDebug()<<"curve!=NULL"<<endl;
    else
        qDebug()<<"curve==NULL"<<endl;
    if(e->button()==Qt::LeftButton || e->button()== Qt::RightButton)
    {
        if(curve!=nullptr)
        {
            if(isSettingPoints)
            {
                curve->setNextPoint(Point(e->pos().x(),e->pos().y()));
                this->drawHandle(painter,pen);
                return;
            }
            else if(changeingVertexs(e)){
                qDebug()<<"CURVE_CHANGE"<<endl;
                setCurve = CURVE_CHANGE;
                return;
            }
            else if(curve->centerPoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
                qDebug()<<"CURVE_MOVE"<<endl;
                setCurve = CURVE_MOVE;
                return;
            }
            else if(curve->rotatePoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
                qDebug()<<"CURVE_ROTATE"<<endl;
                setCurve = CURVE_ROTATE;
                return;
            }

            setCurve = CURVE_NULL;
            *state = UNDO;
            Curve* p = curve;   //防止野指针
            delete p;
            curve = nullptr;
            isSettingPoints = true;
            return;
        }
        qDebug()<<"CurveCtrl first press"<<endl;
        QPoint curPoint(e->pos().x(),e->pos().y());
        curve = new Curve();
        curve->setNextPoint(curPoint);
        isSettingPoints = true;
        setCurve = CURVE_NEXT;
        *state = DRAWING;
    }
}

void CurveController::mouseMoveEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    qDebug()<<"CurveController::mouseMoveEvent"<<endl;
    this->painter = painter;
    Point curPoint(e->pos().x(),e->pos().y());
    if (curve == nullptr)
        return;
    switch(setCurve){
        case CURVE_NEXT: ; break;
        case CURVE_CHANGE: this->changeVertexs(curPoint); break;
        case CURVE_MOVE: this->moveToPoint(curPoint);break;
        case CURVE_ROTATE: this->rotateToPoint(curPoint);break;
        default:
            qDebug()<<"Error setCurve"<<endl;
    }
}

void CurveController::mouseReleaseEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    printCtrlDebugMessage("CurveController::mouseReleaseEvent");
    if(isSettingPoints){
        drawHandle(painter,pen);
    }else {
        this->painter = painter;
        drawCurve(painter,pen);
        drawHandle(painter,pen);
    }


}

void CurveController::setStartPoint(Point point)
{

}

void CurveController::setEndPoint(Point point)
{

}

void CurveController::moveToPoint(Point point)
{
    int offsetX = point.getX() - curve->centerPoint.getX();
    int offsetY = point.getY() - curve->centerPoint.getY();
    for(int i=0;i<this->curve->vertex.size();i++){
        this->curve->vertex[i].addX(offsetX);
        this->curve->vertex[i].addY(offsetY);
    }
    curve->getRectangle();
    drawCurve(painter,pen);
    drawHandle(painter,pen);
}

void CurveController::rotateToPoint(Point point)
{
    //先得到与先前旋转点的夹角，用一般方法把所有的点都转了
    double RotaryAngle = getRotaryAngle(curve->centerPoint,curve->rotatePoint,point);   //旋转角
    bool wiseFlag = clockWise(curve->centerPoint,curve->rotatePoint,point);             //是否顺时针
    if(wiseFlag)
        qDebug()<<"顺时针"<<endl;
    else
        qDebug()<<"逆时针"<<endl;

    if(wiseFlag){//顺时针转
        RotaryAngle *= -1;
    }
    //基准点
    int rx0 = curve->centerPoint.getX();
    int ry0 = curve->centerPoint.getY();
    //对每个点进行处理(这里有BUG，但是不知道为什么会有)
    for(int i=0;i<this->curve->vertex.size();i++){
        int x = this->curve->vertex[i].getX();
        int y = this->curve->vertex[i].getY();
        int rotateStartX = (x - rx0)*cos(RotaryAngle) + (y - ry0)*sin(RotaryAngle) + rx0 ;
        int rotateStartY = -(x - rx0)*sin(RotaryAngle) + (y - ry0)*cos(RotaryAngle) + ry0 ;
        this->curve->vertex[i].setPoint(rotateStartX,rotateStartY);
    }
    //对旋转点处理（用内置的旋转角）
    Point up(this->curve->centerPoint.getX(),0); //表示竖直向上的点
    qreal newAngel = getRotaryAngle(this->curve->centerPoint,up,point);//与竖直的夹角
    if(point.getX()<up.getX()){
        newAngel *= -1;
    }
    this->curve->rotateAngle = newAngel;
    //绘制旋转后图形
    this->curve->getRectangleRotating();
    drawCurve(painter,pen);
    drawHandle(painter,pen);
}

void CurveController::setState(DRAW_STATE *state)
{
    this->state = state;
}

void CurveController::drawHandle(QPainter *painter, QPen pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    for(Point i : this->curve->vertex){
        i.DrawCyclePoint(painter,pen);
    }
    QPen debugPen;
    debugPen.setStyle(Qt::DashLine);
    debugPen.setWidth(1);
    debugPen.setColor(pen.color());
    painter->setPen(debugPen);

    for(int i=0;i<curve->vertex.size()-1;i++){
        painter->drawLine(curve->vertex[i].getX(),curve->vertex[i].getY(),
                          curve->vertex[i+1].getX(),curve->vertex[i+1].getY());
    }

    painter->setPen(pen);

    curve->centerPoint.DrawWarnPoint(painter,pen);
    curve->rotatePoint.DrawCyclePoint(painter,pen);
}

void CurveController::clearState()
{
    this->curve = nullptr;
    this->painter = nullptr;
    this->setCurve = CURVE_NULL;

    this->isSettingPoints = true;
    this->isFirstDrawing = false;
    t = 0;  //参数
    this->ctrlPoints.clear();
    this->bezierNodes.clear();
}

void CurveController::getStartAndEnd(QPoint &start, QPoint &end)
{
    //已经失去这个函数本身的意义了
}

void CurveController::setBigger(QPainter *painter, QMouseEvent *e, QPen pen)
{
    for(int i=0;i<this->curve->vertex.size();i++){
        this->curve->vertex[i].mulX(ZOOM_IN);
        this->curve->vertex[i].mulY(ZOOM_IN);
    }
    curve->getRectangle();
    drawCurve(painter,pen);
    drawHandle(painter,pen);
}

void CurveController::setSmaller(QPainter *painter, QMouseEvent *e, QPen pen)
{
    for(int i=0;i<this->curve->vertex.size();i++){
        this->curve->vertex[i].mulX(ZOOM_OUT);
        this->curve->vertex[i].mulY(ZOOM_OUT);
    }
    curve->getRectangle();
    drawCurve(painter,pen);
    drawHandle(painter,pen);
}

void CurveController::drawCurve(QPainter *painter, QPen pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行
    this->painter = painter;
    //清空控制点数组
    ctrlPoints.clear();
    //转化为浮点数组
    for(Point v : curve->vertex){
        ctrlPoints.push_back(PointD(v.getX(),v.getY()));
        v.DrawCyclePoint(painter,pen);
    }

    drawBezier(painter,pen);
    this->isSettingPoints = false;
}

void CurveController::drawFirstMovingCurve(QPainter *painter, QPen pen)
{
    if(this->curve == nullptr){
        return;
    }

    //绘制后不可再次绘制
    if(isSettingPoints == false){
        return;
    }

    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行

    this->painter = painter;

    //只有第一次画才会进入
    if(isFirstDrawing==false){
        this->isFirstDrawing = true;
        this->t=0;
        //清空控制点数组
        ctrlPoints.clear();
        //转化为浮点数组
        for(Point v : curve->vertex){
            ctrlPoints.push_back(PointD(v.getX(),v.getY()));
            v.DrawCyclePoint(painter,pen);
        }
        this->bezierNodes.clear();
    }

    //drawBezier
    drawnode(ctrlPoints,painter,pen);
    qDebug()<<"moving t="<<t;
    if(t<1){
        t = t+0.01;
    }
}

void CurveController::drawBezier(QPainter *painter, QPen pen)
{
    ///BUG mayber!!!!
    this->isSettingPoints=false;
    this->bezierNodes.clear();
    for(t=0;t<1;t=t+0.01){
        drawnode(ctrlPoints,painter,pen);
    }
    //辅助矩形
    if(this->curve != nullptr){
        if(this->setCurve != CURVE_ROTATE){
            printCtrlDebugMessage("this->curve->getRectangle();");
            this->curve->getRectangle();
            this->drawOutlineToDebug(painter,curve->centerPoint.getQPoint(),curve->LeftUp.getQPoint());
        }else{
            printCtrlDebugMessage("this->curve->getRectangleRotating();");
            this->curve->getRectangleRotating();
            this->drawOutlineToDebug(painter,   curve->LeftUp.getQPoint(),
                                                curve->RightUp.getQPoint(),
                                                curve->RightDown.getQPoint(),
                                                curve->LeftDown.getQPoint());
        }
    }

}

void CurveController::drawnode(QVector<PointD> &nodes,QPainter *painter, QPen pen)
{
    if(nodes.empty()){
        return;
    }
    QVector<PointD> _nodes; //复制一个数组_nodes
    for(int i=0;i<nodes.size();i++){
        _nodes.push_back(nodes[i]);
    }
    QVector<PointD> nextNodes;
    for(int i=0;i<_nodes.size();i++){
        int startX = _nodes[i].getX();
        int startY = _nodes[i].getY();

        //绘制动态
        if(this->isFirstDrawing && this->isSettingPoints && this->curve->vertex.size()<MAX_ANIME_POINTS){

            Point movePoint(startX,startY);
            if(_nodes.size()==1){
                movePoint.DrawColorPoint(painter,pen);
            }else{
                movePoint.DrawMovingPoint(painter,pen);
            }
        }

        if(_nodes.length() == 1){
            bezierNodes.push_back(_nodes[i]);
            if(bezierNodes.size()>1){
                for(int j=1;j<bezierNodes.size();j++){
                    startX = bezierNodes[j-1].getX();
                    startY = bezierNodes[j-1].getY();
                    int x = bezierNodes[j].getX();
                    int y = bezierNodes[j].getY();
                    painter->drawLine(startX,startY,x,y);

                }
            }
        }

        //绘制动态
        if(i>0 && this->isFirstDrawing && this->isSettingPoints && this->curve->vertex.size()<MAX_ANIME_POINTS){
            printCtrlDebugMessage("draw moving");
            int targetX = _nodes[i-1].getX();
            int targetY = _nodes[i-1].getY();
            painter->drawLine(targetX,targetY,startX,startY);
        }
    }

    if(_nodes.size()>=0){
        for(int i=0;i<_nodes.length()-1;i++){
            nextNodes.push_back(getBezierPoint(_nodes[i],_nodes[i+1],t));
        }
        drawnode(nextNodes,painter,pen);
    }
}

PointD CurveController::getBezierPoint(PointD a, PointD b,double t)
{
    double x = 0;
    double y = 0;
    int n = 1;
    QVector<PointD> res;
    res.push_back(a);
    res.push_back(b);
    for(int index=0;index<res.length();index++){
        if(index==0){
            x += res[index].getX() * pow(( 1 - t ), n - index) * pow(t, index);
            y += res[index].getY() * pow(( 1 - t ), n - index) * pow(t, index);
        }else{
            x += fact(n) / fact(index) / fact(n - index) * res[index].getX() * pow(( 1 - t ), n - index) * pow(t, index);
            y += fact(n) / fact(index) / fact(n - index) * res[index].getY() * pow(( 1 - t ), n - index) * pow(t, index);
        }
    }

    return PointD(x,y);
}

int CurveController::fact(int n)
{
    if(n<=1){
        return 1;
    }else{
        return n*fact(n-1);
    }
}

void CurveController::initTestPoints()
{
    PointD a(2*100,2*100);
    PointD a2(2*150,2*150);
    PointD b(2*200,2*300);
    PointD b2(2*250,2*350);
    PointD c(2*300,2*390);
    PointD c2(2*350,2*300);
    PointD d(2*400,2*200);
    PointD d2(2*440,2*100);
    ctrlPoints.push_back(a);
    ctrlPoints.push_back(a2);
    ctrlPoints.push_back(b);
    ctrlPoints.push_back(b2);
    ctrlPoints.push_back(c);
    ctrlPoints.push_back(c2);
    ctrlPoints.push_back(d);
    ctrlPoints.push_back(d2);
}

bool CurveController::changeingVertexs(QMouseEvent *e)
{
    for(int i=0;i<curve->vertex.size();i++){
        if(curve->vertex[i].distanceToPoint(e->pos())<= JUDGE_RIDUS){
            this->indexChange = i;
            printCtrlDebugMessage("当前操作的顶点是");
            qDebug()<<i<<"号顶点"<<endl;
            return true;
        }
        if(curve->startPoint.distanceToPoint(e->pos())<= JUDGE_RIDUS){
            this->indexChange = 0;
            printCtrlDebugMessage("当前操作的顶点是0号顶点");
            return true;
        }
    }
    return false;}

bool CurveController::closeSettingPoints()
{
    this->isSettingPoints = false;
}

bool CurveController::getIsSettingPoints()
{
    return this->isSettingPoints;
}

void CurveController::changeVertexs(Point point)
{
    this->t = 0;
    this->bezierNodes.clear();
    this->curve->changePoint(indexChange,point);

    //顺序不知道为啥会影响粗细。。
    drawCurve(painter,pen);
    drawHandle(painter,pen);
}

bool CurveController::isNullCurve()
{
    return (this->curve==nullptr);
}


double PointD::getY() const
{
    return y;
}

double PointD::getX() const
{
    return x;
}
