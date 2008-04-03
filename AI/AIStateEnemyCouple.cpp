#include "AIStateEnemyCouple.h"
#include "States/IAIState.h"
#include "../GameLogic/Enemies/Enemyship.h"

AIStateEnemyCouple::AIStateEnemyCouple():
m_timeToChange(1.0f),
m_currentTime(0.0f)
{}

void AIStateEnemyCouple::update( Playership *player, float dt )
{
    m_state->update( player, this, dt );
}

bool AIStateEnemyCouple::readyToChange( float dt )
{
    m_currentTime += dt;
    if( m_currentTime > m_timeToChange )
    {
        m_currentTime = 0.0f;
        return true;
    }

    return false;
}

const Point3& AIStateEnemyCouple::getEnemyPosition() const 
{ 
    return m_enemy->getPosition(); 
}

void AIStateEnemyCouple::setEnemyThrusterDirection( const Vector3& dir ) 
{ 
    m_enemy->setThrusterDirection( dir ); 
}