#include "CurveController.h"

CurveController::CurveController()
{

}

void CurveController::drawCurve(QPainter *painter, QPen pen)
{
    if(!painter->isActive()) {return;}//保证在Painter有效的时候才进行
    Point p(6,6);
    p.DrawCyclePoint(painter,pen);
}
