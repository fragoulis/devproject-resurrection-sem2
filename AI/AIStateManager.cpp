#include "AIStateManager.h"
#include "States/AIChase.h"
#include "States/AISurround.h"
#include "../utility/RandomGenerator.h"
#include <iostream>

AIStateManager::~AIStateManager()
{
    StateList::iterator i = m_states.begin();
    for(; i != m_states.end(); ++i )
    {
        delete i->second;
        i->second = 0;
    }
}

void AIStateManager::init()
{
    // Load all states
    m_states.insert( std::make_pair( "Chase", new AIChase ) );
    m_states.insert( std::make_pair( "Surround", new AISurround ) );
}

IAIState* AIStateManager::getRandomState()
{
    const int max = (int)m_states.size();
    const int index = RandomGenerator::GET_RANDOM_INT(0,max);
    
    StateList::iterator iter = m_states.begin();
    for( int i=0; i<index; ++i, ++iter );

    std::cerr << "Enemy spawned with ai state " << index << std::endl;

    return iter->second;
}