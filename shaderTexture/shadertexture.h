#ifndef SHADERGLWIDGET_H
#define SHADERGLWIDGET_H

#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShader>
#include <stdio.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLShaderProgram>
#include <QMatrix>
#include <QMessageBox>
#include <QTimer>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

struct VertexInfo
{
    float pos[4];
    float texture[4];
};

class shaderGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit shaderGLWidget(QOpenGLWidget *parent = nullptr);
    ~shaderGLWidget() override;

private:
    //create a GPU program
    GLuint createGPUProgram(const QString& nVertexShaderFile, const QString& nFragmentShaderFile);

private:
    unique_ptr<QOpenGLShader> mVertexShader{nullptr};
    unique_ptr<QOpenGLShader> mFragmentShader{nullptr};
    unique_ptr<QOpenGLShaderProgram> mShaderProgram{nullptr};

    QOpenGLFunctions_4_3_Core* mOpenGLCore{nullptr};

    GLuint mProgramId{0};

    unique_ptr<QTimer> mRenderTimer{nullptr};

    GLuint unis[3]{0};
    GLuint texs[3]{0};

    unique_ptr<unsigned char[]> YUVdata[3]{nullptr};

    unique_ptr<FILE, decltype(&fclose)> mfpYUVdata{nullptr, &fclose};

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
    
protected slots:
    void onTimerOut(void);
};

#endif // MYGLWIDGET_H
