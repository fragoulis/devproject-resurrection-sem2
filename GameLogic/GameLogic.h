#pragma once
#include "../utility/EventManager.h"
class Terrain;

EVENT_TYPE(Terrain_Changed, Terrain*);

class GameLogic
{
public:
	GameLogic();
	~GameLogic();
};
