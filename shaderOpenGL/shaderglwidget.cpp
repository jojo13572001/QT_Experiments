#include "shaderglwidget.h"

shaderGLWidget::shaderGLWidget(QOpenGLWidget *parent) : QOpenGLWidget(parent)
{
    mRenderTimer = make_unique<QTimer>();
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

   //Triangle model
   VertexInfo triangleVertexInfo[3]= {{{0.0, 1.0, 0.0}, {1.0, 0.0, 0.0, 1.0}},
                                      {{-1.0, -1.0, 0.0}, {0.0, 1.0, 0.0, 1.0}},
                                      {{1.0, -1.0, 0.0}, {0.0, 0.0, 1.0, 1.0}}};

   //Quad model
   VertexInfo quadVertexInfo[4]= {{{-1.0, 1.0, 0.0}, {1.0, 0.0, 0.0, 1.0}},
                                  {{1.0, 1.0, 0.0}, {1.0, 0.0, 0.0, 1.0}},
                                  {{1.0, -1.0, 0.0}, {0.0, 0.0, 1.0, 1.0}},
                                  {{-1.0, -1.0, 0.0}, {0.0, 0.0, 1.0, 1.0}}};

   //Create Triangle VBO
   mOpenGLCore->glGenBuffers(1, &mTriangleVBO);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
   mOpenGLCore->glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * 3, triangleVertexInfo, GL_STATIC_DRAW);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);
   //create Triangle IBO
   unsigned int triangleIndex[3] = {0, 1, 2};
   mOpenGLCore->glGenBuffers(1, &mTriableIBO);
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriableIBO);
   mOpenGLCore->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndex), triangleIndex, GL_STATIC_DRAW);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);


   //Create Quad VBO
   mOpenGLCore->glGenBuffers(1, &mQuadVBO);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
   mOpenGLCore->glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * 4, quadVertexInfo, GL_STATIC_DRAW);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);
   //create Quad IBO
   unsigned int quadIndex[4] = {0, 1, 2, 3};
   mOpenGLCore->glGenBuffers(1, &mQuadIBO);
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadIBO);
   mOpenGLCore->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndex), quadIndex, GL_STATIC_DRAW);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);

   //glShadeModel(GL_SMOOTH);
   mOpenGLCore->glClearColor(0.0, 0.0, 0.0, 0.0);
   mOpenGLCore->glClearDepth(1.0);
   mOpenGLCore->glEnable(GL_DEPTH_TEST);
   QObject::connect(mRenderTimer.get(), SIGNAL(timeout()), this, SLOT(onTimerOut()));
   mRenderTimer->start(33);
}

void shaderGLWidget::resizeGL(int w, int h)
{
   mOpenGLCore->glViewport(0, 0, (GLint)w, (GLint)h);

   //perspertive matrix should be reset everytime here because resize will be called many times
   mProjectionNormalMatrix.setToIdentity();
   mProjectionNormalMatrix.perspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
}

void shaderGLWidget::paintGL()
{
   if (mProgramId<=0 || !mShaderProgram->bind())
       return;

   mOpenGLCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //Get Uniform ID & Attribute ID
   mModelId = mOpenGLCore->glGetUniformLocation(mProgramId, "M");
   mViewId = mOpenGLCore->glGetUniformLocation(mProgramId, "V");
   mProjectionId = mOpenGLCore->glGetUniformLocation(mProgramId, "P");
   mPositionId = mOpenGLCore->glGetAttribLocation(mProgramId, "pos");
   mColorId = mOpenGLCore->glGetAttribLocation(mProgramId, "color");

   //--------------------------------Triangle Rendering-----------------------------------
   QMatrix4x4 modelNormalMatrix, viewNormalMatrix;
   //Model Matrix, rotate and then translate
   modelNormalMatrix.translate(-2.0, 0.0, -6.0);
   modelNormalMatrix.rotate(mRotateAngle, 0, 1, 0);

   mOpenGLCore->glUniformMatrix4fv(mModelId, 1, GL_FALSE, modelNormalMatrix.data());
   mOpenGLCore->glUniformMatrix4fv(mViewId, 1, GL_FALSE, viewNormalMatrix.data());
   mOpenGLCore->glUniformMatrix4fv(mProjectionId, 1, GL_FALSE, mProjectionNormalMatrix.data());

   // DrawArrays paint Triangle
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
   mOpenGLCore->glEnableVertexAttribArray(mPositionId);
   mOpenGLCore->glVertexAttribPointer(mPositionId, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)0);
   mOpenGLCore->glEnableVertexAttribArray(mColorId);
   mOpenGLCore->glVertexAttribPointer(mColorId, 4, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)(sizeof(float) * 3));
   //mOpenGLCore->glDrawArrays(GL_TRIANGLES, 0, 3);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);

   //DrawElement paint Triangle
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriableIBO);
   mOpenGLCore->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   //--------------------------------Quad Rendering-----------------------------------

   //Set Quad posion
   modelNormalMatrix.setToIdentity();
   modelNormalMatrix.translate(2.0, 0.0, -6.0);
   modelNormalMatrix.rotate(mRotateAngle, 1, 0, 0);
   mOpenGLCore->glUniformMatrix4fv(mModelId, 1, GL_FALSE, modelNormalMatrix.data());

   //DrawArrays paint Quad
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
   mOpenGLCore->glEnableVertexAttribArray(mPositionId);
   mOpenGLCore->glVertexAttribPointer(mPositionId, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)0);
   mOpenGLCore->glEnableVertexAttribArray(mColorId);
   mOpenGLCore->glVertexAttribPointer(mColorId, 4, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)(sizeof(float) * 3));
   mOpenGLCore->glDrawArrays(GL_QUADS, 0, 4);
   mOpenGLCore->glBindBuffer(GL_ARRAY_BUFFER, 0);

   //DrawElement paint Quad
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadIBO);
   mOpenGLCore->glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
   mOpenGLCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   mShaderProgram->release();
}

void shaderGLWidget::onTimerOut(void)
{
    mRotateAngle = (mRotateAngle+4)%360;
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
