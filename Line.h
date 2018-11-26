#ifndef LINE_H
#define LINE_H
#include "Figure.h"

class Line:public Figure
{
public:
    Line();
    Line(QPoint*start,QPoint*end,FIGURE_TYPE type);
    void setStartPoint(Point p);
    void setEndPoint(Point p);
    void setLength();
    double getLength();
private:
    qreal length;
};

#endif // LINE_H
