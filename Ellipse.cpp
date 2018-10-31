#include "Ellipse.h"

Ellipse::Ellipse()
{
    this->start = NULL;
    this->end = NULL;
    this->type = ELLIPSE;
}

Ellipse::Ellipse(QPoint *start, QPoint *end)
{
    this->start = start;
    this->end = end;
    this->type = ELLIPSE;
}
