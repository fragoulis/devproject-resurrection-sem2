//*****************************************************************************
//**
//**   File:               AIEngine.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 18:24:39 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 133 $
//**
//*****************************************************************************

#include "AIEngine.h"
//#include "States/IAIState.h"
#include "States/AIChase.h"
#include "States/AISurround.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ConfParser.h"
#include "../gfxutils/Misc/utils.h"
#include "../utility/RandomGenerator.h"
#include <iostream>
using namespace std;




AIEngine::AIEngine(): 
m_playership(0),
m_maxstates(0),
m_states(0)
{
	EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);
    EventManager::instance().registerEventListener< Player_Destroyed >(this);
}

AIEngine::~AIEngine()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void AIEngine::onApplicationLoad(const ParserSection& ps)
{
	// TODO: read ParserSection for info and load global AI data
    const string file = ps.getSection("EnemyFactory")->getVal("file");
    
    const ConfParser p(std::string("config/") + file);
    const string itp = p.getSection("EnemyFighter")->getVal("InitialThrusterPower");

    m_initialThrusterPower = FromString<float>(itp);

	// Allocate ai states according to level of difficulty
    m_maxstates = 2;
    m_states = new IAIState*[m_maxstates];
    m_states[0] = new AIChase;
    m_states[1] = new AISurround;
}

void AIEngine::onApplicationUnload()
{
	for( int i=0; i<m_maxstates; ++i )
    {
        delete m_states[i];
        m_states[i] = 0;
    }
	delete[] m_states;
    m_states = 0;
}

void AIEngine::onEvent(Level_Load& ll)
{
	//const ParserSection* ps = ll.getValue1();

	//// Allocate ai states according to level of difficulty
 //   m_maxstates = 2;
 //   
 //   m_states = new IAIState*[m_maxstates];
 //   m_states[0] = new AIChase;
 //   m_states[1] = new AISurround;
}

void AIEngine::onEvent(Level_Unload&)
{
 //   for( int i=0; i<m_maxstates; ++i )
 //   {
 //       delete m_states[i];
 //       m_states[i] = 0;
 //   }
	//delete[] m_states;
 //   m_states = 0;
}

void AIEngine::onEvent( Player_Spawned& es )
{
	m_playership = es.getValue();
}


void AIEngine::onEvent( Player_Destroyed& pd )
{
	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();

	// TODO: despawn all enemies? no idea!
}

void AIEngine::onEvent(Enemy_Spawned& es)
{
	Enemyship* enemyship = es.getValue();
    int type = enemyship->getType();

    // Give initial thruster power
    enemyship->setThrusterPower(m_initialThrusterPower);

    // Pick random state
    int index = RandomGenerator::GET_RANDOM_INT(0,m_maxstates);

    AIStateEnemyCouple sec;
    sec.enemy = enemyship;
    sec.state = m_states[index];
    m_enemylist.push_back(sec);
        
    //cerr << "Enemy spawned with ai state " << index << endl;
}

void AIEngine::onEvent(Enemy_Despawned& es)
{
	Enemyship* enemyship = es.getValue();

    StateEnemyCoupleList::iterator i = m_enemylist.begin();
    for(; i != m_enemylist.end(); ++i )
    {
        const AIStateEnemyCouple& couple = *i;
        if( couple.enemy == enemyship )
        {
            m_enemylist.erase(i);
            break;
        }
    }
}

void AIEngine::update(float dt)
{
    StateEnemyCoupleList::const_iterator i = m_enemylist.begin();
    for(; i != m_enemylist.end(); ++i )
    {
        const AIStateEnemyCouple& couple = *i;
        couple.state->update( m_playership, couple.enemy );
    }
}