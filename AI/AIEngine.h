//*****************************************************************************
//**
//**   File:               AIEngine.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"
#include "AIStateEnemyCouple.h"
class ParserSection;
class Enemyship;

/**
 * This controls the AI.
 * This includes behavior and controlling how enemies spawn (initial velocity and such)
 * The update function is called every frame before any other engine.
 */
class AIEngine :
	public Singleton< AIEngine >,
	public EventListener< Level_Load >,
	public EventListener< Level_Unload >,
	public EventListener< Player_Spawned >,
	public EventListener< Enemy_Spawned >,
    public EventListener< Enemy_Despawned >,
	public EventListener< Player_Destroyed >
{
private:
    typedef std::list<AIStateEnemyCouple> StateEnemyCoupleList;
	StateEnemyCoupleList m_enemylist;
    float m_initialThrusterPower;
    Playership* m_playership;

public:

	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void onEvent(Level_Load&);
	void onEvent(Level_Unload&);
	void onEvent(Player_Spawned&);
	void onEvent(Enemy_Spawned&);
    void onEvent(Enemy_Despawned&);
	void onEvent(Player_Destroyed&);

	void update(float dt);

private:
	friend Singleton< AIEngine >;
	AIEngine();
	virtual ~AIEngine();
};
