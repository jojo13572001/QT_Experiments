#ifndef BASICGLWIDGET_H
#define BASICGLWIDGET_H

#include <QWidget>
#include <qgl.h>
#include <GL/glu.h>

class shaderGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit shaderGLWidget(QWidget *parent = 0);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
    
public slots:
};

#endif // MYGLWIDGET_H
