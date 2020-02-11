#ifndef SHADERGLWIDGET_H
#define SHADERGLWIDGET_H

#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
//#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#include <GL/glu.h>

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

    unique_ptr<QOpenGLFunctions_3_2_Core> mOpenGLCore{nullptr};

    // MVP Location
    GLint mMLocationMat;
    GLint mVLocationMat;
    GLint mPLocationMat;
    GLint mPosVector;
    GLint mColorVector;

    // VBO
    GLuint mVBO;
    // IBO
    GLuint mIBO;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
    
public slots:
};

#endif // MYGLWIDGET_H
