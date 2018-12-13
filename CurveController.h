#ifndef CURVECONTROLLER_H
#define CURVECONTROLLER_H

#include "FigureController.h"

class CurveController
{
public:
    CurveController();
    void drawCurve(QPainter* painter, QPen pen);
};

#endif // CURVECONTROLLER_H
