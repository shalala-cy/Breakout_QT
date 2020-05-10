#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <QObject>
#include <QVector2D>
#include <QVector3D>

class GameObject : public QObject
{
    Q_OBJECT
public:
    QVector2D pos;      //2D Position
    QVector2D size;     //size of object
    unsigned int* texture;
    QVector3D color;
    GameObject(QVector2D position=QVector2D(0.0, 0.0), QVector2D objSize=QVector2D(5.0, 3.0), uint32_t* texture2D=nullptr, QVector3D objColor=QVector3D(0.0, 0.0, 0.0));
    GameObject();
    virtual ~GameObject();
};
#endif // GAME_OBJECT_H
