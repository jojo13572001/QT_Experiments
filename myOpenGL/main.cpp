#include <QApplication>
#include "basicglwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BasicGLWidget w;
    w.resize(400, 300);
    w.show();
    
    return a.exec();
}
