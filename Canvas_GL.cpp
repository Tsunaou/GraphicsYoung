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

double getLength(QPoint &start,QPoint &end){
    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();
    return sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
}

void drawEighthCycle(QPainter* painter,int x0, int y0, int x, int y){
    QPoint temPt1(x0+x,y0+y);
    QPoint temPt2(x0+y,y0+x);
    QPoint temPt3(x0+x,y0-y);
    QPoint temPt4(x0+y,y0-x);
    QPoint temPt5(x0-x,y0-y);
    QPoint temPt6(x0-y,y0-x);
    QPoint temPt7(x0-x,y0+y);
    QPoint temPt8(x0-y,y0+x);

    painter->drawPoint(temPt1);
    painter->drawPoint(temPt2);
    painter->drawPoint(temPt3);
    painter->drawPoint(temPt4);
    painter->drawPoint(temPt5);
    painter->drawPoint(temPt6);
    painter->drawPoint(temPt7);
    painter->drawPoint(temPt8);
}

void drawQuarterEllipse(QPainter* painter,int x0, int y0, int x, int y)
{
    QPoint temPt1(x0+x,y0+y);
    QPoint temPt2(x0+x,y0-y);
    QPoint temPt3(x0-x,y0+y);
    QPoint temPt4(x0-x,y0-y);

    painter->drawPoint(temPt1);
    painter->drawPoint(temPt2);
    painter->drawPoint(temPt3);
    painter->drawPoint(temPt4);
}

void drawOutlineToDebug(QPainter* painter,QPoint &start,QPoint &end){
    QPen debugPen;
    debugPen.setStyle(Qt::DashLine);
    debugPen.setWidth(1);
    debugPen.setColor(Qt::black);
    QPen savePen;
    savePen.setStyle(Qt::SolidLine);
    savePen.setWidth(1);
    savePen.setColor(Qt::black);

    painter->setPen(debugPen);

    int x0 = start.x();
    int y0 = start.y();
    int x = end.x()-x0;
    int y = end.y()-y0;

    QPoint temPt1(x0+x,y0+y);
    QPoint temPt2(x0+x,y0-y);
    QPoint temPt3(x0-x,y0+y);
    QPoint temPt4(x0-x,y0-y);

    painter->drawLine(temPt1,temPt2);
    painter->drawLine(temPt1,temPt3);
    painter->drawLine(temPt4,temPt2);
    painter->drawLine(temPt3,temPt4);

    painter->setPen(savePen);

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

void MyDrawCycle(QPainter* painter,QPoint &start,QPoint &end){
    //首先先在这里实现我的直线算法
    qDebug()<<"MyDrawCycle "<<endl;
    int x0 = start.x();
    int y0 = start.y();
    double R = getLength(start,end);
    int x,y,p;
    x=0;
    y=R;
    p=3-2*R;
    for(;x<=y;x++)
    {
        drawEighthCycle(painter,x0,y0,x,y);
        if(p>=0){
           p+=4*(x-y)+10;
           y--;
        }else{
            p+=4*x+6;
        }
    }
}

void DrawEllipse(QPainter* painter,QPoint &start,QPoint &end){
    //首先先在这里实现我的椭圆算法
    drawOutlineToDebug(painter,start,end);
    qDebug()<<"MyDrawCycle "<<endl;
    int x0 = start.x();
    int y0 = start.y();
    int a = abs(end.x()-x0);
    int b = abs(end.y()-y0);
//    if(fabs(a)<fabs(b)){

//    }
    qDebug()<<"Ellipse:a="<<a<<",b="<<b<<endl;


    double sqa = a*a;
    double sqb = b*b;

    double d = sqb + sqa*(0.25 - b);
    int x = 0;
    int y = b;
    drawQuarterEllipse(painter,x0, y0, x, y);
    // 1
    while (sqb*(x + 1) < sqa*(y - 0.5))
    {
        if (d < 0)
        {
            d += sqb*(2 * x + 3);
        }
        else
        {
            d += (sqb*(2 * x + 3) + sqa*((-2)*y + 2));
            --y;
        }
        ++x;
        drawQuarterEllipse(painter,x0, y0, x, y);
    }
    d = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a * b) * 2;
    // 2
    while (y > 0)
    {
        if (d < 0)
        {
            d += sqb * (2 * x + 2) + sqa * ((-2) * y + 3);
            ++x;
        }
        else
        {
            d += sqa * ((-2) * y + 3);
        }
        --y;
        drawQuarterEllipse(painter,x0, y0, x, y);
    }
}

void MyDrawEllipse(QPainter* painter,QPoint &start,QPoint &end){
    //用一个虚线框来解决
//    drawOutlineToDebug(painter,start,end);

    //首先先在这里实现我的椭圆算法
    qDebug()<<"MyDrawEllipse "<<endl;
    int x0 = start.x(); //椭圆中心
    int y0 = start.y();
    int rx = abs(end.x()-x0); //椭圆长短轴
    int ry = abs(end.y()-y0);

    double rx_2 = rx*rx;
    double ry_2 = ry*ry;

    double pl = ry_2 - rx_2*ry + rx_2/4; //区域1中决策参数
    int x = 0;
    int y = ry;
    drawQuarterEllipse(painter,x0, y0, x, y);//第一个点

    //区域一 切线斜率k<=1
    while (ry_2*x <= rx_2*y){

        if (pl < 0){
            pl += 2*ry_2*x +3*ry_2;
        }else{
            pl += 2*ry_2*x - 2*rx_2*y + 2*rx_2+ 3*ry_2;
            y--;
        }
        x++;
        drawQuarterEllipse(painter,x0, y0, x, y);
    }
    //区域二 切线斜率k > 1
    pl = ry_2*(x+1/2)*(x+1/2)+rx_2*(y-1)*(y-1)-rx_2*ry_2;  //使用区域1中最后点(x0,y0)来计算区域2中参数初值

    while (y > 0){
        if (pl < 0){
            pl += 2*ry_2*x - 2*rx_2*y + 2*ry_2 + 3*rx_2;
            x++;
        }
        else{
            pl += 3*rx_2 - 2*rx_2*y;
        }
        y--;
        drawQuarterEllipse(painter,x0, y0, x, y);
    }
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

    QPixmap *regTemp =pix;
    pix= getPixCopy(pixToMove);
    delete regTemp;

    painter->begin(pix);					//(b)
    painter->setPen(pen);					//将QPen对象应用到绘制对象中

    switch(this->figureMode){
        case LINE:  MyDrawLineDDA(painter,startPos,endPos); break;
        case CYCLE: MyDrawCycle(painter,startPos,endPos); break;
        case ELLIPSE:MyDrawEllipse(painter,startPos,endPos); break;

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
        case LINE:  MyDrawLineDDA(painter,startPos,endPos); break;
        case CYCLE: MyDrawCycle(painter,startPos,endPos); break;
        case ELLIPSE:MyDrawEllipse(painter,startPos,endPos); break;

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
