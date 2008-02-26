#pragma once
#include "IRenderer.h"

class ParticleSystemsRenderer : public IRenderer
{
public:
	ParticleSystemsRenderer();
	virtual ~ParticleSystemsRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

private:
	// bunch of ParticleSystems here I guess
};
