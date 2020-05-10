#include "game_object.h"

GameObject::GameObject(QVector2D position, QVector2D objSize, uint32_t* texture2D, QVector3D objColor) :
    pos(position), size(objSize), texture(texture2D), color(objColor)
{
}

GameObject::GameObject(){}
GameObject::~GameObject(){}

