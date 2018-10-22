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

private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionClear_triggered();
    void ShowColor();

    void on_actionRecall_triggered();

private:
    Ui::MainWindow *ui;
    QToolButton *colorBtn;
    QVector<Canvas_GL*> canvases;
    QMap<QMdiSubWindow*,int> WindMap;//绑定画布的序号
    int cur_canvasNum;
};

#endif // MAINWINDOW_H
