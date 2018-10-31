#ifndef LINE_H
#define LINE_H
#include "Figure.h"

class Line:public Figure
{
public:
    Line();
    Line(QPoint*start,QPoint*end,FIGURE_TYPE type);
};

#endif // LINE_H
