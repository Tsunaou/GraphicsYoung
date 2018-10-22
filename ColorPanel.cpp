#include "ColorPanel.h"
#include <QInputDialog>

ColorPanel::ColorPanel(QWidget* parent):QWidget(parent)
{
    Btn1 =new QPushButton;
    Btn2 =new QPushButton;
    Btn3 =new QPushButton;
    Btn4 =new QPushButton;
//    Btn1->setText(tr("黑"));
//    Btn2->setText(tr("白"));
//    Btn3->setText(tr("红"));
//    Btn4->setText(tr("蓝"));

    QPixmap p1(25,25);
    p1.fill(Qt::black);
    Btn1->setIcon(QIcon(p1));

    p1.fill(Qt::white);
    Btn2->setIcon(QIcon(p1));

    p1.fill(Qt::red);
    Btn3->setIcon(QIcon(p1));

    p1.fill(Qt::blue);
    Btn4->setIcon(QIcon(p1));

    Btn1->resize(10,10);
    Btn2->resize(10,10);
    Btn3->resize(10,10);
    Btn4->resize(10,10);

    Btn1->setStyleSheet("QPushButton{background:black}");
    Btn2->setStyleSheet("QPushButton{background:white}");
    Btn3->setStyleSheet("QPushButton{background:red}");
    Btn4->setStyleSheet("QPushButton{background:blue}");


    mainLayout =new QGridLayout(this);
    mainLayout->addWidget(Btn1,0,0);
    mainLayout->addWidget(Btn2,0,1);
    mainLayout->addWidget(Btn3,1,0);
    mainLayout->addWidget(Btn4,1,1);

    mainLayout->setMargin(10);
    mainLayout->setSpacing(5);
}

