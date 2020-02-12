#include "shaderglwidget.h"

shaderGLWidget::shaderGLWidget(QOpenGLWidget *parent) : QOpenGLWidget(parent)
{
    mTimer = make_unique<QTimer>();
}

void shaderGLWidget::initializeGL()
{
   mProgramId = createGPUProgram("trianglevertexshader.vert", "trianglefragmentshader.frag");
   if (mProgramId <= 0) {
       return;
   }

   //mOpenGLCore is a reference from currentContext, it will be recycled automatically by context.
   //Don't delete it manually
   mOpenGLCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();

   VertexInfo triangleVertexInfo[3]= {{{0.0, 1.0, 0.0}, {1.0, 0.0, 0.0, 1.0}},
                                      {{-1.0, -1.0, 0.0}, {0.0, 1.0, 0.0, 1.0}},
                                      {{1.0, -1.0, 0.0}, {0.0, 0.0, 1.0, 1.0}}};

   //Create VBO
   mOpenGLCore->glGenBuffers(1, &mVBO);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   mOpenGLCore->glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * 3, triangleVertexInfo, GL_STATIC_DRAW);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);
   //create IBO
   unsigned int index[3] = {0, 1, 2};
   mOpenGLCore->glGenBuffers(1, &mIBO);
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
   mOpenGLCore->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);

   mOpenGLCore->glClearColor(0.0, 0.0, 0.0, 0.0);
   //glShadeModel(GL_SMOOTH);
   //glClearDepth(1.0);
   //glEnable(GL_DEPTH_TEST);
   QObject::connect(mTimer.get(), SIGNAL(timeout()), this, SLOT(onTimerOut()));
   mTimer->start(500);
}

void shaderGLWidget::resizeGL(int w, int h)
{
   mOpenGLCore->glViewport(0, 0, (GLint)w, (GLint)h);

   //perspertive matrix should be reset everytime
   mPnormalMat.setToIdentity();
   mPnormalMat.perspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
}

void shaderGLWidget::paintGL()
{
   if (mProgramId<=0 || !mShaderProgram->bind())
       return;

   mOpenGLCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

   //Get Uniform ID & Attribute ID
   mMLocationMat = mOpenGLCore->glGetUniformLocation(mProgramId, "M");
   mVLocationMat = mOpenGLCore->glGetUniformLocation(mProgramId, "V");
   mPLocationMat = mOpenGLCore->glGetUniformLocation(mProgramId, "P");
   mPosVector = mOpenGLCore->glGetAttribLocation(mProgramId, "pos");
   mColorVector = mOpenGLCore->glGetAttribLocation(mProgramId, "color");

   QMatrix4x4 nMnormalMat, nVnormalMat;
   //Model Matrix, rotate and then translate
   nMnormalMat.translate(-2.0, 0.0, -6.0);
   nMnormalMat.rotate(mRotateAngle, 0, 0, 1);

   mOpenGLCore->glUniformMatrix4fv(mMLocationMat, 1, GL_FALSE, nMnormalMat.data());
   mOpenGLCore->glUniformMatrix4fv(mVLocationMat, 1, GL_FALSE, nVnormalMat.data());
   mOpenGLCore->glUniformMatrix4fv(mPLocationMat, 1, GL_FALSE, mPnormalMat.data());

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

void shaderGLWidget::onTimerOut(void)
{
    mRotateAngle = (mRotateAngle+90) % 360;
    repaint();
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
