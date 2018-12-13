#include "CurveController.h"

CurveController::CurveController()
{
    t = 0;
    Point a(100,100);
    Point a2(150,150);
    Point b(200,300);
    Point b2(250,350);
    Point c(300,390);
    Point c2(350,300);
    Point d(400,200);
    Point d2(440,100);
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
    for(Point v : ctrlPoints){
        v.DrawCyclePoint(painter,pen);
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

void CurveController::drawnode(QVector<Point> &nodes,QPainter *painter, QPen pen)
{
    qDebug()<<"drawnode 0:"<<t<<endl;
    if(nodes.empty()){
        return;
    }
    QVector<Point> _nodes; //复制一个数组_nodes
    for(int i=0;i<nodes.size();i++){
        _nodes.push_back(nodes[i]);
    }
//    qDebug()<<"drawnode 1:"<<t<<endl;
    QVector<Point> nextNodes;
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

Point CurveController::getBezierPoint(Point a, Point b,double t)
{
//    qDebug()<<"getBezierPoint:"<<t<<endl;
    int x = 0;
    int y = 0;
    int n = 1;
    QVector<Point> res;
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

    return Point(x,y);
}

int CurveController::fact(int n)
{
    if(n<=1){
        return 1;
    }else{
        return n*fact(n-1);
    }
};
