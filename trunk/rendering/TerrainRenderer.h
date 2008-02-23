#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameLogic.h"
class Terrain;
class Graphics;

class TerrainRenderer : public EventListener< Terrain_Changed >
{
public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Graphics& g) const;

	void onEvent(Terrain_Changed&);

private:
	Terrain* m_terrain;
};
