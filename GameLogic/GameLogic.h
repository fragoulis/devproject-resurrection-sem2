#pragma once
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include <string>
class Terrain;


EVENT_TYPE(Terrain_Changed, Terrain*);
EVENT_TYPE(Level_Load, std::string);
SIMPLE_EVENT_TYPE(Level_Unload);


class GameLogic : public Singleton< GameLogic >
{
public:

	void loadLevel(const std::string& id);
	void unloadLevel();

private:
	Terrain* m_terrain;


	void despawnTerrain();

	friend Singleton< GameLogic >;
	GameLogic();
	~GameLogic();
};
