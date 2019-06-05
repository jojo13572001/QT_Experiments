#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
     //?性??
    QLinearGradient linearGradient(QPointF(40, 190), QPointF(70, 190));
    //插入?色
    linearGradient.setColorAt(0, Qt::yellow);
    linearGradient.setColorAt(0.5, Qt::red);
    linearGradient.setColorAt(1, Qt::green);
    //指定??区域以外的区域的?散方式
    linearGradient.setSpread(QGradient::RepeatSpread);
    //使用??作?画刷
    QPainter painter(this);
    painter.setBrush(linearGradient);
    painter.drawRect(10, 20, 90, 40);
}
