#pragma once
#include "IRenderer.h"
#include "TerrainRenderer.h"


/**
 * Renders the world!
 *
 */
class WorldRenderer : public IRenderer
{
public:
	WorldRenderer();
	virtual ~WorldRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

private:
	TerrainRenderer m_terrainRenderer;
};
