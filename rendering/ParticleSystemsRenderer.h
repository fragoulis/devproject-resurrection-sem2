//*****************************************************************************
//**
//**   File:               ParticleSystemsRenderer.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"
#include "../ParticleSystem/PS_Base.h"
#include "../utility/EventManager.h"
#include "../control/OSinterface/Input.h"
#include "../GameLogic/GameEvents.h"
#include <vector>

class ParticleSystemsRenderer : public IRenderer, public EventListener<Key_GoingDown>, 
												  public EventListener<Enemy_Despawned>
{
public:
	ParticleSystemsRenderer();
	virtual ~ParticleSystemsRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	virtual void onEvent(Key_GoingDown&);
	virtual void onEvent(Enemy_Despawned&);

private:
	// bunch of ParticleSystems here I guess
	std::vector<PS_Base *>	m_psList;
};
