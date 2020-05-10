#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H
/************************************************************************
 * this class is used to generate a glow effect to make the game more attactive
 *
 *
************************************************************************/
#include <QVector2D>
#include <QVector4D>

struct Particle {
    QVector2D  Position, Velocity;
    QVector4D Color;
    float Life;

    Particle() : Position(0.0, 0.0), Velocity(0.0, 0.0), Color(1.0, 1.0, 1.0, 1.0), Life(0.0) { }
};

class Ball;


class ParticleManager
{
public:
    // Constructor
    ParticleManager(unsigned int& texture, qint32 amount);
    void Update(float dt, Ball* ball, qint32 newParticles, QVector2D offset = QVector2D(0.0, 0.0));
    std::vector<Particle> particles;
    unsigned int& texture;
private:

    qint32 amount;
    void init();
    qint32 firstUnusedParticle();
    void respawnParticle(Particle &particle, Ball* ball, QVector2D offset = QVector2D(0.0f, 0.0f));
};

#endif // PARTICLE_MANAGER_H
