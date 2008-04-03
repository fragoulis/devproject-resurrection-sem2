//*****************************************************************************
//**
//**   File:               AIEngine.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 18:24:39 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 133 $
//**
//*****************************************************************************

#include "AIEngine.h"
#include "AIStateManager.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ConfParser.h"
#include "../gfxutils/Misc/utils.h"
#include "../utility/RandomGenerator.h"
#include <iostream>
using namespace std;




AIEngine::AIEngine(): 
m_playership(0)
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

    AIStateManager::safeInstance().init();
}

void AIEngine::onApplicationUnload()
{
    AIStateManager::destroy();
}

void AIEngine::onEvent(Level_Load& ll)
{
	const ParserSection* ps = ll.getValue1();
}

void AIEngine::onEvent(Level_Unload&)
{
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
    float time = RandomGenerator::GET_RANDOM_FLOAT(1.0f,10.0f);

    AIStateEnemyCouple sec;
    sec.setTimeToChange(time);
    sec.setEnemyship( enemyship );
    sec.setState( AIStateManager::instance().getRandomState() );
    m_enemylist.push_back(sec);
}

void AIEngine::onEvent(Enemy_Despawned& es)
{
	Enemyship* enemyship = es.getValue();

    StateEnemyCoupleList::iterator i = m_enemylist.begin();
    for(; i != m_enemylist.end(); ++i )
    {
        const AIStateEnemyCouple& couple = *i;
        if( couple.getEnemyship() == enemyship )
        {
            m_enemylist.erase(i);
            break;
        }
    }
}

void AIEngine::update(float dt)
{
    StateEnemyCoupleList::iterator i = m_enemylist.begin();
    for(; i != m_enemylist.end(); ++i )
    {
        AIStateEnemyCouple& couple = *i;
        couple.update( m_playership, dt );
    }
}