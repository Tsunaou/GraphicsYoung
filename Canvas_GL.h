#ifndef CANVAS_GL_H
#define CANVAS_GL_H

#include "constParams.h"
#include "Line.h"
#include "Cycle.h"
#include "Ellipse.h"
#include "LineController.h"
#include "CycleController.h"
#include "EllipseController.h"
#include "PolygonController.h"
#include "CurveController.h"
#include "Point.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QtDebug>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPixmap>
#include <QImage>
#include <QPoint>
#include <QPainter>
#include <QPalette>
#include <QStack>

#include <QFileDialog>
#include <QMessageBox>



class Canvas_GL : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Canvas_GL(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;    //鼠标点击时操作
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;     //鼠标移动时操作（先点击为前提）
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;  //鼠标释放时操作
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    void saveImage(); //保存当前画布内容（pix内容）
    void resizeImage(QImage *image, const QSize &newSize);
    bool openImage(const QString &fileName);
    void clearImage(); //清空当前画布内容
    void recallImage();//撤销上一步操作
    void clearStates();//清空控制器状态
    void setMode(FIGURE_TYPE type); //设置图形
    void setCutMode(CUTTER mode);   //设置裁剪状态
    void setBigger();   //放大图形
    void setSmaller();  //缩小图形
    void fillColor(QImage* img, QColor backcolor,QPainter *painter,int cx,int cy);   //填充算法
    QPixmap *getPixCopy();              //得到当前画布的拷贝
    QPixmap *getPixCopy(QPixmap* pic);  //得到指定pic的拷贝
    //一些辅助调试的函数
    void showDrawingStates();   //输出状态信息
    void drawBeforeNewState();  //进入另一个状态前绘图
    void printDebugMessage(QString msg){
        qDebug()<<msg<<endl;
    }
    bool isDrawingFigure();     //判断是否在绘画图形状态

    void drawCurve();   //画曲线，测试
    void animeDraw();
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;


signals:

public slots:
    void setColor(QColor);  //槽函数，用于实现画笔颜色的改变

private:
    QPixmap *pix;       //画布
    QPixmap *pixToMove; //用于实现图形随着鼠标动态移动的画布缓冲区
    QPixmap imageOpen;
    QPen pen;           //画笔，默认颜色为黑色，大小为1像素，样式为solid，实现像素级绘图

    QPoint startPos;    //捕捉画图的起始点
    QPoint endPos;      //捕捉画图的终止点
    QPoint regPos;      //缓冲点
    //裁剪用
    QPoint cutStart;
    QPoint cutEnd;
    //------
    int style;          //画笔样式
    int weight;         //画笔粗细
    QColor color;       //画笔颜色

    //QPainter *painter;
    QVector<QPixmap*> reVec;    //存储画布内容，用于实现撤销操作
    QVector<Figure*> figureVec; //存储当前画布的图形
    LineController lineController;      //控制直线的行为
    CycleController cycleController;    //控制圆的行为
    EllipseController ellipseController;//控制椭圆的行为
    PolygonController polygonController; //控制多边形行为
    CurveController curveController;    //控制曲线的行为
    QVector<FigureController*> figureController; //图形控制器集合

    FIGURE_TYPE figureMode;     //当前画图系统模式
    DRAW_STATE drawState;       //当前画图状态
    CUTTER cutMode;             //当前裁剪状态

    bool** processed;


};

#endif // CANVAS_GL_H
