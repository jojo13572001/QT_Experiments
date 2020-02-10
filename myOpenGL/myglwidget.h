#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <qgl.h>
#include <GL/glu.h>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
    
public slots:
};

#endif // MYGLWIDGET_H
