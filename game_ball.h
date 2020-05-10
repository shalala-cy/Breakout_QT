#ifndef GAME_BALL_H
#define GAME_BALL_H

#include "game_object.h"

class Brick;

class Ball: public GameObject{
public:
    // constructor
    Ball(QVector2D position=QVector2D(0.0, 0.0), float R=24, uint32_t* texture2D=nullptr, QVector3D objColor=QVector3D(0.0, 0.0, 0.0));
    void move(GameObject* paddle);
    void reset();
    bool IsStuck;
    ~Ball();
    float radius;
    QVector2D Velocity;
    float movSpeed();
private:
    float moveSpeed;
};

#endif // GAME_BALL_H
