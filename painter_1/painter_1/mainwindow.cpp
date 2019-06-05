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
     //?��??
    QLinearGradient linearGradient(QPointF(40, 190), QPointF(70, 190));
    //����?��
    linearGradient.setColorAt(0, Qt::yellow);
    linearGradient.setColorAt(0.5, Qt::red);
    linearGradient.setColorAt(1, Qt::green);
    //����??���ʳ�Ū���Ū?������
    linearGradient.setSpread(QGradient::RepeatSpread);
    //����??��?���
    QPainter painter(this);
    painter.setBrush(linearGradient);
    painter.drawRect(10, 20, 90, 40);
}
