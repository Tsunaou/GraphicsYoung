#include "Cycle.h"

Cycle::Cycle()
{
    this->start = NULL;
    this->end = NULL;
    this->type = CYCLE;
}

Cycle::Cycle(QPoint *start, QPoint *end, FIGURE_TYPE type)
{
    this->start = start;
    this->end = end;
    this->type = CYCLE;
}
