#ifndef BASICGLWIDGET_H
#define BASICGLWIDGET_H

#include <QWidget>
#include <qgl.h>
#include <GL/glu.h>
/*
 * The most important point is that
 * QGLWidget only exists for compatibility reasons to older codebases.
 * As suggested in the Qt documentation on QGLWidget,
 * starting from Qt 5.4 you should use QOpenGLWidget instead.
 */
class BasicGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit BasicGLWidget(QWidget *parent = 0);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
    
public slots:
};

#endif // MYGLWIDGET_H
