#include <QApplication>
#include "shaderglwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    shaderGLWidget w;
    w.resize(400, 300);
    w.show();
    
    return a.exec();
}
