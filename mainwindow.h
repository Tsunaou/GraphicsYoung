#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Canvas_GL.h"

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QToolButton>
#include <QColorDialog>
#include <QVector>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void ShowColor();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    QToolButton *colorBtn;
    QVector<Canvas_GL*> canvases;
};

#endif // MAINWINDOW_H
