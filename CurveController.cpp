#include "CurveController.h"

CurveController::CurveController()
{
    this->curve = nullptr;
    this->painter = nullptr;
    this->setCurve = CURVE_NULL;

    this->isSettingPoints = true;
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
    printCtrlDebugMessage("CurveController::mouseMoveEvent");
    this->drawHandle(painter,pen);
}

void CurveController::mouseReleaseEvent(QPainter *painter, QMouseEvent *e, QPen pen)
{
    printCtrlDebugMessage("CurveController::mouseReleaseEvent");
    this->drawHandle(painter,pen);
}

void CurveController::setStartPoint(Point point)
{

}

void CurveController::setEndPoint(Point point)
{

}

void CurveController::moveToPoint(Point point)
{

}

void CurveController::rotateToPoint(Point point)
{

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
    debugPen.setColor(Qt::black);
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
    t = 0;  //参数
    this->ctrlPoints.clear();
    this->bezierNodes.clear();
}

void CurveController::getStartAndEnd(QPoint &start, QPoint &end)
{

}

void CurveController::setBigger(QPainter *painter, QMouseEvent *e, QPen pen)
{

}

void CurveController::setSmaller(QPainter *painter, QMouseEvent *e, QPen pen)
{

}

void CurveController::drawCurve(QPainter *painter, QPen pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行
    //清空控制点数组
    ctrlPoints.clear();
    //转化为浮点数组
    for(Point v : curve->vertex){
        ctrlPoints.push_back(PointD(v.getX(),v.getY()));
        v.DrawCyclePoint(painter,pen);
    }

    drawBezier(painter,pen);
}

void CurveController::drawBezier(QPainter *painter, QPen pen)
{
    this->bezierNodes.clear();
    for(t=0;t<1;t=t+0.01){
        drawnode(ctrlPoints,painter,pen);
    }
    this->isSettingPoints = false;
}

void CurveController::drawnode(QVector<PointD> &nodes,QPainter *painter, QPen pen)
{
    //qDebug()<<"drawnode 0:"<<t<<endl;
    if(nodes.empty()){
        return;
    }
    QVector<PointD> _nodes; //复制一个数组_nodes
    for(int i=0;i<nodes.size();i++){
        _nodes.push_back(nodes[i]);
    }
//    qDebug()<<"drawnode 1:"<<t<<endl;
    QVector<PointD> nextNodes;
//    qDebug()<<"drawnode 1:"<<t<<"_nodes.length="<<_nodes.length()<<endl;
    for(int i=0;i<_nodes.size();i++){
        if(_nodes.length() == 1){
            bezierNodes.push_back(_nodes[i]);
//            qDebug()<<"drawnode 1:"<<t<<"bezierNodes.length="<<bezierNodes.length()<<endl;
            if(bezierNodes.size()>1){
                for(int j=1;j<bezierNodes.size();j++){
                    int startX = bezierNodes[j-1].getX();
                    int startY = bezierNodes[j-1].getY();
                    int x = bezierNodes[j].getX();
                    int y = bezierNodes[j].getY();
//                    qDebug()<<"drawnode drawLine:"<<t<<endl;
                    painter->drawLine(startX,startY,x,y);

                }
            }
        }
    }
//    qDebug()<<"drawnode 2:"<<t<<endl;
    if(_nodes.size()>=0){
        for(int i=0;i<_nodes.length()-1;i++){
            nextNodes.push_back(getBezierPoint(_nodes[i],_nodes[i+1],t));
        }
//        qDebug()<<"drawnode 3:"<<t<<endl;
        drawnode(nextNodes,painter,pen);
    }
}

PointD CurveController::getBezierPoint(PointD a, PointD b,double t)
{
//    qDebug()<<"getBezierPoint:"<<t<<endl;
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
    return false;
}

bool CurveController::closeSettingPoints()
{
    this->isSettingPoints = false;
}

bool CurveController::getIsSettingPoints()
{
    return this->isSettingPoints;
}


double PointD::getY() const
{
    return y;
}

double PointD::getX() const
{
    return x;
}
