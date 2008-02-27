#pragma once
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include <string>
#include <vector>
class Terrain;
class Playership;
class Enemyship;


EVENT_TYPE(Level_Load, std::string);
SIMPLE_EVENT_TYPE(Level_Unload);
EVENT_TYPE(Terrain_Changed, Terrain*);
EVENT_TYPE(Player_Spawned, Playership*);


class GameplayLogic : public Singleton< GameplayLogic >
{
public:

	void loadLevel(const std::string& id);
	void unloadLevel();

private:
	Terrain* m_terrain;
	Playership* m_playership;
	std::vector<Enemyship*> m_enemyships;


	void despawnTerrain();
	void despawnPlayer();

	friend Singleton< GameplayLogic >;
	GameplayLogic();
	~GameplayLogic();
};