#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("YoungPaint"));//设置软件标题为YoungPaint

    setCentralWidget(ui->mdiArea);
    ui->mdiArea->setViewMode(QMdiArea::TabbedView); //设置视口模式：tabBar模式
    ui->mdiArea->setTabPosition(QTabWidget::North); //设置tabBar的位置
    ui->mdiArea->setTabShape(QTabWidget::Rounded); //设置tab形状
    ui->mdiArea->setTabsClosable(true); //tab可关闭
    ui->mdiArea->setTabsMovable(true); //tab可在tabBar拖动



//    QWidget *widgetAppearance=new QWidget();
//        widgetAppearance->setFixedSize(QSize(60,60));
//        QCheckBox *checkBoxWeiWu=new QCheckBox(widgetAppearance);
//        checkBoxWeiWu->setGeometry(QRect(5,12,60,15));
//        checkBoxWeiWu->setText("欧阳鸿荣");

//        QCheckBox *checkBoxXiongZhuang=new QCheckBox(widgetAppearance);
//        checkBoxXiongZhuang->setGeometry(QRect(5,32,60,15));
//        checkBoxXiongZhuang->setText("荣鸿阳欧");

//        //在“关于”按钮之前插入widget
//        ui->mainToolBar->insertWidget(ui->actionCurve,widgetAppearance);
//        //在“关于”按钮之前插入分隔符
//        ui->mainToolBar->insertSeparator(ui->actionCurve);


    colorBtn =new QToolButton;                  //创建颜色选择控件
    QPixmap pixmap(20,20);
    pixmap.fill(Qt::black);
    colorBtn->setIcon(QIcon(pixmap));
    connect(colorBtn,SIGNAL(clicked()),this,SLOT(ShowColor()));

    ui->mainToolBar->addWidget(colorBtn);

    cur_canvasNum = -1;

    ColorPanel *p = new ColorPanel(this);
    ui->toolColorBar->addWidget(p);
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
