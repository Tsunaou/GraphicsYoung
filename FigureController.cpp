#include "FigureController.h"

FigureController::FigureController()
{
    painter = NULL;
}

double FigureController::getRotaryAngle(Point center, Point a, Point b)
{
    double ab = a.distanceToPoint(b.getQPoint());
    double ac = a.distanceToPoint(center.getQPoint());
    double bc = b.distanceToPoint(center.getQPoint());
    qreal cosC = (bc*bc+ac*ac-ab*ab)/(2*bc*ac);
    double theta = qAcos(cosC);
    qDebug()<<"Angle is"<<theta<<endl;
    return theta; //弧度制
}

bool FigureController::clockWise(Point center, Point a, Point b)
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

double FigureController::getLength(QPoint &start, QPoint &end)
{
    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();
    return sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
}

void FigureController::drawOutlineToDebug(QPainter *painter, QPoint &start, QPoint &end)
{
    QPen savePen = painter->pen();

    QPen debugPen;
    debugPen.setStyle(Qt::DashLine);
    debugPen.setWidth(1);
    debugPen.setColor(Qt::black);

    painter->setPen(debugPen);

    int x0 = start.x();
    int y0 = start.y();
    int x = end.x()-x0;
    int y = end.y()-y0;

    QPoint temPt1(x0+x,y0+y);
    QPoint temPt2(x0+x,y0-y);
    QPoint temPt3(x0-x,y0+y);
    QPoint temPt4(x0-x,y0-y);

    painter->drawLine(temPt1,temPt2);
    painter->drawLine(temPt1,temPt3);
    painter->drawLine(temPt4,temPt2);
    painter->drawLine(temPt3,temPt4);

    painter->setPen(savePen);
}

void FigureController::drawOutlineToDebug(QPainter *painter, QPoint a, QPoint b, QPoint c, QPoint d)
{
    QPen savePen = painter->pen();

    QPen debugPen;
    debugPen.setStyle(Qt::DashLine);
    debugPen.setWidth(1);
    debugPen.setColor(Qt::black);

    painter->setPen(debugPen);

    painter->drawLine(a,b);
    painter->drawLine(b,c);
    painter->drawLine(c,d);
    painter->drawLine(d,a);

    painter->setPen(savePen);
}
