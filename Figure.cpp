#include "Figure.h"

Figure::Figure()
{
    this->start = NULL;
    this->end = NULL;
    this->type = FIGURE;
}

Figure::Figure(QPoint *start, QPoint *end)
{
    this->start = start;
    this->end = end;
    this->type = FIGURE;
}




