#include "shaderglwidget.h"

shaderGLWidget::shaderGLWidget(QWidget *parent) : QGLWidget(parent)
{
    
}

void shaderGLWidget::initializeGL()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);
   glClearDepth(1.0);
   glEnable(GL_DEPTH_TEST);
}

void shaderGLWidget::resizeGL(int w, int h)
{
   glViewport(0, 0, (GLint)w, (GLint)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void shaderGLWidget::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   glTranslatef(-2.0, 0.0, -6.0);
   glBegin(GL_TRIANGLES);
   glColor3f(1.0, 0.0, 0.0);
   glVertex3f(0.0, 1.0, 0.0);
   glColor3f(0.0, 1.0, 0.0);
   glVertex3f(-1.0, -1.0, 0.0);
   glColor3f(0.0, 0.0, 1.0);
   glVertex3f(1.0, -1.0, 0.0);
   glEnd();

   glTranslatef(4.0, 0.0, 0.0);
   glBegin(GL_QUADS);
   glColor3f(1.0, 1.0, 0.0);
   glVertex3f(-1.0, 1.0, 0.0);
   glVertex3f(1.0, 1.0, 0.0);
   glVertex3f(1.0, -1.0, 0.0);
   glVertex3f(-1.0, -1.0, 0.0);
   glEnd();
}
