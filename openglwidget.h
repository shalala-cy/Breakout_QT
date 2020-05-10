#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

//forward declaration
class GameObject;
class Brick;
class Ball;
class GameLevel;
class QTimer;
class ParticleManager;

class OpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(GLint w, GLint h) override;

public:
    OpenglWidget(QWidget *parent = nullptr);
    ~OpenglWidget();
    void keyPressEvent(QKeyEvent *event) override;
    void drawObj(GameObject* object);
    void drawObj(Brick* object);
    void drawObj(GameLevel* level);
    void drawObj(ParticleManager* particleManager);
    void doCollision();
    void updateGame();
private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLShaderProgram parcileShaderProgram;
    QOpenGLShader* recShader_V;
    QOpenGLShader* recShader_F;
    QOpenGLShader* particleShader_V;
    QOpenGLShader* particleShader_F;   
    //Game Object
    GameLevel* level;
    Ball* ball;
    Brick* paddle;
    ParticleManager* particleManager;
    //timer for updating game
    QTimer* timer;
};

#endif // OPENGLWIDGET_H
