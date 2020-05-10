/************************************************************************
 *
************************************************************************/
#include "particle_manager.h"
#include "game_ball.h"
#include <iostream>

ParticleManager::ParticleManager(unsigned int& tex, qint32 num)
    : texture(tex), amount(num)
{
    this->init();
}

void ParticleManager::Update(float dt, Ball* ball, qint32 newParticles, QVector2D offset)
{
    //
    for (qint32 i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], ball, offset);
    }
    //
    for (qint32 i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt * 1.0; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.setW(p.Color.w()-dt * 1.0);
        }
    }
}


void ParticleManager::init()
{

    for (qint32 i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// Stores the index of the last particle used
qint32 lastUsedParticle = 0;

qint32 ParticleManager::firstUnusedParticle()
{
    // First search from last used particle, this will usually return almost instantly
    for (qint32 i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (unsigned i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    lastUsedParticle = 0;
    return 0;
}

void ParticleManager::respawnParticle(Particle &particle, Ball* ball, QVector2D offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.Position = ball->pos + QVector2D(random,random) + offset;
    particle.Color = QVector4D(rColor, rColor, rColor, 1.0);
    particle.Life = 1.0f;
    particle.Velocity = ball->Velocity * 0.2f;
}
