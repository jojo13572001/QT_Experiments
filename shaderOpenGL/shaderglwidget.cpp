#include "shaderglwidget.h"

shaderGLWidget::shaderGLWidget(QOpenGLWidget *parent) : QOpenGLWidget(parent)
{
    
}

void shaderGLWidget::initializeGL()
{
   createGPUProgram("trianglevertexshader.vert", "trianglefragmentshader.frag");

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

GLuint shaderGLWidget::createGPUProgram(const QString& vertexShaderFile, const QString& fragmentShaderFile)
{
    mVertexShader = make_unique<QOpenGLShader>(QOpenGLShader::Vertex);

    if (!mVertexShader->compileSourceFile(vertexShaderFile))
    {
        mVertexShader.reset();
        return 0;
    }

    mFragmentShader = make_unique<QOpenGLShader>(QOpenGLShader::Fragment);
    if (!mFragmentShader->compileSourceFile(fragmentShaderFile))
    {
        mVertexShader.reset();
        mFragmentShader.reset();
        return 0;
    }

    mShaderProgram = make_unique<QOpenGLShaderProgram>(this);
    mShaderProgram->addShader(mVertexShader.get());
    mShaderProgram->addShader(mFragmentShader.get());
    mShaderProgram->link();

    return mShaderProgram->programId();
}
