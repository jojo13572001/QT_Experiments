#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent) : QGLWidget(parent)
{
    
}

void MyGLWidget::initializeGL()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);
   glClearDepth(1.0);
   glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::resizeGL(int w, int h)
{
   glViewport(0, 0, (GLint)w, (GLint)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void MyGLWidget::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   // ?制三角形
   glTranslatef(-2.0, 0.0, -6.0);
   glBegin(GL_TRIANGLES);  
   glVertex3f(0.0, 1.0, 0.0);
   glVertex3f(-1.0, -1.0, 0.0);
   glVertex3f(1.0, -1.0, 0.0);
   glEnd();
   // ?制四?形
   glTranslatef(4.0, 0.0, 0.0);
   glBegin(GL_QUADS);   
   glVertex3f(-1.0, 1.0, 0.0);
   glVertex3f(1.0, 1.0, 0.0);
   glVertex3f(1.0, -1.0, 0.0);
   glVertex3f(-1.0, -1.0, 0.0);
   glEnd();
}
