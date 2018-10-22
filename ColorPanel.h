#ifndef COLORPANEL_H
#define COLORPANEL_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

class ColorPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPanel(QWidget *parent = nullptr);

signals:

private slots:

private:
    QPushButton *Btn1;
    QPushButton *Btn2;
    QPushButton *Btn3;
    QPushButton *Btn4;
    QGridLayout *mainLayout;
};

#endif // COLORPANEL_H
