#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H
#include <QObject>
#include <QVector>
#include "game_brick.h"

class GameLevel : public QObject
{
    Q_OBJECT
public:
    GameLevel();
    virtual ~GameLevel();

    void load(const char* path, float w, float h );
    void init(QVector<QVector<qint32>>, float w, float h);
    QVector<Brick> bricks;
};
#endif // GAME_LEVEL_H
