#include "shadertexture.h"

namespace{
    constexpr int A_VER = 3;
    constexpr int T_VER = 4;
    constexpr int YUVdataWidth = 240;
    constexpr int YUVdataHeight = 128;
}

shaderGLWidget::shaderGLWidget(QOpenGLWidget *parent) : QOpenGLWidget(parent)
{
    mRenderTimer = make_unique<QTimer>();
}

shaderGLWidget::~shaderGLWidget()
{
    mShaderProgram->release();
}

void shaderGLWidget::initializeGL()
{
   mProgramId = createGPUProgram("texturevertexshader.vert", "texturefragmentshader.frag");
   if (mProgramId <= 0) {
       return;
   }

   //mOpenGLCore is a reference from currentContext, it will be recycled automatically by context.
   //Don't delete it manually
   mOpenGLCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();

   mOpenGLCore->initializeOpenGLFunctions();
   mOpenGLCore->glEnable(GL_DEPTH_TEST);
   mOpenGLCore->glDepthFunc(GL_LEQUAL);

   //Vertex Coordinate
   static const GLfloat vertexCoordinate[] = {
           -1.0f,-1.0f,
           1.0f,-1.0f,
           -1.0f, 1.0f,
           1.0f,1.0f
   };

   //Texture Coordinate
   static const GLfloat textureCoordinate[] = {
       0.0f, 1.0f,
       1.0f, 1.0f,
       0.0f, 0.0f,
       1.0f, 0.0f
   };

   mOpenGLCore->glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, vertexCoordinate);
   mOpenGLCore->glEnableVertexAttribArray(A_VER);

   mOpenGLCore->glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, textureCoordinate);
   mOpenGLCore->glEnableVertexAttribArray(T_VER);

   unis[0] = mShaderProgram->uniformLocation("tex_y");
   unis[1] = mShaderProgram->uniformLocation("tex_u");
   unis[2] = mShaderProgram->uniformLocation("tex_v");

   //´´½¨²ÄÖÊ
   mOpenGLCore->glGenTextures(3, texs);

   //Y
   mOpenGLCore->glBindTexture(GL_TEXTURE_2D, texs[0]);

   mOpenGLCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   mOpenGLCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   mOpenGLCore->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, YUVdataWidth, YUVdataHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

   //U
   mOpenGLCore->glBindTexture(GL_TEXTURE_2D, texs[1]);

   mOpenGLCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   mOpenGLCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   //´´½¨²ÄÖÊÏÔ¿¨¿Õ¼ä
   mOpenGLCore->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, YUVdataWidth/2, YUVdataHeight/2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

   //V
   mOpenGLCore->glBindTexture(GL_TEXTURE_2D, texs[2]);
   //·Å´ó¹ýÂË£¬ÏßÐÔ²åÖµ
   mOpenGLCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   mOpenGLCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   //´´½¨²ÄÖÊÏÔ¿¨¿Õ¼ä
   mOpenGLCore->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, YUVdataWidth/2, YUVdataHeight/2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

   ///·ÖÅä²ÄÖÊÄÚ´æ¿Õ¼ä
   YUVdata[0] = make_unique<unsigned char[]>(YUVdataWidth*YUVdataHeight);   //Y
   YUVdata[1] = make_unique<unsigned char[]>(YUVdataWidth*YUVdataHeight/4);	//U
   YUVdata[2] = make_unique<unsigned char[]>(YUVdataWidth*YUVdataHeight/4);	//V

   //load yuv file
   mfpYUVdata.reset(fopen("./data/out240x128.yuv","rb"));

   //unique_ptr can check empty directly
   if (!mfpYUVdata) {
       QMessageBox::information(this, "yuv fail", "out240x128.yuv file open failed!");
   }

   QObject::connect(mRenderTimer.get(), SIGNAL(timeout()), this, SLOT(onTimerOut()));
   mRenderTimer->start(33);
}

void shaderGLWidget::resizeGL(int w, int h)
{
   mOpenGLCore->glViewport(0, 0, (GLint)w, (GLint)h);

   //perspertive matrix should be reset everytime here because resize will be called many times
   //mProjectionNormalMatrix.setToIdentity();
   //mProjectionNormalMatrix.perspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
}

void shaderGLWidget::paintGL()
{
   if (mProgramId<=0)
       return;

   mOpenGLCore->glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
   mOpenGLCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if(feof(mfpYUVdata.get())) {
      fseek(mfpYUVdata.get(), 0, SEEK_SET);
   }
   fread(YUVdata[0].get(), 1, YUVdataWidth*YUVdataHeight, mfpYUVdata.get());
   fread(YUVdata[1].get(), 1, YUVdataWidth*YUVdataHeight/4, mfpYUVdata.get());
   fread(YUVdata[2].get(), 1, YUVdataWidth*YUVdataHeight/4, mfpYUVdata.get());

   mOpenGLCore->glActiveTexture(GL_TEXTURE0);
   mOpenGLCore->glBindTexture(GL_TEXTURE_2D, texs[0]);

   mOpenGLCore->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, YUVdataWidth, YUVdataHeight, GL_RED, GL_UNSIGNED_BYTE, YUVdata[0].get());

   mOpenGLCore->glUniform1i(unis[0], 0);


   mOpenGLCore->glActiveTexture(GL_TEXTURE0+1);
   mOpenGLCore->glBindTexture(GL_TEXTURE_2D, texs[1]);

   mOpenGLCore->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, YUVdataWidth/2, YUVdataHeight/2, GL_RED, GL_UNSIGNED_BYTE, YUVdata[1].get());

   mOpenGLCore->glUniform1i(unis[1],1);


   mOpenGLCore->glActiveTexture(GL_TEXTURE0+2);
   mOpenGLCore->glBindTexture(GL_TEXTURE_2D, texs[2]);

   mOpenGLCore->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, YUVdataWidth/2, YUVdataHeight/2, GL_RED, GL_UNSIGNED_BYTE, YUVdata[2].get());

   mOpenGLCore->glUniform1i(unis[2], 2);

   mOpenGLCore->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void shaderGLWidget::onTimerOut(void)
{
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

    //Bind Attribute is for texture only
    mShaderProgram->bindAttributeLocation("vertexIn", A_VER);
    mShaderProgram->bindAttributeLocation("textureIn", T_VER);

    mShaderProgram->link();

    //only for texture to bind()
    mShaderProgram->bind();

    return mShaderProgram->programId();
}
