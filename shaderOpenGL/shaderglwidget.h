#ifndef SHADERGLWIDGET_H
#define SHADERGLWIDGET_H

#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLShaderProgram>
#include <QMatrix>
#include <QTimer>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

struct VertexInfo
{
    float pos[3];
    float color[4];
};

class shaderGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit shaderGLWidget(QOpenGLWidget *parent = nullptr);
    ~shaderGLWidget() = default;

private:
    //create a GPU program
    GLuint createGPUProgram(const QString& nVertexShaderFile, const QString& nFragmentShaderFile);

private:
    unique_ptr<QOpenGLShader> mVertexShader{nullptr};
    unique_ptr<QOpenGLShader> mFragmentShader{nullptr};
    unique_ptr<QOpenGLShaderProgram> mShaderProgram{nullptr};

    QOpenGLFunctions_4_3_Core* mOpenGLCore{nullptr};

    // MVP Location
    GLint mMLocationMat;
    GLint mVLocationMat;
    GLint mPLocationMat;
    GLint mPosVector;
    GLint mColorVector;
    QMatrix4x4 mPnormalMat;
    GLint mRotateAngle{0};

    // VBO
    GLuint mVBO;
    // IBO
    GLuint mIBO;

    GLuint mProgramId{0};

    unique_ptr<QTimer> mTimer{nullptr};

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
    
protected slots:
    void onTimerOut(void);
};

#endif // MYGLWIDGET_H
