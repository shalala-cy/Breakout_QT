#ifndef GAME_BRICK_H
#define GAME_BRICK_H
#include "game_object.h"

class Brick: public GameObject{
public:
    // constructor
    Brick(QVector2D position=QVector2D(0.0, 0.0), QVector2D objSize=QVector2D(5.0, 3.0), uint32_t* texture2D=nullptr, QVector3D objColor=QVector3D(0.0, 0.0, 0.0), bool destroyed=false, bool solid=false);
    Brick(const Brick& bk); //copy constructor.  necessary!  Brick will be copied to Vector in game_level.h
    void destroyBrick();
    bool isDestroyed();
    bool isSolid();
    ~Brick();
private:
    bool IsDestroyed;
    bool IsSolid;
};

#endif // GAME_BRICK_H
