#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Canvas_GL.h"
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
    void initMdiArea();
    void initColorSelection();  //初始化toolbar上的颜色选择框
    void setFigureMode(FIGURE_TYPE type);   //设置你要绘画的图形

private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionClear_triggered();
    void ShowColor();

    void on_actionRecall_triggered();

    void on_actionLine_triggered();
    void on_actionCycle_triggered();
    void on_actionCurve_triggered();
    void on_actionEllipse_triggered();

private:
    Ui::MainWindow *ui;
    QToolButton *colorBtn;
    ColorPanel *colorSel;
    QVector<Canvas_GL*> canvases;
    QMap<QMdiSubWindow*,int> WindMap;//绑定画布的序号
    int cur_canvasNum; //画布的数目 注意到点击叉号并不会减少，因此用map进行了一个映射
    FIGURE_TYPE figureMode;
};

#endif // MAINWINDOW_H
