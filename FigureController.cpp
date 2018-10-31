#include "FigureController.h"

FigureController::FigureController()
{

}

double FigureController::getLength(QPoint &start, QPoint &end)
{
    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();
    return sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
}

void FigureController::drawOutlineToDebug(QPainter *painter, QPoint &start, QPoint &end)
{
    QPen debugPen;
    debugPen.setStyle(Qt::DashLine);
    debugPen.setWidth(1);
    debugPen.setColor(Qt::black);
    QPen savePen;
    savePen.setStyle(Qt::SolidLine);
    savePen.setWidth(1);
    savePen.setColor(Qt::black);

    painter->setPen(debugPen);

    int x0 = start.x();
    int y0 = start.y();
    int x = end.x()-x0;
    int y = end.y()-y0;

    QPoint temPt1(x0+x,y0+y);
    QPoint temPt2(x0+x,y0-y);
    QPoint temPt3(x0-x,y0+y);
    QPoint temPt4(x0-x,y0-y);

    painter->drawLine(temPt1,temPt2);
    painter->drawLine(temPt1,temPt3);
    painter->drawLine(temPt4,temPt2);
    painter->drawLine(temPt3,temPt4);

    painter->setPen(savePen);
}
