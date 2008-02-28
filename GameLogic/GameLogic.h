#pragma once
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include <string>
#include <list>
class Terrain;
class Playership;
class Enemyship;


EVENT_TYPE(Level_Load, std::string);
SIMPLE_EVENT_TYPE(Level_Unload);
EVENT_TYPE(Terrain_Changed, Terrain*);
EVENT_TYPE(Player_Spawned, Playership*);


class GameLogic : public Singleton< GameLogic >
{
public:

	void loadLevel(const std::string& id);
	void unloadLevel();

private:
	Terrain* m_terrain;
	Playership* m_playership;

	typedef std::list<Enemyship*> EnemyshipList;
	EnemyshipList m_enemyships;


	void despawnTerrain();
	void despawnPlayer();

	friend Singleton< GameLogic >;
	GameLogic();
	~GameLogic();
};
