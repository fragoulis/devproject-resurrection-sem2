#pragma once
#include "IRenderer.h"
#include "../ParticleSystem/PS_Base.h"
#include <vector>

class ParticleSystemsRenderer : public IRenderer
{
public:
	ParticleSystemsRenderer();
	virtual ~ParticleSystemsRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

private:
	// bunch of ParticleSystems here I guess
	std::vector<PS_Base *>	_psList;
};
