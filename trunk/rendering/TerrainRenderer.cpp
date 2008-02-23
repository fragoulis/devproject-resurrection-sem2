#include "TerrainRenderer.h"

TerrainRenderer :: TerrainRenderer()
{
}

TerrainRenderer :: ~TerrainRenderer()
{
}


void TerrainRenderer :: render(Graphics& g) const
{
	// Do some magic to render the terrain
	// don't make assumptions about the render target (framebuffer/texture)
	// let graphics object take care of that
}

void TerrainRenderer :: onEvent(Terrain_Changed& tc)
{
	m_terrain = tc.getData();

	// Do some magic to create VBOs or whatever
}