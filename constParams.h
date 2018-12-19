#ifndef CONSTPARAMS_H
#define CONSTPARAMS_H
//#include <QDebug>
//#include <QString>
#include <cmath>

enum FIGURE_TYPE{FIGURE,LINE,CYCLE,ELLIPSE,POLYGON,CURVE,PEN,BRUSH,FILLER};    //绘图模式（图形类别）
enum DRAW_STATE{DRAWING,UNDO};  //绘画状态
enum SETLINE{SETBEGIN, SETEND, SETCENTER, SETHANDLE, SETNULL};  //对直线的操作
enum SETCYCLE{CYCLE_CENTER,CYCLE_OUT,CYCLE_HANDLE,CYCLE_NULL};  //对圆的操作
enum SETELLIPSE{ELLIPSE_CENTER,ELLIPSE_OUT,ELLIPSE_HANDLE,ELLIPSE_NULL};    //对椭圆的操作
enum SETPOLYGON{POLYGON_START,POLYGON_NEXT,POLYGON_CHANGE,POLYGON_NULL,POLYGON_MOVE,POLYGON_ROTATE};    //对多边形的操作
enum SETCURVE{CURVE_NEXT,CURVE_CHANGE,CURVE_MOVE,CURVE_ROTATE,CURVE_NULL};    //对曲线的操作
enum CUTTER{CUTTING,UNCUT};

const int BRUSH_WIDTH = 10; //刷子宽度
const int PEN_WIDTH = 1;    //笔宽，为了像素级实现定为1
const double ZOOM_IN = 1.25;        //缩放比例
const double ZOOM_OUT = 1/ZOOM_IN;  //缩放比例
const double JUDGE_RIDUS = 5;       //判断是否是一个点的基准
const double ROTATE_RIDUS = 30;     //旋转点距离中心的距离（多边形和椭圆可能用到）
const double DEFAULT_ANGLE = 0;     //旋转点和竖直向上的夹角
const int ROTATE_ACCURACY = 5;      //旋转的精度（主要用于圆旋转的校正）
const int MAX_ANIME_POINTS = 15;    //最大展示动画的控制点数

#endif // CONSTPARAMS_H
