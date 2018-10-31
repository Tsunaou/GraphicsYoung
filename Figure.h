#ifndef FIGURE_H
#define FIGURE_H

#include "constParams.h"
#include <QPoint>
class Figure
{
public:
    Figure();
    Figure(QPoint* start,QPoint* end);
    QPoint* start;
    QPoint* end;
    FIGURE_TYPE type;
};

#endif // FIGURE_H
