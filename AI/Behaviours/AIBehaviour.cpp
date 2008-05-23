#include "AIBehaviour.h"
#include "States/IAIState.h"
#include "../AIEnemy.h"

AIBehaviour::AIBehaviour():
m_enabled(0),
m_minTime(0.0f),
m_maxTime(0.0f)
{}

void AIBehaviour::update( float dt, Playership *player, AIEnemy *enemy )
{
    int state = enemy->getState();

    if( m_stateList.size() > 1 )
    {
        if( enemy->readyToChange( dt ) )
        {
            m_stateList[state]->onEnd(enemy);

            state++;
            state %= (int)m_stateList.size();

            m_stateList[state]->onBegin(enemy);
        }
    }

    m_stateList[state]->update( dt, player, enemy );

    enemy->setState(state);
}