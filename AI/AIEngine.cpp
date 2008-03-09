//*****************************************************************************
//**
//**   File:               AIEngine.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "AIEngine.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"

AIEngine :: AIEngine() : m_playership(0)
{
	EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
}

AIEngine :: ~AIEngine()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void AIEngine :: onApplicationLoad(const ParserSection& ps)
{
	// TODO: read ParserSection for info and load global AI data
}

void AIEngine :: onApplicationUnload()
{
	// TODO: delete allocated memory
}

void AIEngine :: onEvent(Level_Load& ll)
{
	const ParserSection& ps = ll.getReference();

	// TODO: make sure all AIs required for this level are loaded
	// If not, load them now.
}

void AIEngine :: onEvent(Level_Unload&)
{
	// TODO: delete all level data
}

void AIEngine :: onEvent( Player_Spawned& es )
{
	m_playership = es.getPointer();
}


void AIEngine :: onEvent( Player_Destroyed& pd )
{
	Playership* ps = pd.getPointer();
	EnergyType type = pd.getValue();

	// TODO: despawn all enemies? no idea!
}

void AIEngine :: onEvent(Enemy_Spawned& es)
{
	Enemyship* enemy = es.getPointer();

	// TODO: Wrap es in a AIEnemyStateCouple and add it to a list
}

void AIEngine :: update(float dt)
{
	// TODO: do update for all enemies
}

