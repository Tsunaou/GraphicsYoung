#ifndef CAVANS_3DGL_H
#define CAVANS_3DGL_H
#include "Canvas_GL.h"

#include <QWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QOpenGLWidget>
#include "QOpenGLFunctions"
#include <QtOpenGL/QtOpenGL>
#include <QKeyEvent>



class Cavans_3DGL : public QOpenGLWidget,protected QOpenGLFunctions
{
public:
    explicit Cavans_3DGL(QWidget *parent = nullptr);
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void getOffFiles();
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    void drawFisrtVersion();
    void drawPoints();
    void drawLines();
    void drawFaces();
    int numVertices;
    int numFaces;
    int numEdges;
    int nFace;

    QVector<QVector3D> vecs;
    QVector<QVector<int>> faces;
    float angle; //旋转角度
    float offsetX;  //z轴观察距离
    float offsetY;  //z轴观察距离
    float offsetZ;  //z轴观察距离
};

#endif // CAVANS_3DGL_H
