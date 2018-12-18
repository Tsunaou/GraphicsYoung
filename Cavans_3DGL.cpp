#include "Cavans_3DGL.h"

Cavans_3DGL::Cavans_3DGL(QWidget *parent) : QOpenGLWidget(parent)
{
    numVertices = 0;
    numFaces = 0;
    numEdges = 0;
    nFace = 0;
    this->getOffFiles();
    angle = 0;
    offsetX = 0;
    offsetY = 0;
    offsetZ = -4;
    this->setFocusPolicy(Qt::StrongFocus);
}

void Cavans_3DGL::initializeGL()
{
    initializeOpenGLFunctions();    //初始化OpenGLFunctions
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
}

void Cavans_3DGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(offsetX, offsetY, offsetZ);
    glRotatef(angle, 0.0, 1.0, 0.0);
    gluLookAt(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //绘制各个要素
    //drawFisrtVersion();
    drawFaces();
    drawLines();
    //drawPoints();
}

void Cavans_3DGL::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Cavans_3DGL::getOffFiles()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("OFF File (*.off)"));
    if(fileName == "")                  //如果用户直接关闭了文件浏览对话框，那么文件名就为空值，直接返回
    {
        return;
    }
    else
    {
       QFile file(fileName);
       if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
           QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
           return;
       }
       else
       {
           if(!file.isReadable())
           {
               QMessageBox::warning(this,tr("错误"),tr("该文件不可读"));
           }
           else
           {
               QTextStream textStream(&file);       //读取文件，使用QTextStream
               QString offLabel = textStream.readLine();
               assert(offLabel=="OFF");
               //qDebug()<<offLabel<<endl;
               QString VFE_numbers = textStream.readLine();
               QStringList nums = VFE_numbers.split(QRegExp("[ ]"));
               numVertices = nums.at(0).toInt();
               numFaces = nums.at(1).toInt();
               numEdges = nums.at(2).toInt();
               //cout<<numVertices<<" " << numFaces <<" " << numEdges <<endl;

               for(int i=0;i<numVertices;i++){
                   GLfloat x = 0;
                   GLfloat y = 0;
                   GLfloat z = 0;
                   textStream >> x >> y >> z;
                   textStream.readLine();
                   vecs.push_back(QVector3D(x,y,z));
                   //cout<<x<<" " << y <<" " << z <<endl;
               }


               //单位化

               float max = 0.0;
               for(int i=0;i<vecs.size();i++){
                   if(vecs[i].x()>max)
                       max = vecs[i].x();
                   if(vecs[i].y()>max)
                       max = vecs[i].y();
                   if(vecs[i].z()>max)
                       max = vecs[i].z();
               }

               for(int i=0;i<vecs.size();i++){
                   float xi = vecs[i].x()/max;
                   float yi = vecs[i].y()/max;
                   float zi = vecs[i].z()/max;
                   vecs[i].setX(xi);
                   vecs[i].setY(yi);
                   vecs[i].setZ(zi);
               }


               //第一行特殊处理
               textStream >> nFace;
               //qDebug()<<nFace<<endl;
               QVector<int> face;
               int temp = 0;
               for(int i=0;i<nFace;i++){
                   textStream >> temp;
                   face.push_back(temp);
               }
               faces.push_back(face);

               for(int i=1;i<numFaces;i++){
                   QVector<int> faceNext;
                   int temp = 0;
                   textStream >> temp;
                   for(int i=0;i<nFace;i++){
                       textStream >> temp;
                       faceNext.push_back(temp);
                   }
                   faces.push_back(faceNext);
               }
               int index = 2 + numVertices + 1;
               for(int i=0;i<faces.size();i++){
                   //cout << index+i << " ";
                   for(int j=0;j<faces[i].size();j++){
                       //cout << faces[i][j]<<" ";
                   }
                   //cout <<endl;
               }
           }
       }
    }
}

void Cavans_3DGL::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
        case Qt::Key_Left:  angle+=5;break;
        case Qt::Key_Right: angle-=5;break;
        case Qt::Key_Up:    offsetZ+=0.5;break;
        case Qt::Key_Down:  offsetZ-=0.5;break;
        case Qt::Key_3:     offsetX+=0.5;break;
        case Qt::Key_1:     offsetX-=0.5;break;
        case Qt::Key_5:     offsetY+=0.5;break;
        case Qt::Key_2:     offsetY-=0.5;break;
    default:break;
    }
    if(event->key()==Qt::Key_S){
        this->numVertices = 0;
        this->numFaces = 0;
        this->numEdges = 0;
        this->nFace = 0;
        this->vecs.clear();
        this->faces.clear();
        this->angle = 0;
        this->getOffFiles();
    }
    //qDebug()<<"angle:"<<angle;
    update();
}

void Cavans_3DGL::drawFisrtVersion()
{
    for (int i = 0; i < faces.size(); i++)
    {
        if(nFace == 4){
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_QUADS);
            for(int j=0;j<nFace;j++){
                glVertex3f(vecs[faces[i][j]].x(), vecs[faces[i][j]].y(), vecs[faces[i][j]].z());
            }
            glEnd();
        }else {
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_TRIANGLES);
            for(int j=0;j<nFace;j++){
                glVertex3f(vecs[faces[i][j]].x(), vecs[faces[i][j]].y(), vecs[faces[i][j]].z());
            }
            glEnd();
        }
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_LINES);
        for(int j=0;j<nFace;j++){
            glVertex3f(vecs[faces[i][j]].x(), vecs[faces[i][j]].y(), vecs[faces[i][j]].z());
        }
        glEnd();
    }
}

void Cavans_3DGL::drawPoints()
{
    glColor3f(0.0, 1.0, 0.0);
    glPointSize(2);
    glBegin(GL_POINTS);
    for(int i=0;i<numVertices;i++){
        glVertex3f(vecs[i].x(),vecs[i].y(),vecs[i].z());
    }
    glEnd();
}

void Cavans_3DGL::drawLines()
{
    GLfloat temp_x = 0,temp_y=0,temp_z=0;

    for(int i=0;i<faces.size();i++){
        QVector<int> face = faces[i];
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_LINES);
        for(int j=0;j<nFace;j++){
            if(j==0){
                temp_x = vecs[face[j]].x();
                temp_y = vecs[face[j]].y();
                temp_z = vecs[face[j]].z();
                continue;
            }
            glVertex3f(temp_x, temp_y, temp_z);
            glVertex3f(vecs[face[j]].x(), vecs[face[j]].y(), vecs[face[j]].z());
            temp_x = vecs[face[j]].x();
            temp_y = vecs[face[j]].y();
            temp_z = vecs[face[j]].z();
        }
        glVertex3f(temp_x, temp_y, temp_z);
        glVertex3f(vecs[face[0]].x(), vecs[face[0]].y(), vecs[face[0]].z());
        glEnd();
    }
}

void Cavans_3DGL::drawFaces()
{
    for(int i=0;i<faces.size();i++){
        QVector<int> face = faces[i];
        glColor3f(1.0, 1.0, 1.0);
        switch (nFace) {
            case 3: glBegin(GL_TRIANGLES);break;
            case 4: glBegin(GL_QUADS);break;
            default:glBegin(GL_POLYGON);
        }
        for(int j=0;j<nFace;j++){
            glVertex3f(vecs[face[j]].x(), vecs[face[j]].y(), vecs[face[j]].z());
        }
        glEnd();
    }
}
