#include "CurveController.h"

CurveController::CurveController()
{
    t = 0;
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

void CurveController::drawCurve(QPainter *painter, QPen pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行
    for(PointD v : ctrlPoints){
        Point t(v.getX(),v.getY());
        t.DrawCyclePoint(painter,pen);
    }
    for(int i=0;i<ctrlPoints.size()-1;i++){
        painter->drawLine(ctrlPoints[i].getX(),ctrlPoints[i].getY(),
                          ctrlPoints[i+1].getX(),ctrlPoints[i+1].getY());
    }
    drawBezier(painter,pen);
}

void CurveController::drawBezier(QPainter *painter, QPen pen)
{
    for(t=0;t<1;t=t+0.01){
        drawnode(ctrlPoints,painter,pen);
    }
}

void CurveController::drawnode(QVector<PointD> &nodes,QPainter *painter, QPen pen)
{
    qDebug()<<"drawnode 0:"<<t<<endl;
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
};

double PointD::getY() const
{
    return y;
}

double PointD::getX() const
{
    return x;
}
