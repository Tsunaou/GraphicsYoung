#include "Line.h"

Line::Line()
{
    this->start = NULL;
    this->end = NULL;
    this->type = LINE;
}

Line::Line(QPoint *start, QPoint *end, FIGURE_TYPE type)
{
    this->start = start;
    this->end = end;
    this->type = LINE;
}
