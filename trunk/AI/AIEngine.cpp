//*****************************************************************************
//**
//**   File:               AIEngine.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 18:24:39 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 133 $
//**
//*****************************************************************************

#include "AIEngine.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include <iostream>

AIEngine::AIEngine() : m_playership(0)
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
}

void AIEngine::onApplicationUnload()
{
	// TODO: delete allocated memory
}

void AIEngine::onEvent(Level_Load& ll)
{
	const ParserSection* ps = ll.getValue1();

	// TODO: make sure all AIs required for this level are loaded
	// If not, load them now.
}

void AIEngine::onEvent(Level_Unload&)
{
	// TODO: delete all level data
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

    // Give initial thruster power
    enemyship->setThrusterPower(50000.0f);

    m_enemylist.push_back(enemyship);
}

void AIEngine::onEvent(Enemy_Despawned& es)
{
	Enemyship* enemyship = es.getValue();
    m_enemylist.remove(enemyship);
}

void AIEngine::update(float dt)
{
	// TODO: do update for all enemies
    EnemyshipList::iterator i = m_enemylist.begin();
    for(; i != m_enemylist.end(); ++i )
    {
        Enemyship *enemyship = *i;
        
        // Get direction to the player ship
        Vector3 dir = m_playership->getPosition() - enemyship->getPosition();
        dir.normalize();
        enemyship->setThrusterDirection( dir );
    }
}

