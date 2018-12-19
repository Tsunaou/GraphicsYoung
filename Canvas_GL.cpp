#include "Canvas_GL.h"

Canvas_GL::Canvas_GL(QWidget *parent) : QOpenGLWidget(parent)
{
    setAutoFillBackground(true);    	//对窗体背景色的设置
    setPalette(QPalette(Qt::gray));

    pix =new QPixmap(size());           //此QPixmap对象用来准备随时接收绘制的内容,大小为初始的大小
    pix->fill(Qt::white);               //填充背景色为白色
    setMaximumSize(1366,768);            //设置绘制区窗体的最大尺寸（这里因为一开始放大后失真， 因此就限制了大小）
    setMinimumSize(1366,768);            //设置绘制区窗体的最小尺寸
//    setMaximumSize(1980,768);            //设置绘制区窗体的最大尺寸（这里因为一开始放大后失真， 因此就限制了大小）
//    setMinimumSize(1366,768);            //设置绘制区窗体的最小尺寸
    //painter = new QPainter;
    style = static_cast<int>(Qt::SolidLine);//设置QPainter的属性
    weight = PEN_WIDTH;                     //画笔宽度
    color = Qt::black;                      //默认颜色
    pen.setStyle((Qt::PenStyle)style);		//(a)
    pen.setWidth(weight);					//设置画笔的线宽值
    pen.setColor(color);					//设置画笔的颜色
    //painter->setPen(pen);					//将QPen对象应用到绘制对象中

    QPixmap* tmp = pix;
    reVec.push_back(tmp);

    //状态设置
    this->figureMode = PEN; //笔比较OK感觉
    this->drawState = UNDO;
    this->cutMode = UNCUT;
    this->lineController.setState(&this->drawState);
    this->cycleController.setState(&this->drawState);
    this->ellipseController.setState(&this->drawState);
    this->polygonController.setState(&this->drawState);
    this->curveController.setState(&this->drawState);
    //用统一的接口来约束行为（测试中，目前效果还行。。）
    figureController.push_back(&lineController);
    figureController.push_back(&lineController);
    figureController.push_back(&cycleController);
    figureController.push_back(&ellipseController);
    figureController.push_back(&polygonController);
    figureController.push_back(&curveController);

    //鼠标跟踪
    //setMouseTracking(true);
    this->processed = nullptr;

    //窗体设置

}

void Canvas_GL::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<"mousePressEvent"<<endl;
    showDrawingStates();

    //解决曲线时闪烁问题（应该还要加一个setting points的判断 TODO）
//    if(this->figureMode == CURVE && this->drawState == DRAWING){
//        *pixToMove = *pix;
//    }

    //裁剪2018/11/29
    if(this->cutMode == CUTTING){
        this->cutStart = e->pos();
        return;
    }

    if(this->drawState == UNDO){ //上次绘画状态结束，pixToMove更新到最新状态,双缓冲结束（此时可以认为是首次Press）
        printDebugMessage("mousePressEvent1");
        pixToMove = getPixCopy();
        printDebugMessage("mousePressEvent1.2");
    }
    else{
        if(figureController[figureMode]->isOperationing(e,startPos,endPos)){ //绘画状态中，双缓冲准备
            printDebugMessage("isOperationing");
            *pix = *pixToMove;
            setCursor(Qt::PointingHandCursor);//设置鼠标样式
            printDebugMessage("mousePressEvent1.2");
        }else{
            //对保存上次绘画状态的图像在图中,此时startPos和endPos存储上次图像的信息
            printDebugMessage("drawBeforeNewState");
            setCursor(Qt::ArrowCursor);//设置鼠标样式
            drawBeforeNewState();
        }
    }



    //Refactor Begin---------------------------------------------------------------------------------------------------------------------------------------
    QPainter *painter = new QPainter();
    printDebugMessage("mousePressEvent1.3");
    painter->begin(pix);
    painter->setPen(pen);
    printDebugMessage("mousePressEvent1.4");
    if(isDrawingFigure()){
        figureController[figureMode]->mousePressEvent(painter,e,pen);
        printDebugMessage("mousePressEvent1.5");
        painter->end();
        printDebugMessage("mouseMoveEvent painter end");
        delete painter;
        if(this->drawState == UNDO){
            pixToMove = getPixCopy();
        }else{
            *pix = *pixToMove;
        }
        update();
        return;
    }

    //填充
    if(e->button()==Qt::RightButton || this->figureMode == FILLER){
        QImage img = pix->toImage();
        QColor pcolor = img.pixelColor(e->pos().x(),e->pos().y());   //得到Press点的信息
        qDebug()<< pcolor <<endl;
        //
        qDebug()<<"命中"<<endl;

        fillColor(&img, pcolor,painter,e->pos().x(),e->pos().y());
        painter->end();
        //update();

        QPixmap* tmp = this->getPixCopy();
        reVec.push_back(tmp);
        update();
        pixToMove = getPixCopy();//结束绘画状态，准备下次绘画
        return;
    }
    //
    painter->end();
    delete painter;
    //Refactor End---------------------------------------------------------------------------------------------------------------------------------------

    //Pen的话能到这里
    printDebugMessage("Pen here press");
    startPos = e->pos();
}

void Canvas_GL::mouseMoveEvent(QMouseEvent *e)
{
//    if(this->figureMode == CURVE && curveController.getIsSettingPoints()){
//        return;
//    }

    qDebug()<<"mouseMoveEvent"<<endl;
    QPoint endPos = e->pos();

    if(this->figureMode == FILLER){
        return;
    }

    if(this->figureMode!=PEN && this->figureMode !=BRUSH){
        *pix = *pixToMove; //实现图形随着鼠标动态加载,双缓冲
    }
    printDebugMessage("mouseMoveEvent 1");
    QPainter *painter = new QPainter();
    painter->begin(pix);
    painter->setPen(pen);
    printDebugMessage("mouseMoveEvent 2");


//Refactor---------------------------------------------------------------------------------------------------------------------------------------
    if(isDrawingFigure()){
        printDebugMessage("mouseMoveEvent 3");
        //裁剪
        if(this->cutMode == CUTTING){
            this->cutEnd = e->pos();
            QPen debugPen;
            debugPen.setWidth(1);
            debugPen.setColor(Qt::blue);
            debugPen.setStyle(Qt::DashLine);
            painter->setPen(debugPen);
            painter->drawRect(cutStart.x(),cutStart.y(),cutEnd.x()-cutStart.x(),cutEnd.y()-cutStart.y());
            painter->setPen(pen);
            if(this->figureMode == LINE){
                this->lineController.drawWhileCutting(painter,pen);
            }
        }else{
            figureController[figureMode]->mouseMoveEvent(painter,e,pen);
        }
        //
        painter->end();
        delete painter;
        update();
        return;
    }
//Refactor---------------------------------------------------------------------------------------------------------------------------------------

    //不是LINE,CYCLE,ELLIPSE才会执行下列语句
    //Pen的话能到这里
    printDebugMessage("Pen here move");
    lineController.MyDrawLineDDA(painter,startPos,endPos);
    startPos = e->pos();
    painter->end();
    delete painter;
    update();						//重绘绘制区窗体
}

void Canvas_GL::mouseReleaseEvent(QMouseEvent *e)
{
//    if(this->figureMode == CURVE && curveController.getIsSettingPoints()){
//        return;
//    }

    qDebug()<<"mouseReleaseEvent"<<endl;
    setCursor(Qt::ArrowCursor);//设置鼠标样式

    if(this->figureMode == FILLER){
        return;
    }

    if(this->figureMode!=PEN && this->figureMode !=BRUSH){
        *pix = *pixToMove;
    }

    QPainter *painter = new QPainter();
    painter->begin(pix);
    painter->setPen(pen);

    QPoint endPos = e->pos();

    //Refactor--------------------------------------------------------------------------------------------------------------------------------------
    if(isDrawingFigure()){
        if(this->cutMode == CUTTING){
            this->cutEnd = e->pos();
            QPen debugPen;
            debugPen.setWidth(1);
            debugPen.setColor(Qt::blue);
            debugPen.setStyle(Qt::DashLine);
            painter->setPen(debugPen);
            painter->drawRect(cutStart.x(),cutStart.y(),cutEnd.x()-cutStart.x(),cutEnd.y()-cutStart.y());
            painter->setPen(pen);
            if(this->figureMode == LINE){
                this->lineController.drawWhileCutting(painter,pen);
            }
        }else{
            figureController[figureMode]->mouseReleaseEvent(painter,e,pen);
        }
        painter->end();
        delete painter;
        update();
        return;
    }
    //Refactor---------------------------------------------------------------------------------------------------------------------------------------

    //不是图形才会执行下列语句
    printDebugMessage("Pen here release");
    lineController.MyDrawLineDDA(painter,startPos,endPos);
    painter->end();
    delete painter;
    startPos =e->pos();				//更新鼠标的当前位置，为下次绘制做准备
    update();						//重绘绘制区窗体

    QPixmap* tmp = this->getPixCopy();
    reVec.push_back(tmp);

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

void Canvas_GL::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

bool Canvas_GL::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    imageOpen = QPixmap::fromImage(loadedImage);
    //modified = false;
    int width = loadedImage.width();
    int height = loadedImage.height();
    setMaximumSize(width,height);            //设置绘制区窗体的最大尺寸（这里因为一开始放大后失真， 因此就限制了大小）
    setMinimumSize(width,height);            //设置绘制区窗体的最小尺寸
//    this->resize(width,height);
//    this->resizeGL(width,height);
//    qDebug()<<"height:"<<height<<endl;
//    qDebug()<<"width:"<<width<<endl;
    this->pix = &imageOpen;
//    this->resize(width,height);
//    this->resizeGL(width,height);
    update();
    return true;
}

void Canvas_GL::clearImage()
{
    //清屏后，重置状态
    QPixmap *clearPix =new QPixmap(size());
    clearPix->fill(Qt::white);
    this->pix = clearPix;
    this->pixToMove = this->pix;
    while(!reVec.empty()){
        reVec.pop_back();
    }
    reVec.push_back(clearPix);
    this->drawState = UNDO;
    this->lineController.clearState();
    this->cycleController.clearState();
    this->ellipseController.clearState();
    this->polygonController.clearState();
    this->curveController.clearState();
    clearStates();
    update();
}

void Canvas_GL::recallImage()
{
    qDebug()<<"Recall"<<endl;
    clearStates();
    qDebug()<<"reVec.size ="<<reVec.size()<<endl;
    if(reVec.size()<=1){
        qDebug()<<"It the initial state of the image."<<endl;
        QPixmap *clearPix =new QPixmap(size());
        clearPix->fill(Qt::white);
        reVec[0]=clearPix;
        pix = clearPix;
        pixToMove = pix;
        update();
    }else{
        pix = reVec[reVec.size()-1];
        pixToMove = pix;
        reVec.pop_back();
        //figureVec.pop_back();//这里可能会有bug//果真出bug
        qDebug()<<"reVec.size>1"<<endl;
        update();
    }
}

void Canvas_GL::clearStates()
{
    this->drawState = UNDO;
    this->lineController.clearState();
    this->cycleController.clearState();
    this->ellipseController.clearState();
    this->polygonController.clearState();
    this->curveController.clearState();
}

void Canvas_GL::setMode(FIGURE_TYPE type)
{
    if(figureMode == FIGURE || figureMode == PEN || figureMode == BRUSH || figureMode == FILLER){//初始化
        this->figureMode = type;
        clearStates();
    }
    else{
        //只坐到上面，图像上会留下辅助点，因为还是得处理下QAQ
        figureController[figureMode]->getStartAndEnd(startPos,endPos);
        drawBeforeNewState();
        //切换前应该要注意清空信息
        clearStates();
        //
        this->figureMode = type;
    }

    if(figureMode == BRUSH){
        pen.setWidth(BRUSH_WIDTH);
    }else{
        pen.setWidth(this->weight);
    }
}

void Canvas_GL::setCutMode(CUTTER mode)
{
    if(this->cutMode==CUTTING && mode == UNCUT){ //进行裁剪事件
        if(this->figureMode == LINE){
            *pix = *pixToMove;
            QPainter *painter = new QPainter();
            painter->begin(pix);
            painter->setPen(pen);
            if(lineController.cutLineLiangBsrsky(this->cutStart,this->cutEnd,painter,pen)) {
                QPen debugPen;
                debugPen.setWidth(1);
                debugPen.setColor(Qt::blue);
                debugPen.setStyle(Qt::DashLine);
                painter->setPen(debugPen);
                painter->drawRect(cutStart.x(),cutStart.y(),cutEnd.x()-cutStart.x(),cutEnd.y()-cutStart.y());
                painter->setPen(pen);
                //do nothing
            }else //直线被舍弃了
            {
                this->drawState = UNDO;
                this->lineController.clearState();
                this->cycleController.clearState();
                this->ellipseController.clearState();
                this->polygonController.clearState();
                this->curveController.clearState();
            }
            painter->end();
            delete painter;
            update();

        }
    }
    this->cutMode = mode;
}

void Canvas_GL::setBigger()
{
    QMouseEvent *e;//纯属失误,不过也不会用到
    if(this->drawState == UNDO){
        return;
    }
    else{
        if(isDrawingFigure()){
            *pix = *pixToMove;
            QPainter *painter = new QPainter();
            painter->begin(pix);
            painter->setPen(pen);
            figureController[figureMode]->setBigger(painter,e,pen);
            painter->end();
            delete painter;
            update();
        }
    }
}

void Canvas_GL::setSmaller()
{
    QMouseEvent *e;//纯属失误,不过也不会用到
    if(this->drawState == UNDO){
        return;
    }
    else{
        if(isDrawingFigure()){
            *pix = *pixToMove;
            QPainter *painter = new QPainter();
            painter->begin(pix);
            painter->setPen(pen);
            figureController[figureMode]->setSmaller(painter,e,pen);
            painter->end();
            delete painter;
            update();
        }
    }
}

void Canvas_GL::fillColor(QImage *img, QColor backcolor, QPainter *painter, int cx, int cy)
{
    QStack<QPoint> *stack = new QStack<QPoint>;

    int maxWidth = pix->width()-1;
    int maxHeight = pix->height()-1;

    bool **flag = new bool*[maxWidth];
    for(int i=0;i<maxWidth;i++){
        flag[i] = new bool[maxHeight];
    }
    for(int i=0;i<maxWidth;i++){
        for(int j=0;j<maxHeight;j++){
            flag[i][j] = false;
        }
    }
    this->processed = flag;


    stack->clear();
    stack->push(QPoint(cx,cy));    //要填充的press的那个点
    while(!stack->empty()){
        //qDebug()<<"stack size is"<<stack.size()<<endl;
        QPoint p = stack->pop();
        int x = p.x();
        int y = p.y();

        if((x<=0) || (x>=(pix->width()-3))) continue;     //超限
        if((y<=0) || (y>=(pix->height()-3))) continue;    //超限
        try{
            if(img->pixelColor(x,y) != backcolor) continue;   //边界
        }catch(QString){
            qDebug()<<"异常处理"<<endl;
        }
        if(processed[x][y] == true) continue;      //上色过了
        //qDebug()<<"print a"<<x<<","<<y<<")"<<endl;
        processed[x][y] = true;
        if(painter->isActive()) {//保证在Painter有效的时候才进行
            painter->drawPoint(x,y);
        }
        if(!processed[x][y+1]){
            stack->push(QPoint(x,y+1));
        }
        if(!processed[x][y-1]){
            stack->push(QPoint(x,y-1));
        }
        if(!processed[x-1][y]){
            stack->push(QPoint(x-1,y));
        }
        if(!processed[x+1][y]){
            stack->push(QPoint(x+1,y));
        }
    }
    stack->clear();
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

void Canvas_GL::showDrawingStates()
{
    qDebug()<<"state is";
    switch (drawState) {
        case UNDO:
            qDebug()<<"UNDO"<<endl;
            break;
        case DRAWING :
            qDebug()<<"DRAWING"<<endl;
            break;
    }
}

void Canvas_GL::drawBeforeNewState()
{
    if(this->figureMode == PEN || this->figureMode == BRUSH || this->figureMode == FILLER){
        return;
    }

//    if(this->figureMode == CURVE){
//        if(reVec.size()>=2){
//            *pixToMove = *reVec[reVec.size()-2];
//        }else if(reVec.size() == 1){
//            *pixToMove = *reVec[0];
//        }
//        //QMessageBox::warning(this,tr("drawBeforeNewState"),tr("pixToMove 复原"));
//    }

    *pix = *pixToMove;
    QPainter *painter = new QPainter();
    painter->begin(pix);
    painter->setPen(pen);
    switch(figureMode){
        case LINE:
                    lineController.MyDrawLineDDA(painter,startPos,endPos);break;
        case CYCLE:
                    cycleController.MyDrawCycleMidpoint(painter,startPos,endPos);break;
        case ELLIPSE:
                    ellipseController.MyDrawEllipse(painter,startPos,endPos);
                    ellipseController.clearRotateAngle(); break;//清空现场
        case POLYGON:
                    polygonController.drawUpPolygon(painter);break;
        case CURVE:
                    //QMessageBox::warning(this,tr("drawBeforeNewState"),QString::fromLocal8Bit("drawBeforeNewState Curve To"));
                    curveController.drawBezier(painter,pen);
                    curveController.clearState();
                    break;

    default:
        break;
    }
    painter->end();
    delete painter;
    //为撤销做准备
    QPixmap* tmp = this->getPixCopy();
    reVec.push_back(tmp);
    update();
    pixToMove = getPixCopy();//结束绘画状态，准备下次绘画
}

bool Canvas_GL::isDrawingFigure()
{
    if(this->figureMode == LINE || this->figureMode == CYCLE || this->figureMode == ELLIPSE || this->figureMode == POLYGON || this->figureMode == CURVE){
        return true;
    }else{
        return false;
    }
}

void Canvas_GL::animeDraw(){
    QPainter *painter = new QPainter();
    Sleep(10);
    *pix = *pixToMove;
    painter->begin(pix);
    painter->setPen(pen);

    printDebugMessage("drawCurve 点击");

    curveController.drawFirstMovingCurve(painter,pen);
    curveController.drawHandle(painter,pen);
    painter->end();
    delete painter;
    repaint();
}

void Canvas_GL::drawCurve()
{
    //没有曲线，就不能画
    if(curveController.isNullCurve()){
        return;
    }

    //不能再次绘制
    if(curveController.getIsSettingPoints()==false){
        return;
    }

    //动画版本
    for(double i=0;i<1;i+=0.01){
        animeDraw();
    }
    curveController.closeSettingPoints();

    //无动画版本
    QPainter *painter = new QPainter();
    *pix = *pixToMove;
    painter->begin(pix);
    painter->setPen(pen);

    printDebugMessage("drawCurve 点击");

    curveController.drawCurve(painter,pen);
    curveController.drawHandle(painter,pen);
    curveController.closeSettingPoints();
    painter->end();
    delete painter;
    update();
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
    //drawBeforeNewState();
    //setMode(this->figureMode);//(颜色bug在12/19)
    //this->drawBeforeNewState(); Maybe bug here
    color = c;
    pen.setColor(color);
}

void Canvas_GL::paintGL(){
    qDebug() << "Function: paintGL()" << endl;
     glClear(GL_COLOR_BUFFER_BIT);//清屏
 }
