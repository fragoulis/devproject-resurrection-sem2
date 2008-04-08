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
#include "AIEnemy.h"
#include <map>
class ParserSection;
class ConfParser;
class Enemyship;
class AIBehaviour;
class IAIState;

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
    // Typedef containers
    typedef std::list<AIEnemy> EnemyList;
    typedef std::map<const std::string, AIBehaviour*> BehaviourList;
    typedef std::map<const std::string, IAIState*> StateList;

    // Typedef iterators
    typedef EnemyList::iterator EnemyListIt;
    typedef BehaviourList::iterator BehaviourListIt;
    typedef StateList::iterator StateListIt;

    EnemyList       m_enemyList;
    BehaviourList   m_behaviours;
    StateList       m_states;
    Playership      *m_playership;

    float m_maxThrusterPower;
    float m_minThrusterPower;

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

    void readStates(const ConfParser&);
    void readBehaviours(const ConfParser&);
    AIBehaviour* getRandomBehaviour();
};
