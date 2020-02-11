#include "shaderglwidget.h"

shaderGLWidget::shaderGLWidget(QOpenGLWidget *parent) : QOpenGLWidget(parent)
{
    
}

void shaderGLWidget::initializeGL()
{
   GLuint programId = createGPUProgram("trianglevertexshader.vert", "trianglefragmentshader.frag");
   mOpenGLCore.reset(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>());

   //Get Uniform ID & Attribute ID
   mMLocationMat = mOpenGLCore->glGetUniformLocation(programId, "M");
   mVLocationMat = mOpenGLCore->glGetUniformLocation(programId, "V");
   mPLocationMat = mOpenGLCore->glGetUniformLocation(programId, "P");
   mPosVector = mOpenGLCore->glGetAttribLocation(programId, "pos");
   mColorVector = mOpenGLCore->glGetAttribLocation(programId, "color");

   VertexInfo vertexInfo[3]= {{{0.0, 1.0, 0.0}, {1.0, 0.0, 0.0, 1.0}},
                              {{-1.0, -1.0, 0.0}, {0.0, 1.0, 0.0, 1.0}},
                              {{1.0, -1.0, 0.0}, {0.0, 0.0, 1.0, 1.0}}};

   //Create VBO
   mOpenGLCore->glGenBuffers(1, &mVBO);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   mOpenGLCore->glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * 3, vertexInfo, GL_STATIC_DRAW);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);
   //create IBO
   unsigned int index[3] = {0, 1, 2};
   mOpenGLCore->glGenBuffers(1, &mIBO);
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
   mOpenGLCore->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);

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
   /*
   glBegin(GL_TRIANGLES);
   glColor3f(1.0, 0.0, 0.0);
   glVertex3f(0.0, 1.0, 0.0);
   glColor3f(0.0, 1.0, 0.0);
   glVertex3f(-1.0, -1.0, 0.0);
   glColor3f(0.0, 0.0, 1.0);
   glVertex3f(1.0, -1.0, 0.0);
   glEnd();
   */
    /*
   glTranslatef(4.0, 0.0, 0.0);
   glBegin(GL_QUADS);
   glColor3f(1.0, 1.0, 0.0);
   glVertex3f(-1.0, 1.0, 0.0);
   glVertex3f(1.0, 1.0, 0.0);
   glVertex3f(1.0, -1.0, 0.0);
   glVertex3f(-1.0, -1.0, 0.0);
   glEnd();
   */

   if (!mShaderProgram->bind())
       return;
   QMatrix4x4 nMnormalMat, nVnormalMat, projectionMatrix;

   mOpenGLCore->glUniformMatrix4fv(mMLocationMat, 1, GL_FALSE, nMnormalMat.data());
   mOpenGLCore->glUniformMatrix4fv(mVLocationMat, 1, GL_FALSE, nVnormalMat.data());
   mOpenGLCore->glUniformMatrix4fv(mPLocationMat, 1, GL_FALSE, projectionMatrix.data());

   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, mVBO);

   // DrawArrays paint
   mOpenGLCore->glEnableVertexAttribArray(mPosVector);
   mOpenGLCore->glVertexAttribPointer(mPosVector, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)0);
   mOpenGLCore->glEnableVertexAttribArray(mColorVector);
   mOpenGLCore->glVertexAttribPointer(mColorVector, 4, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)(sizeof(float) * 3));
   //OpenGLCore->glDrawArrays(GL_TRIANGLES, 0, 3);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);

   // DrawElement paint
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
   mOpenGLCore->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   mShaderProgram->release();
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
