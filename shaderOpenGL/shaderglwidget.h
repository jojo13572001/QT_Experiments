#ifndef SHADERGLWIDGET_H
#define SHADERGLWIDGET_H

#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <GL/glu.h>

using namespace std;
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
    unique_ptr<QOpenGLShader> mVertexShader;
    unique_ptr<QOpenGLShader> mFragmentShader;
    unique_ptr<QOpenGLShaderProgram> mShaderProgram;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
    
public slots:
};

#endif // MYGLWIDGET_H
