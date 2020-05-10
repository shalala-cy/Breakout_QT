#include "game_ball.h"

extern const float WINDOW_WIDTH, WINDOW_HEIGHT;

Ball::Ball(QVector2D position, float R, uint32_t *texture2D, QVector3D objColor) : GameObject(position, QVector2D(R*2, R*2), texture2D, objColor) , radius(R)
{
    moveSpeed = 4.0;
    IsStuck = true;
    Velocity=QVector2D(0.0, -3.0).normalized()*moveSpeed;
}

void Ball::move(GameObject *paddle)
{
    if(IsStuck){
        pos=QVector2D(paddle->pos.x()+paddle->size.x()*0.5 - radius, paddle->pos.y() - 2*radius);
    }else{
        float nextPosX, nextPosY;
        nextPosX = pos.x() + Velocity.x();
        nextPosY = pos.y() + Velocity.y();
        pos=QVector2D(nextPosX,nextPosY);
        if(nextPosX<0 ){
           this->pos=QVector2D(0,pos.y());
           Velocity=QVector2D(-Velocity.x(), Velocity.y());
        }else if(nextPosX > WINDOW_WIDTH - radius*2.0 ){
            this->pos=QVector2D(WINDOW_WIDTH - radius*2.0 ,pos.y());
            Velocity=QVector2D(-Velocity.x(), Velocity.y());
        }
        if(nextPosY<0){
            this->pos=QVector2D(pos.x(),0);
            Velocity=QVector2D(Velocity.x(), -Velocity.y());
        }
        if(nextPosY > WINDOW_HEIGHT){
            reset();
        }
    }
}

void Ball::reset()
{
    IsStuck = true;
    Velocity=QVector2D(3.0, -4.0);
}

Ball::~Ball()
{
}

float Ball::movSpeed()
{
    return  moveSpeed;
}
