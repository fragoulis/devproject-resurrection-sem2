#pragma once
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include "../physics/PhysicsEvents.h"
#include <string>
#include <list>
class Terrain;
class Playership;
class Enemyship;
class Spawnpoint;
class Crater;
class Ebomb;


class GameLogic :
	public Singleton< GameLogic >,
	public EventListener< Collision_Player_Enemy >
{
public:

	void loadLevel(const std::string& id);
	void unloadLevel();
	void spawnEnemies(int count, int type);  // Spawnpoints has a tiny bit of game logic in it!
	void update(float dt);

	void onEvent(Collision_Player_Enemy&);

private:

	typedef std::list<Enemyship*> EnemyshipList;
	typedef std::list<Spawnpoint*> SpawnpointList;
	typedef std::list<Crater*> CraterList;
	typedef std::list<Ebomb*> EbombList;

	Terrain* m_terrain;
	Playership* m_playership;
	EnemyshipList m_enemyships;
	SpawnpointList m_spawnpoints;
	CraterList m_craters;
	EbombList m_ebombs;

	friend Singleton< GameLogic >;
	GameLogic();
	~GameLogic();
};
