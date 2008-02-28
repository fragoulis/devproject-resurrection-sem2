#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameLogic.h"
#include "CoordinateModel.h"

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

	void addShadowCaster(const CoordinateModel& model);
	void removeShadowCaster(const CoordinateModel& model);

private:
	void _clearResources();

	Terrain* m_terrain;
	Model     * m_terrainModel;			// The terrain 'model'
	VBO       * m_vbo;					// The terrain will always have it's own VBO

	std::vector<CoordinateModel> m_shadowCasters;

	// Put any extra data that is needed goes here
};
