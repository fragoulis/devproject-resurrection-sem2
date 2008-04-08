#include "AIBehaviour.h"
#include "States/IAIState.h"
#include "../AIEnemy.h"
#include <cassert>

AIBehaviour::AIBehaviour():
m_minTime(0.0f),
m_maxTime(0.0f),
m_enabled(0)
{}

void AIBehaviour::update( float dt, Playership *player, AIEnemy *enemy )
{
    int state = enemy->getState();

    if( m_stateList.size() > 1 )
    {
        if( enemy->readyToChange( dt ) )
        {
            state++;
            state %= (int)m_stateList.size();
        }
    }

    m_stateList[state]->update( dt, player, enemy );

    enemy->setState(state);
}