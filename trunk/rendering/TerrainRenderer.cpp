#include "TerrainRenderer.h"
#include "../GameLogic/Terrain.h"

TerrainRenderer :: TerrainRenderer()
{
	EventManager::instance().registerEventListener(this);
}

TerrainRenderer :: ~TerrainRenderer()
{
}


void TerrainRenderer :: render(Graphics& g) const
{
	// Do some magic to render the terrain
}

void TerrainRenderer :: onEvent(Terrain_Changed& tc)
{
	m_terrain = tc.getData();
	const std::string& id = m_terrain->getID();

	// Do some magic to load VBOs or whatever
}