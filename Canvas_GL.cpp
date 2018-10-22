#include "Canvas_GL.h"

Canvas_GL::Canvas_GL(QWidget *parent) : QOpenGLWidget(parent)
{
    setAutoFillBackground(true);    	//对窗体背景色的设置
    setPalette(QPalette(Qt::gray));

    pix =new QPixmap(size());           //此QPixmap对象用来准备随时接收绘制的内容
    pix->fill(Qt::white);               //填充背景色为白色
    setMaximumSize(1200,900);            //设置绘制区窗体的最大尺寸（这里因为一开始放大后失真， 因此就限制了大小）
    setMinimumSize(1200,900);            //设置绘制区窗体的最小尺寸

    style = static_cast<int>(Qt::SolidLine);//设置QPainter的属性
    weight = 3;
    color = Qt::black;

//    setAttribute(Qt::WA_StaticContents);
}

void Canvas_GL::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<"mousePressEvent"<<endl;
    startPos = e->pos();
}

void MyDrawLineDDA(QPainter* painter,QPoint &start,QPoint &end){
    //首先先在这里实现我的直线算法
    qDebug()<<"MyDrawLine DDA"<<endl;

    int x1 = start.x();
    int y1 = start.y();
    int x2 = end.x();
    int y2 = end.y();

    double dx=x2-x1;
    double dy=y2-y1;
    double e=(fabs(dx)>fabs(dy))?fabs(dx):fabs(dy);
    double x=x1;
    double y=y1;

    dx/=e;
    dy/=e;

    for(int i=1;i<=e;i++){

        QPoint temPt((int)(x+0.5), (int)(y+0.5));
        painter->drawPoint(temPt);

        x+=dx;
        y+=dy;
    }

}

void MyDrawLineBresenham(QPainter* painter,QPoint &start,QPoint &end){
    //首先先在这里实现我的直线算法
    qDebug()<<"MyDrawLine Bresenham"<<endl;

    int x1 = start.x();
    int y1 = start.y();
    int x2 = end.x();
    int y2 = end.y();

    int x,y,dx,dy,p;
    x=x1;
    y=y1;
    dx=x2-x1;
    dy=y2-y1;
    p=2*dy-dx;
    for(;x<=x2;x++)
    {
        QPoint temPt(x,y);
        painter->drawPoint(temPt);
        if(p>=0)
        {     y++;
            p+=2*(dy-dx);
        }
        else
        {
            p+=2*dy;
        }
    }

}

void Canvas_GL::mouseMoveEvent(QMouseEvent *e)
{
    qDebug()<<"mouseMoveEvent"<<endl;

    QPainter *painter = new QPainter;		//新建一个QPainter对象
    QPen pen;								//新建一个QPen对象
    pen.setStyle((Qt::PenStyle)style);		//(a)
    pen.setWidth(weight);					//设置画笔的线宽值
    pen.setColor(color);					//设置画笔的颜色
    painter->begin(pix);					//(b)
    painter->setPen(pen);					//将QPen对象应用到绘制对象中
    //绘制从startPos到鼠标当前位置的直线
//    painter->drawPoint(e->pos());
//    painter->drawLine(startPos,e->pos());
//    double radius = sqrt((startPos.x()-e->pos().x())*(startPos.x()-e->pos().x())+(startPos.y()-e->pos().y())*(startPos.y()-e->pos().y()));
//    painter->drawEllipse(startPos.x()-radius,startPos.y()-radius,2*radius,2*radius);
    painter->end();
//    startPos =e->pos();				//更新鼠标的当前位置，为下次绘制做准备
    update();						//重绘绘制区窗体
}

void Canvas_GL::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug()<<"mouseReleaseEvent"<<endl;
    qDebug()<<"Pixmap size:"<<pix->height()<<","<<pix->width()<<endl;
    QPainter *painter = new QPainter;		//新建一个QPainter对象
    QPen pen;								//新建一个QPen对象
    pen.setStyle((Qt::PenStyle)style);		//(a)
    pen.setWidth(weight);					//设置画笔的线宽值
    pen.setColor(color);					//设置画笔的颜色
    painter->begin(pix);					//(b)
    painter->setPen(pen);					//将QPen对象应用到绘制对象中
    //绘制从startPos到鼠标当前位置的直线
//    painter->drawLine(startPos,e->pos());
    QPoint endPos = e->pos();

    MyDrawLineDDA(painter,startPos,endPos);
//    double radius = sqrt((startPos.x()-e->pos().x())*(startPos.x()-e->pos().x())+(startPos.y()-e->pos().y())*(startPos.y()-e->pos().y()));
//    painter->drawEllipse(startPos.x()-radius,startPos.y()-radius,2*radius,2*radius);
    painter->end();
    startPos =e->pos();				//更新鼠标的当前位置，为下次绘制做准备
    update();						//重绘绘制区窗体

}

void Canvas_GL::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0),*pix);
}

void Canvas_GL::resizeEvent(QResizeEvent *event)
{

    qDebug()<<"resizeEvent"<<endl;

    if(height()>pix->height()||width()>pix->width())	//(a)
    {
        QPixmap *newPix = new QPixmap(size());	//创建一个新的QPixmap对象
        newPix->fill(Qt::white);                //填充新QPixmap对象newPix的颜色为白色背景色
        QPainter p(newPix);
        p.drawPixmap(QPoint(0,0),*pix);         //在newPix中绘制原pix中的内容
        pix = newPix;                           //将newPix赋值给pix作为新的绘制图形接收对象
    }
    QWidget::resizeEvent(event);                //完成其余的工作
    //    this->resizeGL(width(),height());
}

QPixmap *Canvas_GL::getPix()
{
    return this->pix;
}



void Canvas_GL::initializeGL(){

    qDebug() << "Function: initializeGL()" << endl;

    initializeOpenGLFunctions();

    setGeometry(0, 0, 1920, 1080);    //设置画布大小,但是在resizeGL()中会自适应，所以其实可以不用管
    glClearColor(1.0, 1.0, 1.0, 0); //rgb(255,255,255) 背景色为白色
//    glClearColor(0.0, 0.0, 0.0, 0); //rgb(0,0,0) 背景色为黑色

}

void Canvas_GL::resizeGL(int width, int height){

    qDebug() << "Function: resizeGL()" << width << height << endl;
//    glViewport(0,0,width,height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-width/2,width/2,-height/2,height/2,-1,1);
//    glMatrixMode(GL_MODELVIEW);
}

void Canvas_GL::setColor(QColor c)
{
    color = c;
}

void Canvas_GL::paintGL(){
    qDebug() << "Function: paintGL()" << endl;
     glClear(GL_COLOR_BUFFER_BIT);//清屏
 }
