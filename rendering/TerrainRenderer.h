#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameLogic.h"
class Terrain;
class Graphics;
class Model;
class VBO;

class TerrainRenderer : public EventListener< Terrain_Changed >
{
public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Graphics& g) const;

	void onEvent(Terrain_Changed&);

private:
	void _clearResources();

	Terrain* m_terrain;
	Model     * m_terrainModel;			// The terrain 'model'
	VBO       * m_vbo;					// The terrain will always have it's own VBO

	// Put any extra data that is needed goes here
};
