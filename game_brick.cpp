#include "game_brick.h"


Brick::Brick(QVector2D position, QVector2D objSize, uint32_t *texture2D, QVector3D objColor, bool destroyed, bool solid):
    GameObject(position, objSize, texture2D, objColor), IsDestroyed(destroyed), IsSolid(solid)
{
}

Brick::Brick(const Brick &bk) : GameObject(bk.pos, bk.size, bk.texture, bk.color), IsDestroyed(bk.IsDestroyed), IsSolid(bk.IsSolid)
{
}

void Brick::destroyBrick()
{
    if(!isSolid()){
        IsDestroyed = true;
    }
}

bool Brick::isDestroyed()
{
    return  IsDestroyed;
}

bool Brick::isSolid()
{
    return IsSolid;
}

Brick::~Brick()
{
}
