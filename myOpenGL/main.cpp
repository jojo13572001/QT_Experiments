#include <QApplication>
#include "myglwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGLWidget w;
    w.resize(400, 300);
    w.show();
    
    return a.exec();
}
