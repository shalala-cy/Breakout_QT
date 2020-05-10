#include "openglwidget.h"

#include "game_object.h"
#include "game_brick.h"
#include "game_ball.h"
#include "game_level.h"
#include "particle_manager.h"
#include "texture.h"

#include <QImage>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <iostream>
#include <QTimer>
#include <tuple>
#include <QDebug>

enum Dirction{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

unsigned int VAO, VBO, textureSolid, textureBlock, texturePaddle, textureBall, textureBgd, textureParticle;
extern const float WINDOW_WIDTH=1100.0, WINDOW_HEIGHT=680.0;
const float paddleSpeed=  8.0;
GameObject background(QVector2D(0.0, 0.0), QVector2D(WINDOW_WIDTH, WINDOW_HEIGHT), &textureBgd, QVector3D(1.0, 1.0, 1.0));

typedef std::tuple<bool, Dirction, QVector2D> Collision;
Collision checkCollision(Ball* ball, Brick* brick);

GLfloat vertices[] = {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};
GLfloat verticesb[] = {
    // Pos      // Tex
    -1.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, 0.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f
};


Dirction vectorDirection(QVector2D dir);

void OpenglWidget::initializeGL(){

    this->initializeOpenGLFunctions();
    //shader
    recShader_V = new QOpenGLShader(QOpenGLShader::Vertex);
    recShader_F = new QOpenGLShader(QOpenGLShader::Fragment);

    particleShader_V = new QOpenGLShader(QOpenGLShader::Vertex);
    particleShader_F = new QOpenGLShader(QOpenGLShader::Fragment);


    bool success=particleShader_F->compileSourceFile("shader/particle.f.txt");
    if(!success){
        qDebug()<<" Particle Fragment Shader compileSourceFile failed:"<<particleShader_F->log();
    }
    success=particleShader_V ->compileSourceFile("shader/particle.v.txt");
        if(!success){
            qDebug()<<"Particle Vertex Shader compileSourceFile failed:"<<particleShader_V->log();
        }
    parcileShaderProgram.addShader(particleShader_V);
    parcileShaderProgram.addShader(particleShader_F);
    success=parcileShaderProgram.link();
    if(!success){
        qDebug()<<"particleShaderPrograme link failed :"<<parcileShaderProgram.log();
    }

    success=recShader_F->compileSourceFile("shader/recShader_F.txt");
    if(!success){
        qDebug()<<" Rec Fragment Shader compileSourceFile failed:"<<recShader_F->log();
    }
    success=recShader_V ->compileSourceFile("shader/recShader_V.txt");
        if(!success){
            qDebug()<<"Rec Vertex Shader compileSourceFile failed:"<<recShader_V->log();
        }
    shaderProgram.addShader(recShader_V);
    shaderProgram.addShader(recShader_F);
    success=shaderProgram.link();
    if(!success){
        qDebug()<<"ShaderPrograme link failed :"<<shaderProgram.log();
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4 , GL_FLOAT, GL_FALSE, 4 * sizeof (GL_FLOAT), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Texture
    TextureCreator textureCreator;
    textureCreator.create2DTexture("texture/block.png", textureBlock);
    textureCreator.create2DTexture("texture/block_solid.png", textureSolid);
    textureCreator.create2DTexture("texture/paddle.png", texturePaddle);
    textureCreator.create2DTexture("texture/ball.jpg", textureBall);
    textureCreator.create2DTexture("texture/background.png", textureBgd);
    textureCreator.create2DTexture("texture/particle.png", textureParticle);

    shaderProgram.bind();
    shaderProgram.setUniformValue("image", 0);

    //Translation
    QMatrix4x4 projection;
    projection.ortho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, -1.0, 1.0);
    shaderProgram.bind();
    shaderProgram.setUniformValue("projection", projection);

    parcileShaderProgram.bind();
    parcileShaderProgram.setUniformValue("projection", projection);

    //OPengl config
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //init game objects
    //Level
    level = new GameLevel();
    level->load("levels/one.txt", WINDOW_WIDTH, WINDOW_HEIGHT);
    //paddle
    QVector2D paddleSize=QVector2D(110.0, 22.0);
    QVector2D paddlePos=QVector2D(0.5 * WINDOW_WIDTH, WINDOW_HEIGHT-paddleSize.y());
    paddle = new Brick(paddlePos, paddleSize, &texturePaddle, QVector3D(1.0, 1.0, 1.0), false, false);
    //ball
    float radiusBall=12.0;
    QVector2D ballPos=QVector2D(paddlePos.x()+paddleSize.x()*0.5 - radiusBall, paddlePos.y() - 2*radiusBall);
    ball = new Ball(ballPos, radiusBall, &textureBall, QVector3D(1.0, 1.0, 0.0));
    //particle
    particleManager = new ParticleManager(textureParticle, 500);
}

void OpenglWidget::paintGL(){
    drawObj(&background);
    drawObj(level);
    drawObj(paddle);
    drawObj(particleManager);
    drawObj(static_cast<GameObject*>(ball));
}

void OpenglWidget::resizeGL(GLint w, GLint h)
{
    glViewport(0,0,w,h);
}

//Response for keyevent
void OpenglWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_A){
        float leftPos=paddle->pos.x()-paddleSpeed;
        leftPos=leftPos<0?0:leftPos;
        paddle->pos = QVector2D(leftPos, paddle->pos.y());
    }else if(event->key()==Qt::Key_D){
        float rightPos=paddle->pos.x() + paddleSpeed;
        rightPos = rightPos > WINDOW_WIDTH-paddle->size.x() ? WINDOW_WIDTH-paddle->size.x():rightPos;
        paddle->pos = QVector2D(rightPos, paddle->pos.y());
    }else if(event->key()==Qt::Key_Space){
        ball->IsStuck = false;
    }else if(event->key()==Qt::Key_P){
        disconnect(timer, &QTimer::timeout, this, &OpenglWidget::updateGame);
    }else if(event->key()==Qt::Key_R){
        connect(timer, &QTimer::timeout, this, &OpenglWidget::updateGame);
    }
}

void OpenglWidget::drawObj(GameObject* object)
{
    shaderProgram.bind();
    glBindVertexArray(VAO);
    QMatrix4x4 model;
    model.translate(QVector3D(object->pos,1.0));
    model.scale(QVector3D(object->size,1.0));
    shaderProgram.setUniformValue("model", model);
    shaderProgram.setUniformValue("Color", object->color);
    glBindTexture(GL_TEXTURE_2D, *(object->texture));
    glDrawArrays(GL_TRIANGLES,0,6);
    shaderProgram.release();
}

void OpenglWidget::drawObj(Brick* object)
{
    if(object->isDestroyed())
        return;
    drawObj(static_cast<GameObject*>(object));
}

void OpenglWidget::drawObj(GameLevel *level)
{
    for(Brick brick : level->bricks)
        drawObj(&brick);
}

void OpenglWidget::drawObj(ParticleManager *particleManager)
{
    parcileShaderProgram.bind();
    // Use  blending to give it glow effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindVertexArray(VAO);
    for (Particle particle : particleManager->particles)
    {
        if (particle.Life > 0.0f)
        {
            parcileShaderProgram.setUniformValue("offset", particle.Position);
            parcileShaderProgram.setUniformValue("color", particle.Color);
            glBindTexture(GL_TEXTURE_2D, particleManager->texture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenglWidget::doCollision()
{
    for(Brick& brick: this->level->bricks)
        if(!brick.isDestroyed()){
            Collision collision= checkCollision(ball, &brick);
            if(std::get<0>(collision)){
                if(!brick.isSolid())
                    brick.destroyBrick();
                //do collision
                Dirction dir= std::get<1>(collision);
                if(dir==LEFT || dir==RIGHT){                            //horizontal collisin
                    ball->Velocity=QVector2D(-ball->Velocity.x(), ball->Velocity.y());
                }else{                                                  //vertical collision
                    ball->Velocity=QVector2D(ball->Velocity.x(), -ball->Velocity.y());
                }
            }
        }
    //do  ball-to-paddle collision
    Collision result= checkCollision(ball, paddle);\
    if(!ball->IsStuck && std::get<0>(result)){
        qDebug()<<"paddle collision"<<endl;
        float  centerPaddle=paddle->pos.x() + paddle->size.x()*0.5;
        float distance=ball->pos.x()+ball->radius - centerPaddle;
        float percentage=0.8 * distance/(paddle->size.x()*0.5);

        ball->Velocity=QVector2D(percentage, -(1-abs(percentage))).normalized() * ball->movSpeed();
    }

}

void OpenglWidget::updateGame()
{
    ball->move(paddle);
    doCollision();
    particleManager->Update(0.03, ball, 8, QVector2D(ball->radius*0.5,ball->radius*0.5));
    update();
}


OpenglWidget::OpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent), timer(new QTimer())
{
    this->resize(QSize(WINDOW_WIDTH,WINDOW_HEIGHT));
    timer->setInterval(10);
    connect(timer, &QTimer::timeout, this, &OpenglWidget::updateGame);
    timer->start();
}

OpenglWidget::~OpenglWidget()
{
    delete level;
    delete ball;
    delete paddle;
    delete recShader_F;
    delete recShader_V;
    delete timer;
}

//to get the dirction of a vector
Dirction vectorDirection(QVector2D dir){
    QVector2D compass[]={
        QVector2D(0.0, 1.0),
        QVector2D(1.0, 0.0),
        QVector2D(0.0, -1.0),
        QVector2D(-1.0, 0.0)
    };
    float max=0;
    int match=0;
    for(int i=0;i<4;i++){
        float product = QVector2D::dotProduct(dir,compass[i]);
        if(product>max){
            match=i;
            max=product;
        }
    }
    return Dirction(match);
}

//check collision and return the collision result
Collision checkCollision(Ball* ball, Brick* brick){
    //center of ball
    QVector2D centerBall(ball->pos.x()+ball->radius, ball->pos.y()+ball->radius);
    //center of brick;
    float recHalfX=0.5*brick->size.x();
    float recHalfY=0.5*brick->size.y();
    QVector2D centerBrick(brick->pos.x()+recHalfX, brick->pos.y()+recHalfY);
    //
    QVector2D difference=centerBall - centerBrick;
    //clamp difference
    float clampedX=difference.x();
    float clampedY=difference.y();
    if(clampedX<-recHalfX){
        clampedX=-recHalfX;
    }else if(clampedX>recHalfX){
        clampedX=recHalfX;
    }
    if(clampedY<-recHalfY){
        clampedY=-recHalfY;
    }else if(clampedY>recHalfY){
        clampedY=recHalfY;
    }
    //closet point to Ball
    QVector2D closet=centerBrick+QVector2D(clampedX,clampedY);
    difference=closet - centerBall;
    if(difference.length()<=ball->radius)
        return  std::make_tuple(true,vectorDirection(difference),difference);
    else
        return  std::make_tuple(false, UP, QVector2D(0.0, 0.0));
}


