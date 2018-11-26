#include "Polygon.h"

Polygon::Polygon()
{
    this->start = NULL;
    this->end = NULL;
    this->type = POLYGON;
    this->colseFlag = false;
}

Polygon::Polygon(QPoint *start, QPoint *end, FIGURE_TYPE type)
{
    this->start = start;
    this->end = end;
    this->type = POLYGON;
    this->vertex.push_back(Point(start->x(),start->y()));   //起始点进去
    this->colseFlag = false;

    //对于起始点的处理
    this->startPoint = Point(*start);
}

void Polygon::setNextPoint(Point p)
{
    vertex.push_back(p);
}

void Polygon::changeNextPoint(Point p)
{
    vertex[vertex.size()-1] = p;
}

void Polygon::changePoint(int index, Point p)
{
    if(index<0 || index >= vertex.size()){
        qDebug()<<"Change Points in polygon error"<<endl;
    }else{
        if(index == 0){
            this->vertex[0] = p;
            this->vertex[this->vertex.size()-1] = p;
        }else{
            this->vertex[index] = p;
        }
    }
}



void Polygon::getRectangle()
{
    int maxX = vertex.first().getX();
    int minX = vertex.first().getX();
    int maxY = vertex.first().getY();
    int minY = vertex.first().getY();
    for(Point v: vertex){
        int vx = v.getX();
        int vy = v.getY();
        if(vx>maxX){
            maxX = vx;
        }
        if(vx<minX){
            minX = vx;
        }
        if(vy>maxY){
            maxY = vy;
        }
        if(vy<minY){
            minY = vy;
        }
    }
    LeftUp.setPoint(minX,minY);   //左上角（minX,minY）
    LeftDown.setPoint(minX,maxY); //左下角（minX,maxY）
    RightUp.setPoint(maxX,minY);  //右上角（maxX,minY）
    RightDown.setPoint(maxX,maxY);//右下角（maxX,maxY）
    centerPoint.setPoint((minX+maxX)/2,(minY+maxY)/2);
}
