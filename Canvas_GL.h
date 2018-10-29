#ifndef CANVAS_GL_H
#define CANVAS_GL_H

#include "constParams.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QtDebug>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPixmap>
#include <QPoint>
#include <QPainter>
#include <QPalette>

#include <QFileDialog>
#include <QMessageBox>



class Canvas_GL : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Canvas_GL(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    void saveImage(); //保存当前画布内容（pix内容）
    void clearImage(); //清空当前画布内容
    void recallImage();//撤销上一步操作
    void setMode(FIGURE_TYPE type); //设置图形
    QPixmap *getPixCopy();
    QPixmap *getPixCopy(QPixmap* pic);
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;


signals:

public slots:
    void setColor(QColor);

private:
    QPixmap *pix;
    QPixmap *pixToMove;
    QPen pen;

    QPoint startPos;
    QPoint endPos;
    QPoint regPos;
    int style;
    int weight;
    QColor color;

    QVector<QPixmap*> reVec;
    FIGURE_TYPE figureMode;

};

#endif // CANVAS_GL_H
