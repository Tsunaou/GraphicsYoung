#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("YoungPaint"));//设置软件标题为YoungPaint

    cur_canvasNum = -1;
    figureMode = LINE;
    initMdiArea();
    initColorSelection();


}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getCurCanvasNum()//返回当前画布序号
{
    int i = this->WindMap.find(this->ui->mdiArea->activeSubWindow()).value();
    qDebug()<<"This index is"<<i<<endl;
    return i;
}

void MainWindow::initMdiArea()
{
    setCentralWidget(ui->mdiArea);
    ui->mdiArea->setViewMode(QMdiArea::TabbedView); //设置视口模式：tabBar模式
    ui->mdiArea->setTabPosition(QTabWidget::North); //设置tabBar的位置
    ui->mdiArea->setTabShape(QTabWidget::Rounded); //设置tab形状
    ui->mdiArea->setTabsClosable(true); //tab可关闭
    ui->mdiArea->setTabsMovable(true); //tab可在tabBar拖动
}

void MainWindow::initColorSelection()
{
    colorBtn =new QToolButton;                  //创建颜色选择控件
    QPixmap pixmap(20,20);
    pixmap.fill(Qt::black);
    colorBtn->setIcon(QIcon(pixmap));
    connect(colorBtn,SIGNAL(clicked()),this,SLOT(ShowColor()));

    ui->mainToolBar->addWidget(colorBtn);


    colorSel = new ColorPanel(this);
    ui->toolColorBar->addWidget(colorSel);

}

void MainWindow::setFigureMode(FIGURE_TYPE type)
{
    if(type==PEN){
        this->ui->actionPen->setChecked(true);
    }
    this->figureMode = type;
    for(int i=0;i<canvases.size();i++){
        canvases[i]->setMode(type);
    }
}

void MainWindow::clearCheckedState()
{
    ui->actionPen->setChecked(false);
    ui->actionBrush->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionCurve->setChecked(false);
    ui->actionCycle->setChecked(false);
    ui->actionEllipse->setChecked(false);
    ui->actionPolygon->setChecked(false);
    ui->actionFiller->setChecked(false);
}

void MainWindow::on_actionNew_triggered()
{
    Canvas_GL *cv = new Canvas_GL(this);
    canvases.push_back(cv);
    this->cur_canvasNum++;//表示新增画布的序号
    QMdiSubWindow *w = ui->mdiArea->addSubWindow(cv);
    this->WindMap.insert(w,cur_canvasNum);

    ui->mdiArea->setActiveSubWindow(w);
    w->setWindowTitle(tr("NewCanvas%1").arg(ui->mdiArea->subWindowList().size()));
    w->show();
}

void MainWindow::ShowColor()
{
    QColor color = QColorDialog::getColor(static_cast<int> (Qt::black), this);
    //使用标准颜色对话框QColorDialog获得一个颜色值
    if(color.isValid())
    {
       //将新选择的颜色传给绘制区，用于改变画笔的颜色值        
        for(int i=0;i<canvases.size();i++){
            canvases[i]->setColor(color);
        }
        QPixmap p(20,20);
        p.fill(color);
        colorBtn->setIcon(QIcon(p));		//更新颜色选择按钮上的颜色显示
    }
}

void MainWindow::on_actionSave_triggered()
{
    int index = this->getCurCanvasNum();
    this->canvases[index]->saveImage();
}

void MainWindow::on_actionClear_triggered()
{
    int index = this->getCurCanvasNum();
    this->canvases[index]->clearImage();
}

void MainWindow::on_actionRecall_triggered()
{
    int index = this->getCurCanvasNum();
    this->canvases[index]->recallImage();
}

void MainWindow::on_actionLine_triggered()
{
    clearCheckedState();
    this->setFigureMode(LINE);
    ui->actionLine->setChecked(true);
}

void MainWindow::on_actionCycle_triggered()
{
    clearCheckedState();
    this->setFigureMode(CYCLE);
    ui->actionCycle->setChecked(true);
}

void MainWindow::on_actionCurve_triggered()
{
    clearCheckedState();
    this->setFigureMode(LINE); //TODO
    ui->actionCurve->setChecked(true);

}

void MainWindow::on_actionEllipse_triggered()
{
    clearCheckedState();
    this->setFigureMode(ELLIPSE);
    ui->actionEllipse->setChecked(true);
}

void MainWindow::on_actionPen_triggered()
{
    clearCheckedState();
    this->setFigureMode(PEN);
    ui->actionPen->setChecked(true);
}

void MainWindow::on_actionBrush_triggered()
{
    clearCheckedState();
    this->setFigureMode(BRUSH);
    ui->actionBrush->setChecked(true);
}

void MainWindow::on_actionBigger_triggered()
{
    int index = this->getCurCanvasNum();
    this->canvases[index]->setBigger();
}

void MainWindow::on_actionSmaller_triggered()
{
    int index = this->getCurCanvasNum();
    this->canvases[index]->setSmaller();
}

void MainWindow::on_actionPolygon_triggered()
{
    clearCheckedState();
    this->setFigureMode(POLYGON);
    ui->actionPolygon->setChecked(true);
}

void MainWindow::on_actionFiller_triggered()
{
    clearCheckedState();
    this->setFigureMode(FILLER);
    ui->actionFiller->setChecked(true);
}

void MainWindow::on_actionOpen_triggered()
{
    qDebug()<<"Open"<<endl;
}

void MainWindow::on_actionCutter_toggled(bool arg1)
{
    qDebug()<<"toggle"<<endl;
    if(arg1){
        qDebug()<<"State on"<<endl;
    }else{
        qDebug()<<"State close"<<endl;
    }
}
