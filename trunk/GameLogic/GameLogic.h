#pragma once
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include <string>
#include <list>
class Terrain;
class Playership;
class Enemyship;
class Spawnpoint;
class Crater;
class Ebomb;


EVENT_TYPE(Level_Load, std::string);
SIMPLE_EVENT_TYPE(Level_Unload);
EVENT_TYPE(Terrain_Changed, Terrain*);
EVENT_TYPE(Player_Spawned, Playership*);
EVENT_TYPE(Enemy_Spawned, Enemyship*);


class GameLogic : public Singleton< GameLogic >
{
public:

	void loadLevel(const std::string& id);
	void unloadLevel();

	void update(float dt);

	void spawnEnemies(int count, int type);

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
