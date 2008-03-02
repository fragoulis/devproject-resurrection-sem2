#pragma once
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include "../physics/PhysicsEvents.h"
#include <string>
#include <list>
class ParserSection;
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

	//! Called by Application startup.
	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void loadLevel(const std::string& id);
	void unloadLevel();
	void spawnEnemies(int count, int type);  // Spawnpoint has a tiny bit of game logic in it!
	void setPlayerDirection(const Vector3& v);
	void update(float dt);

	void onEvent(Collision_Player_Enemy&);

private:

	typedef std::list<Enemyship*> EnemyshipList;
	typedef std::list<Spawnpoint*> SpawnpointList;
	typedef std::list<Crater*> CraterList;
	typedef std::list<Ebomb*> EbombList;

	Terrain* m_terrain;
	Playership* m_playership;
	Playership* m_playershipPrototype;
	EnemyshipList m_enemyships;
	SpawnpointList m_spawnpoints;
	CraterList m_craters;
	EbombList m_ebombs;

	void _deleteLevelData();

	friend Singleton< GameLogic >;
	GameLogic();
	~GameLogic();
};
