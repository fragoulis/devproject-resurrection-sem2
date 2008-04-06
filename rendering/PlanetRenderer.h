#pragma once
#include "irenderer.h"

class PlanetRenderer : public IRenderer
{
public:
	PlanetRenderer();
	virtual ~PlanetRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);
};
