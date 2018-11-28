#include "ColorPanel.h"
#include <QInputDialog>

ColorPanel::ColorPanel(QWidget* parent):QWidget(parent)
{
    Btn1 =new QPushButton;
    Btn2 =new QPushButton;
    Btn3 =new QPushButton;
    Btn4 =new QPushButton;
    Btn5 =new QPushButton;
    Btn6 =new QPushButton;
    Btn7 =new QPushButton;
    Btn8 =new QPushButton;
    Btn9 =new QPushButton;


    Btn1->resize(10,10);
    Btn2->resize(10,10);
    Btn3->resize(10,10);
    Btn4->resize(10,10);
    Btn5->resize(10,10);
    Btn6->resize(10,10);
    Btn7->resize(10,10);
    Btn8->resize(10,10);
    Btn9->resize(10,10);


    Btn1->setStyleSheet("QPushButton{background:black;}");
    Btn2->setStyleSheet("QPushButton{background:white}");
    Btn3->setStyleSheet("QPushButton{background:red}");
    Btn4->setStyleSheet("QPushButton{background:blue}");
    Btn5->setStyleSheet("QPushButton{background:gray}");
    Btn6->setStyleSheet("QPushButton{background:green}");
    Btn7->setStyleSheet("QPushButton{background:yellow}");
    Btn8->setStyleSheet("QPushButton{background:orange}");
    Btn9->setStyleSheet("QPushButton{background:blue}");


    mainLayout =new QGridLayout(this);
    mainLayout->addWidget(Btn1,0,0);
    mainLayout->addWidget(Btn2,0,1);
    mainLayout->addWidget(Btn3,0,2);
    mainLayout->addWidget(Btn4,1,0);
    mainLayout->addWidget(Btn5,1,1);
    mainLayout->addWidget(Btn6,1,2);
    mainLayout->addWidget(Btn7,2,0);
    mainLayout->addWidget(Btn8,2,1);
    mainLayout->addWidget(Btn9,2,2);

    mainLayout->setMargin(10);
    mainLayout->setSpacing(5);
}

