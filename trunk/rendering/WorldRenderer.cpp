#include "WorldRenderer.h"

WorldRenderer :: WorldRenderer()
{
}

WorldRenderer :: ~WorldRenderer()
{
}


void WorldRenderer :: render(Graphics& g) const
{
	m_terrainRenderer.render(g);
}