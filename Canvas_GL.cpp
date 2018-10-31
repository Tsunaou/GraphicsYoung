#include "Canvas_GL.h"

Canvas_GL::Canvas_GL(QWidget *parent) : QOpenGLWidget(parent)
{
    setAutoFillBackground(true);    	//对窗体背景色的设置
    setPalette(QPalette(Qt::gray));

    pix =new QPixmap(size());           //此QPixmap对象用来准备随时接收绘制的内容
    pix->fill(Qt::white);               //填充背景色为白色
    setMaximumSize(1366,768);            //设置绘制区窗体的最大尺寸（这里因为一开始放大后失真， 因此就限制了大小）
    setMinimumSize(1366,768);            //设置绘制区窗体的最小尺寸

    style = static_cast<int>(Qt::SolidLine);//设置QPainter的属性
    weight = 1;
    color = Qt::black;

    pen.setStyle((Qt::PenStyle)style);		//(a)
    pen.setWidth(weight);					//设置画笔的线宽值
    pen.setColor(color);					//设置画笔的颜色

    QPixmap* tmp = pix;
    reVec.push_back(tmp);

//    setAttribute(Qt::WA_StaticContents);
}

void Canvas_GL::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<"mousePressEvent"<<endl;
    pixToMove = getPixCopy();
    startPos = e->pos();
    regPos= e->pos();
}


void Canvas_GL::mouseMoveEvent(QMouseEvent *e)
{
//    qDebug()<<"mouseMoveEvent"<<endl;

    QPainter *painter = new QPainter;		//新建一个QPainter对象
    QPoint endPos = e->pos();

    QPixmap *regTemp =pix;                  //实现图形随着鼠标动态加载
    pix= getPixCopy(pixToMove);
    delete regTemp;

//    *pix = *pixToMove; //

    painter->begin(pix);					//(b)
    painter->setPen(pen);					//将QPen对象应用到绘制对象中

    switch(this->figureMode){
        case LINE:  lineController.MyDrawLineDDA(painter,startPos,endPos); break;
        case CYCLE: cycleController.MyDrawCycle(painter,startPos,endPos); break;
        case ELLIPSE:ellipseController.MyDrawEllipse(painter,startPos,endPos); break;

        default:
            qDebug()<<"Select Error Mode of Figure"<<endl;
    }
    regPos = e->pos();//记录下次消除的位置

    painter->end();
//    startPos =e->pos();				//更新鼠标的当前位置，为下次绘制做准备
    update();						//重绘绘制区窗体
}

void Canvas_GL::mouseReleaseEvent(QMouseEvent *e)
{
//    qDebug()<<"mouseReleaseEvent"<<endl;
//    qDebug()<<"Pixmap size:"<<pix->height()<<","<<pix->width()<<endl;
    QPainter *painter = new QPainter;		//新建一个QPainter对象

    painter->begin(pix);					//(b)
    painter->setPen(pen);					//将QPen对象应用到绘制对象中

    QPoint endPos = e->pos();

    switch(this->figureMode){
        case LINE:  lineController.MyDrawLineDDA(painter,startPos,endPos); break;
        case CYCLE: cycleController.MyDrawCycle(painter,startPos,endPos); break;
        case ELLIPSE:ellipseController.MyDrawEllipse(painter,startPos,endPos); break;

        default:
            qDebug()<<"Select Error Mode of Figure"<<endl;
    }


    painter->end();
    startPos =e->pos();				//更新鼠标的当前位置，为下次绘制做准备
    update();						//重绘绘制区窗体

    QPixmap* tmp = this->getPixCopy();
    reVec.push_back(tmp);
//    qDebug()<<"push_back"<<&tmp<<endl;

}

void Canvas_GL::paintEvent(QPaintEvent *)
{
    qDebug()<<"paintEvent"<<endl;
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

void Canvas_GL::saveImage()
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("Save File"),
                                   "/SaveTest",
                                   tr("Images (*.png *.xpm *.jpg)"));
        if(!file_path.isEmpty())
        {
            //如果没有写后缀就自动加上
            if(QFileInfo(file_path).suffix().isEmpty())
                file_path.append(".png");
            this->pix->save(file_path);
        }
        else
            QMessageBox::warning(this,tr("Path"),QString::fromLocal8Bit("You haven't input the filename"));
}

void Canvas_GL::clearImage()
{
    QPixmap *clearPix =new QPixmap(size());
    clearPix->fill(Qt::white);
    this->pix = clearPix;
    while(!reVec.empty()){
        reVec.pop_back();
    }
    reVec.push_back(clearPix);
    update();
}

void Canvas_GL::recallImage()
{
    qDebug()<<"Recall"<<endl;
    if(reVec.size()<=1){
        qDebug()<<"It the initial state of the image."<<endl;
        QPixmap *clearPix =new QPixmap(size());
        clearPix->fill(Qt::white);
        reVec[0]=clearPix;
        pix = clearPix;
        update();
    }else{
        pix = reVec[reVec.size()-1];
        reVec.pop_back();
        update();
    }
}

void Canvas_GL::setMode(FIGURE_TYPE type)
{
    this->figureMode = type;
}

QPixmap *Canvas_GL::getPixCopy()
{
    QPixmap *newPix = new QPixmap(size());	//创建一个新的QPixmap对象
    newPix->fill(Qt::white);                //填充新QPixmap对象newPix的颜色为白色背景色
    QPainter p(newPix);
    p.drawPixmap(QPoint(0,0),*pix);         //在newPix中绘制原pix中的内容
    qDebug()<<"Pix Copy at"<<&newPix<<endl;
    return newPix;
}

QPixmap *Canvas_GL::getPixCopy(QPixmap *pic)
{
    QPixmap *newPix = new QPixmap(size());	//创建一个新的QPixmap对象
    newPix->fill(Qt::white);                //填充新QPixmap对象newPix的颜色为白色背景色
    QPainter p(newPix);
    p.drawPixmap(QPoint(0,0),*pic);         //在newPix中绘制原pix中的内容
    qDebug()<<"Pix Copy at"<<&newPix<<endl;
    return newPix;
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
    pen.setColor(color);
}

void Canvas_GL::paintGL(){
    qDebug() << "Function: paintGL()" << endl;
     glClear(GL_COLOR_BUFFER_BIT);//清屏
 }
