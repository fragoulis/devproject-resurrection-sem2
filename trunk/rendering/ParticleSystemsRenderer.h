#pragma once
#include "IRenderer.h"
#include "../ParticleSystem/PS_Base.h"
#include "../utility/EventManager.h"
#include "../control/OSinterface/Input.h"
#include <vector>

class ParticleSystemsRenderer : public IRenderer, public EventListener<Key_GoingDown>
{
public:
	ParticleSystemsRenderer();
	virtual ~ParticleSystemsRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	virtual void onEvent(Key_GoingDown&);

private:
	// bunch of ParticleSystems here I guess
	std::vector<PS_Base *>	_psList;
};
