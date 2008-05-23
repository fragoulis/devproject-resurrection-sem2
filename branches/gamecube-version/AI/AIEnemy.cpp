#include "AIEnemy.h"
#include "Behaviours/AIBehaviour.h"
#include "Behaviours/States/IAIState.h"
#include "../utility/RandomGenerator.h"

AIEnemy::AIEnemy():
m_state(0),
m_timeToChange(0.0f),
m_currentTime(0.0f),
m_forceChange(0)
{}

void AIEnemy::setBehaviour( AIBehaviour *behaviour ) 
{ 
    m_state = 0;
    m_behaviour = behaviour;
    float min = behaviour->getMinTime();
    float max = behaviour->getMaxTime();
    m_timeToChange = RandomGenerator::GET_RANDOM_FLOAT(min,max);

    m_behaviour->getState(m_state)->onBegin(this);
}

void AIEnemy::update( float dt, Playership *player ){
    m_behaviour->update( dt, player, this );
}

bool AIEnemy::readyToChange( float dt )
{
    if( m_forceChange ) {
        m_forceChange = false;
        return true;
    }

    m_currentTime += dt;
    if( m_currentTime > m_timeToChange )
    {
        m_currentTime = 0.0f;
        return true;
    }

    return false;
}