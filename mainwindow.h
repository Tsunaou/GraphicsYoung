#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Canvas_GL.h"
#include "Cavans_3DGL.h"
#include "ColorPanel.h"

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QToolButton>
#include <QColorDialog>
#include <QVector>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
#include <QMap>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int getCurCanvasNum();
    void initMdiArea();         //初始化子窗口
    void initColorSelection();  //初始化toolbar上的颜色选择框
    void setFigureMode(FIGURE_TYPE type);   //设置你要绘画的图形
    void clearCheckedState();   //清空选择状态
    void setCutState(CUTTER mode);  //设置裁剪状态

private slots:
    void on_actionNew_triggered();      //新建
    void on_actionSave_triggered();     //保存
    void on_actionOpen_triggered();     //打开文件
    void on_actionClear_triggered();    //清屏
    void on_actionRecall_triggered();   //撤销

    void ShowColor();                   //展示颜色选择按钮颜色

    void on_actionLine_triggered();     //直线
    void on_actionCycle_triggered();    //圆
    void on_actionCurve_triggered();    //曲线
    void on_actionEllipse_triggered();  //椭圆

    void on_actionPen_triggered();      //画笔
    void on_actionBrush_triggered();    //笔刷

    void on_actionBigger_triggered();   //放大
    void on_actionSmaller_triggered();  //缩小
    void on_actionPolygon_triggered();  //多边形
    void on_actionFiller_triggered();   //填充

    void on_actionCutter_toggled(bool arg1);    //裁剪

    void on_actionAbout_triggered();    //关于

    void on_action_3D_triggered();

    void on_actionDrawCurve_triggered();

private:
    Ui::MainWindow *ui;
    QToolButton *colorBtn;      //颜色选择按钮
    ColorPanel *colorSel;       //多功能选择区域
    QVector<Canvas_GL*> canvases;       //多画布存储区域
    QMap<QMdiSubWindow*,int> WindMap;   //绑定画布的序号
    int cur_canvasNum;          //画布的数目 注意到点击叉号并不会减少，因此用map进行了一个映射
    FIGURE_TYPE figureMode;     //当前绘画模式
};

#endif // MAINWINDOW_H
